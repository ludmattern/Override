
undefined4 main(undefined4 param_1,int *param_2,int *param_3)
{
  char cVar1;
  byte bVar2;
  int iVar3;
  uint uVar4;
  byte *pbVar5;
  undefined4 *puVar6;
  char *pcVar7;
  byte *pbVar8;
  int in_GS_OFFSET;
  bool bVar9;
  bool bVar10;
  bool bVar11;
  byte bVar12;
  int *local_1c8;
  int *local_1c4;
  undefined4 local_1bc [100];
  undefined4 local_2c;
  byte local_28 [20];
  int local_14;
  
  bVar12 = 0;
  local_1c4 = param_2;
  local_1c8 = param_3;
  local_14 = *(int *)(in_GS_OFFSET + 0x14);
  local_2c = 0;
  local_28[0] = 0;
  local_28[1] = 0;
  local_28[2] = 0;
  local_28[3] = 0;
  local_28[4] = 0;
  local_28[5] = 0;
  local_28[6] = 0;
  local_28[7] = 0;
  local_28[8] = 0;
  local_28[9] = 0;
  local_28[10] = 0;
  local_28[0xb] = 0;
  local_28[0xc] = 0;
  local_28[0xd] = 0;
  local_28[0xe] = 0;
  local_28[0xf] = 0;
  local_28[0x10] = 0;
  local_28[0x11] = 0;
  local_28[0x12] = 0;
  local_28[0x13] = 0;
  puVar6 = local_1bc;
  for (iVar3 = 100; iVar3 != 0; iVar3 = iVar3 + -1) {
    *puVar6 = 0;
    puVar6 = puVar6 + 1;
  }
  for (; *local_1c4 != 0; local_1c4 = local_1c4 + 1) {
    uVar4 = 0xffffffff;
    pcVar7 = (char *)*local_1c4;
    do {
      if (uVar4 == 0) break;
      uVar4 = uVar4 - 1;
      cVar1 = *pcVar7;
      pcVar7 = pcVar7 + (uint)bVar12 * -2 + 1;
    } while (cVar1 != '\0');
    memset((void *)*local_1c4,0,~uVar4 - 1);
  }
  for (; *local_1c8 != 0; local_1c8 = local_1c8 + 1) {
    uVar4 = 0xffffffff;
    pcVar7 = (char *)*local_1c8;
    do {
      if (uVar4 == 0) break;
      uVar4 = uVar4 - 1;
      cVar1 = *pcVar7;
      pcVar7 = pcVar7 + (uint)bVar12 * -2 + 1;
    } while (cVar1 != '\0');
    memset((void *)*local_1c8,0,~uVar4 - 1);
  }
  puts(
      "----------------------------------------------------\n  Welcome to wil\'s crappy number stora ge service!   \n----------------------------------------------------\n Commands:                                          \n    store - store a number into the data storage    \n    read  - read a number from the data storage     \n    quit  - exit the program                        \n----------------------------------------------------\n   wil has reserved some storage :>                 \n----------------------------------------------------\n"
      );
  do {
    printf("Input command: ");
    local_2c = 1;
    fgets((char *)local_28,0x14,stdin);
    uVar4 = 0xffffffff;
    pbVar5 = local_28;
    do {
      if (uVar4 == 0) break;
      uVar4 = uVar4 - 1;
      bVar2 = *pbVar5;
      pbVar5 = pbVar5 + (uint)bVar12 * -2 + 1;
    } while (bVar2 != 0);
    uVar4 = ~uVar4;
    bVar9 = uVar4 == 1;
    bVar11 = uVar4 == 2;
    *(undefined1 *)((int)&local_2c + uVar4 + 2) = 0;
    iVar3 = 5;
    pbVar5 = local_28;
    pbVar8 = (byte *)"store";
    do {
      if (iVar3 == 0) break;
      iVar3 = iVar3 + -1;
      bVar9 = *pbVar5 < *pbVar8;
      bVar11 = *pbVar5 == *pbVar8;
      pbVar5 = pbVar5 + (uint)bVar12 * -2 + 1;
      pbVar8 = pbVar8 + (uint)bVar12 * -2 + 1;
    } while (bVar11);
    bVar10 = false;
    bVar9 = (!bVar9 && !bVar11) == bVar9;
    if (bVar9) {
      local_2c = store_number(local_1bc);
    }
    else {
      iVar3 = 4;
      pbVar5 = local_28;
      pbVar8 = &DAT_08048d61;
      do {
        if (iVar3 == 0) break;
        iVar3 = iVar3 + -1;
        bVar10 = *pbVar5 < *pbVar8;
        bVar9 = *pbVar5 == *pbVar8;
        pbVar5 = pbVar5 + (uint)bVar12 * -2 + 1;
        pbVar8 = pbVar8 + (uint)bVar12 * -2 + 1;
      } while (bVar9);
      bVar11 = false;
      bVar9 = (!bVar10 && !bVar9) == bVar10;
      if (bVar9) {
        local_2c = read_number(local_1bc);
      }
      else {
        iVar3 = 4;
        pbVar5 = local_28;
        pbVar8 = &DAT_08048d66;
        do {
          if (iVar3 == 0) break;
          iVar3 = iVar3 + -1;
          bVar11 = *pbVar5 < *pbVar8;
          bVar9 = *pbVar5 == *pbVar8;
          pbVar5 = pbVar5 + (uint)bVar12 * -2 + 1;
          pbVar8 = pbVar8 + (uint)bVar12 * -2 + 1;
        } while (bVar9);
        if ((!bVar11 && !bVar9) == bVar11) {
          if (local_14 == *(int *)(in_GS_OFFSET + 0x14)) {
            return 0;
          }
                    /* WARNING: Subroutine does not return */
          __stack_chk_fail();
        }
      }
    }
    if (local_2c == 0) {
      printf(" Completed %s command successfully\n",local_28);
    }
    else {
      printf(" Failed to do %s command\n",local_28);
    }
    local_28[0] = 0;
    local_28[1] = 0;
    local_28[2] = 0;
    local_28[3] = 0;
    local_28[4] = 0;
    local_28[5] = 0;
    local_28[6] = 0;
    local_28[7] = 0;
    local_28[8] = 0;
    local_28[9] = 0;
    local_28[10] = 0;
    local_28[0xb] = 0;
    local_28[0xc] = 0;
    local_28[0xd] = 0;
    local_28[0xe] = 0;
    local_28[0xf] = 0;
    local_28[0x10] = 0;
    local_28[0x11] = 0;
    local_28[0x12] = 0;
    local_28[0x13] = 0;
  } while( true );
}


