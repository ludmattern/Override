undefined8 main(void)
{
  puts(
      "--------------------------------------------\n|   ~Welcome to l33t-m$n ~    v1337        |\n- -------------------------------------------"
      );
  handle_msg();
  return 0;
}


void handle_msg(void)
{
  undefined1 local_c8 [140];
  undefined8 local_3c;
  undefined8 local_34;
  undefined8 local_2c;
  undefined8 local_24;
  undefined8 local_1c;
  undefined4 local_14;
  
  local_3c = 0;
  local_34 = 0;
  local_2c = 0;
  local_24 = 0;
  local_1c = 0;
  local_14 = 0x8c;
  set_username(local_c8);
  set_msg(local_c8);
  puts(">: Msg sent!");
  return;
}

void set_username(long param_1)
{
  long lVar1;
  char *pcVar2;
  char local_98 [140];
  int local_c;
  
  pcVar2 = local_98;
  for (lVar1 = 0x10; lVar1 != 0; lVar1 = lVar1 + -1) {
    pcVar2[0] = '\0';
    pcVar2[1] = '\0';
    pcVar2[2] = '\0';
    pcVar2[3] = '\0';
    pcVar2[4] = '\0';
    pcVar2[5] = '\0';
    pcVar2[6] = '\0';
    pcVar2[7] = '\0';
    pcVar2 = pcVar2 + 8;
  }
  puts(">: Enter your username");
  printf(">>: ");
  fgets(local_98,0x80,_stdin);
  for (local_c = 0; (local_c < 0x29 && (local_98[local_c] != '\0')); local_c = local_c + 1) {
    *(char *)(param_1 + 0x8c + (long)local_c) = local_98[local_c];
  }
  printf(">: Welcome, %s",param_1 + 0x8c);
  return;
}

void set_msg(char *param_1)
{
  long lVar1;
  char *pcVar2;
  char local_408 [1024];
  
  pcVar2 = local_408;
  for (lVar1 = 0x80; lVar1 != 0; lVar1 = lVar1 + -1) {
    pcVar2[0] = '\0';
    pcVar2[1] = '\0';
    pcVar2[2] = '\0';
    pcVar2[3] = '\0';
    pcVar2[4] = '\0';
    pcVar2[5] = '\0';
    pcVar2[6] = '\0';
    pcVar2[7] = '\0';
    pcVar2 = pcVar2 + 8;
  }
  puts(">: Msg @Unix-Dude");
  printf(">>: ");
  fgets(local_408,0x400,_stdin);
  strncpy(param_1,local_408,(long)*(int *)(param_1 + 0xb4));
  return;
}

void secret_backdoor(void)
{
  char local_88 [128];
  
  fgets(local_88,0x80,_stdin);
  system(local_88);
  return;
}