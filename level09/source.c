#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void secret_backdoor(void)
{
    char buffer[128];
    
    fgets(buffer, 0x80, stdin);
    system(buffer);
}

void set_username(char *data)
{
    char username_buffer[140];
    int i;
    
    // Initialiser le buffer à zéro
    memset(username_buffer, 0, sizeof(username_buffer));
    
    puts(">: Enter your username");
    printf(">>: ");
    fgets(username_buffer, 0x80, stdin);
    
    // Copier jusqu'à 41 caractères (0x29) dans data + 0x8c (offset 140)
    for (i = 0; i < 0x29 && username_buffer[i] != '\0'; i++) {
        data[0x8c + i] = username_buffer[i];
    }
    
    printf(">: Welcome, %s", data + 0x8c);
}

void set_msg(char *data)
{
    char message_buffer[1024];
    
    // Initialiser le buffer à zéro
    memset(message_buffer, 0, sizeof(message_buffer));
    
    puts(">: Msg @Unix-Dude");
    printf(">>: ");
    fgets(message_buffer, 0x400, stdin);
    
    // Copier le message en utilisant la longueur stockée à data + 0xb4 (offset 180)
    strncpy(data, message_buffer, *(int *)(data + 0xb4));
}

void handle_msg(void)
{
    char buffer[140];         // local_c8
    long padding1;            // local_3c
    long padding2;            // local_34
    long padding3;            // local_2c
    long padding4;            // local_24
    long padding5;            // local_1c
    int msg_len;              // local_14
    
    padding1 = 0;
    padding2 = 0;
    padding3 = 0;
    padding4 = 0;
    padding5 = 0;
    msg_len = 0x8c;  // 140 en décimal
    
    set_username(buffer);
    set_msg(buffer);
    puts(">: Msg sent!");
}

int main(void)
{
    puts("--------------------------------------------\n"
         "|   ~Welcome to l33t-m$n ~    v1337        |\n"
         "--------------------------------------------");
    handle_msg();
    return 0;
}