undefined4 store_number(int param_1)
{
  uint uVar1;
  uint uVar2;
  undefined4 uVar3;
  
  printf(" Number: ");
  uVar1 = get_unum();
  printf(" Index: ");
  uVar2 = get_unum();
  if ((uVar2 % 3 == 0) || (uVar1 >> 0x18 == 0xb7)) {
    puts(" *** ERROR! ***");
    puts("   This index is reserved for wil!");
    puts(" *** ERROR! ***");
    uVar3 = 1;
  }
  else {
    *(uint *)(uVar2 * 4 + param_1) = uVar1;
    uVar3 = 0;
  }
  return uVar3;
}

undefined4 read_number(int param_1)
{
  int iVar1;
  
  printf(" Index: ");
  iVar1 = get_unum();
  printf(" Number at data[%u] is %u\n",iVar1,*(undefined4 *)(iVar1 * 4 + param_1));
  return 0;
}


undefined4 get_unum(void)
{
  undefined4 local_10 [3];
  
  local_10[0] = 0;
  fflush(stdout);
  __isoc99_scanf(&DAT_08048ad0,local_10);
  clear_stdin();
  return local_10[0];
}


/*
                            DAT_08048ad0                                    XREF[2]:     get_unum:08048601(*), 
                                                                                          get_unum:0804860d(*)  
        08048ad0 25              ??         25h    %
        08048ad1 75              ??         75h    u
        08048ad2 00              ??         00h

                            DAT_08048d62                                    XREF[1]:     main:0804890f(R)  
        08048d62 65              undefined1 65h
        08048d63 61              ??         61h    a
        08048d64 64              ??         64h    d
        08048d65 00              ??         00h

                            DAT_08048d66                                    XREF[2]:     main:08048942(*), 
                                                                                          main:08048950(R)  
        08048d66 71              undefined1 71h
    
                            DAT_08048d67                                    XREF[1]:     main:08048950(R)  
        08048d67 75              undefined1 75h
        08048d68 69              ??         69h    i
        08048d69 74              ??         74h    t
        08048d6a 00              ??         00h

*/