# LEVEL 03

## Code analysis

### Overview

The program:

1. Reads a numeric password from stdin (via `scanf`).
2. Passes the input to `test()` with a hard-coded constant.
3. Computes an XOR key as the difference between the constant and the input.
4. XOR-decrypts a hard-coded buffer with that key.
5. Compares the decrypted buffer with the string "Congratulations!".
6. If the comparison succeeds, the program spawns a shell.

### Execution flow

```text
main()
  ↓
  scanf("%d", &input)          ← user input
  ↓
  test(input, 322424845)        ← hard-coded constant
  ↓
  xor_key = 322424845 - input   ← key calculation
  ↓
  decrypt(xor_key)
  ↓
  XOR-decrypted buffer
  ↓
  strcmp(buffer, "Congratulations!") == 0 ?
  ↓
  system("/bin/sh")            ← SHELL spawned on success
```

### Simplified / pseudocode

```c
int main(void)
{
    int input;
    scanf("%d", &input);
    test(input, 322424845);  // 0x13371337 in hex
    return 0;
}

void test(int param_1, int param_2)
{
    int xor_key = param_2 - param_1;
    decrypt(xor_key);
}

void decrypt(int xor_key)
{
    /* hard-coded encrypted buffer */
    char encrypted[17] = {
        0x51, 0x7d, 0x7c, 0x75, 0x60, 0x73, 0x66, 0x67,
        0x7e, 0x73, 0x66, 0x7b, 0x7d, 0x7c, 0x61, 0x33, 0x00
    };

    /* XOR decrypt */
    for (int i = 0; i < 16; i++) {
        encrypted[i] = encrypted[i] ^ (char)xor_key;
    }

    /* Check and spawn shell */
    if (strcmp(encrypted, "Congratulations!") == 0) {
        system("/bin/sh");
    } else {
        puts("\nInvalid Password");
    }
}
```

---

## Understanding XOR

### XOR operator (`^`)

The XOR operator compares two bits:

| A | B | = |
|---|---|---|
| 0 | 0 | 0 |
| 0 | 1 | 1 |
| 1 | 0 | 1 |
| 1 | 1 | 0 |

Property: the operation is reversible:

```
A ^ B = C
C ^ B = A  // applying the same key reverses the operation
```

Example:

```
Q = 0x51 = 01010001 (binary)
C = 0x43 = 01000011 (binary)

  01010001  ('Q')
^ 01000011  ('C')
----------
  00010010  = 0x12 = 18
```

---

## Finding the XOR key

Known values:

| Item | Value |
|------|-------|
| Encrypted buffer | `0x51, 0x7d, 0x7c, ...` (hard-coded) |
| Expected plaintext | `"Congratulations!"` |
| First encrypted byte | `0x51` ('Q') |
| First expected byte | `0x43` ('C') |

If `C ^ key = Q`, then `Q ^ C = key`.

Compute:

```bash
printf "%d\n" $((0x51 ^ 0x43))
# prints: 18
```

So the XOR key is 18.

---

## Compute the required password

In `test()`:

```c
xor_key = param_2 - param_1
```

Where:

- `param_2` = 322424845 (hard-coded constant)
- `param_1` = user input
- `xor_key` must equal 18

So:

```
18 = 322424845 - input
input = 322424845 - 18 = 322424827
```

Verify:

```
322424845 - 322424827 = 18
```

---

## Solution

Password found: **322424827**

### Exploitation example

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

### token

Token found `kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf`
