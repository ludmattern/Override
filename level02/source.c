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