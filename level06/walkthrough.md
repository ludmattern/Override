# level06

1. Overview

   * There is no practical stack overflow path here (stack canary + checks). The intended path is to **reproduce the serial algorithm**.
   * Anti‑debug: `ptrace(PTRACE_TRACEME)` inside `auth` makes it fail under a debugger.

2. Relevant code (pseudo‑C)

```c
int main(void) {
  char login[0x20];
  printf("-> Enter Login: ");
  fgets(login, 0x20, stdin);
  printf("-> Enter Serial: ");
  unsigned int serial; scanf("%u", &serial);  // conceptual representation
  if (auth(login, serial) == 0) {
    puts("Authenticated!");
    system("/bin/sh");
  }
}

int auth(char *login, unsigned int serial) {
  size_t n = strcspn(login, "\n");
  login[n] = '\0';                        // strip newline
  size_t L = strnlen(login, 0x20);
  if ((int)L < 6) return 1;               // min length 6

  long t = ptrace(PTRACE_TRACEME);        // anti‑debug
  if (t == -1) {
    puts("Tampering detected");
    return 1;
  }

  unsigned int acc = ((int)login[3] ^ 0x1337U) + 0x5eeded;  // seed from 4th char
  for (int i = 0; i < (int)L; i++) {
    if (login[i] < ' ') return 1;         // reject non‑printable bytes
    acc = acc + (((int)login[i] ^ acc) % 0x539);
  }
  return (serial == acc) ? 0 : 1;
}
```

3. Constraints

* **Length**: `6 ≤ L ≤ 32` (buffer is 0x20; length checked with `strnlen`).
* **Characters**: each byte in `login` must be printable (`>= ' '` → 0x20).
* **Anti‑debug**: do not run under GDB/strace; otherwise `auth` will fail.
* **Canary**: `__stack_chk_fail` present → no trivial overflow.

4. Serial algorithm (to reproduce)

Given a valid `login`, compute the expected serial as:

```
acc = ( login[3] ^ 0x1337 ) + 0x5eeded
for each character c in login:
    acc = acc + ( ( c ^ acc ) mod 0x539 )
expected_serial = acc  // 32‑bit unsigned on x86
```

Notes:

* `login[3]` is the **fourth** character (0‑based index 3).
* Arithmetic is effectively 32‑bit unsigned.

5. Exploitation / Steps to get the shell

1) Choose a login string (≥ 6 printable characters).
2) Compute the serial with the script below.
3) Run the binary **without a debugger**, enter the same login and the computed serial.
4) On success, you get a shell; read the next level token.

6. Python script

```python
login = "jgavairo"  # example
val = ((ord(login[3]) ^ 0x1337) + 0x5eeded)
for ch in login:
    val = val + ((ord(ch) ^ val) % 0x539)
print(val)
```

**Example (login = `jgavairo`):**

```
6231547
```

7. Example run

```text
-> Enter Login: jgavairo
-> Enter Serial: 6231547
Authenticated!
$ # program shell
$ # cat /home/users/level07/.pass   # typical path depending on the platform
```

8. Token

Token found `GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8`