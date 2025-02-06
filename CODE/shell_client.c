/* client.c - Multi-threaded Asynchronous Client */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_MSG_SIZE 256
#define CLIENT_REGISTER 1

// Message structure for IPC
struct message {
    long msg_type;
    char msg_text[MAX_MSG_SIZE];
};

int main() {
    key_t client_key = ftok("client", 75);
    int client_msg_id = msgget(client_key, 0666 | IPC_CREAT);
    struct message client_msg;
    
    while (1) {
        printf("Enter command: ");
        fgets(client_msg.msg_text, MAX_MSG_SIZE, stdin);
        client_msg.msg_text[strcspn(client_msg.msg_text, "\n")] = 0;
        client_msg.msg_type = CLIENT_REGISTER;
        msgsnd(client_msg_id, &client_msg, sizeof(client_msg.msg_text), 0);
    }
    return 0;
}