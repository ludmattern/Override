bool main(void)
{
  int local_14[4];

  puts("***********************************");
  puts("* \t     -Level00 -\t\t  *");
  puts("***********************************");
  printf("Password:");
  __isoc99_scanf("%d", local_14); // read an integer into local_14[0]
  if (local_14[0] != 0x149c) { // 0x149c == 5276
    puts("\nInvalid Password!");
  }
  else {
    puts("\nAuthenticated!");
    system("/bin/sh");
  }
  return local_14[0] != 0x149c;
}
