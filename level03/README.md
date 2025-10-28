# LEVEL 03

## Analyse du code

### Vue d'ensemble

Le programme :
1. Demande un **mot de passe numérique** (via `scanf`)
2. Le passe à la fonction `test()` avec une constante
3. Calcule une **clé XOR** basée sur la différence
4. Déchiffre un buffer avec cette clé
5. Compare le résultat avec `"Congratulations!"`
6. Lance un shell si la comparaison réussit

### Flux d'exécution

```
main()
  ↓
  scanf("%d", &input)          ← Notre input
  ↓
  test(input, 322424845)        ← Constante hardcodée
  ↓
  xor_key = 322424845 - input  ← Calcul de la clé
  ↓
  decrypt(xor_key)
  ↓
  Buffer chiffré XOR avec clé
  ↓
  strcmp(buffer, "Congratulations!") == 0 ?
  ↓
  system("/bin/sh")             ← SHELL !
```

### Code simplifié

```c
int main(void)
{
    int input;
    scanf("%d", &input);
    test(input, 322424845);  // 0x13371337 en hex
    return 0;
}

void test(int param_1, int param_2)
{
    int xor_key = param_2 - param_1;
    decrypt(xor_key);
}

void decrypt(int xor_key)
{
    // Buffer chiffré hardcodé
    char encrypted[17] = {
        0x51, 0x7d, 0x7c, 0x75, 0x60, 0x73, 0x66, 0x67,
        0x7e, 0x73, 0x66, 0x7b, 0x7d, 0x7c, 0x61, 0x33, 0x00
    };
    
    // Déchiffrement XOR
    for (int i = 0; i < 16; i++) {
        encrypted[i] = encrypted[i] ^ (char)xor_key;
    }
    
    // Vérification
    if (strcmp(encrypted, "Congratulations!") == 0) {
        system("/bin/sh");
    } else {
        puts("\nInvalid Password");
    }
}
```

---

## Comprendre le chiffrement XOR

### Opérateur XOR (`^`)

L'opérateur **XOR** (eXclusive OR) compare deux bits :

| A | B | = |
|---|---|---|
| 0 | 0 | 0 |
| 0 | 1 | 1 |
| 1 | 0 | 1 |
| 1 | 1 | 0 |

**Règle :** Le résultat est `1` si les bits sont **différents**, `0` s'ils sont **identiques**.

### Propriété magique : Réversibilité

```
A ^ B = C
C ^ B = A  ← La même clé déchiffre !
```

### Exemple de calcul XOR

```
Q = 0x51 = 01010001 (binaire)
C = 0x43 = 01000011 (binaire)

  01010001  ('Q')
^ 01000011  ('C')
----------
  00010010  = 0x12 = 18
```

---

## Trouver la clé XOR

### Données connues

| Élément | Valeur |
|---------|--------|
| Buffer chiffré | `0x51, 0x7d, 0x7c, ...` (hardcodé) |
| Résultat attendu | `"Congratulations!"` |
| Premier caractère chiffré | `0x51` (Q) |
| Premier caractère attendu | `0x43` (C) |

### Calcul de la clé

Donc si 'C' ^ Xor_key = 'Q', alors 'Q' ^ 'C' = xor_key

```bash
printf "%d\n" $((0x51 ^ 0x43))
# Affiche : 18
```

**La clé XOR est 18 !**

---

## Calculer le mot de passe

### Formule

Dans la fonction `test()` :
```c
xor_key = param_2 - param_1
```

Où :
- `param_2` = **322424845** (constante hardcodée)
- `param_1` = **notre input**
- `xor_key` doit valoir **18**

### Calcul

```
xor_key (18) = 322424845 - input

322424845 - 18 = input(322424827)
```

### Vérification

```
322424845 - 322424827 = 18
```

---

## Solution

### Mot de passe trouvé : **322424827**

### Exploitation

```bash
level03@OverRide:~$ ./level03 
***********************************
*               level03         **
***********************************
Password:322424827
$ whoami
level04
$ cat /home/users/level04/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
```

### Token level04

```
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
```