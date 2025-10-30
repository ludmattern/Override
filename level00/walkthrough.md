# LEVEL 00

## Code analysis

### Decompiled code (Ghidra)

```c
bool main(void)
{
  int local_14[4];

  puts("***********************************");
  puts("* \t     -Level00 -\t\t  *");
  puts("***********************************");
  printf("Password:");
  __isoc99_scanf("%d", local_14); // read an integer into local_14[0]
  if (local_14[0] != 0x149c) { // 0x149c == 5276
    puts("\nInvalid Password!");
  }
  else {
    puts("\nAuthenticated!");
    system("/bin/sh");
  }
  return local_14[0] != 0x149c;
}
```

---

## What the program does

- Prints a banner and prompts the user for a numeric "Password".
- Reads input with `scanf` into an integer stored at `local_14[0]`.
- Compares the entered value against the constant `0x149c` (hex).
- If the value matches, the program prints "Authenticated!" and spawns a shell.
  Otherwise it prints "Invalid Password!" and exits.

## Key observations

- The comparison uses a fixed constant: `0x149c`.
- There are no format-string vulnerabilities or obvious buffer overflows in this snippet — it's a straight numeric check.
- The decompiled `scanf` target is a 4-element int array; the code only uses `local_14[0]` for the check.

## Exploitation / How to get a shell

1. Run the `level00` binary (or whichever executable corresponds to this level).
2. When prompted with "Password:", enter the decimal value of `0x149c`.

Then the program will call `system("/bin/sh")` and give you a shell.

## Solution

- Correct password (hex): 0x149c
- Correct password (decimal): 5276

Token found `uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL`
