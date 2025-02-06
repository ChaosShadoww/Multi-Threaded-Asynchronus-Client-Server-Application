/* server.c - Multi-threaded Asynchronous Server */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <stdbool.h>


#define MAX_MSG_SIZE 256
#define CLIENT_REGISTER 1
#define SERVER_RESPONSE 2
#define MAX_CLIENTS 10

// Message structure for IPC
struct message {
    long msg_type;
    char msg_text[MAX_MSG_SIZE];
};

int msg_id;
void handle_signal(int sig) {
    printf("\nServer shutting down...\n");
    msgctl(msg_id, IPC_RMID, NULL);
    exit(0);
}



// Function executed by the child thread
void *child_thread_function(void *arg) {

// Loop to print numbers from 1 to the given limit
for (int i = 1; i <= limit; i++) {
printf("Child thread: %d\n", i);
sleep(1); // Simulate some delay
}
printf("Child thread: Finished counting.\n");
pthread_exit(NULL); // Exit the thread
}

void *client_handler(void *arg) 
{
    int limit = *(int *)arg; // Retrieve the argument (cast void* to int*)
    printf("Child thread: Started counting from 1 to %d.\n", limit);

    typedef struct {
        int id;
        bool is_hidden;
    } Client;
    

    Client clients[MAX_CLIENTS];
    int client_count = 0;


    struct message msg;
    while (1) {
        if (msgrcv(msg_id, &msg, sizeof(msg.msg_text), CLIENT_REGISTER, 0) > 0) {
            printf("Received command: %s\n", msg.msg_text);

            // Handle user-defined commands
            if (strncmp(msg.msg_text, "REGISTER", 8) == 0) {
                if (client_count < MAX_CLIENTS) {
                    clients[client_count].id = client_count;
                    clients[client_count].is_hidden = false;
                    client_count++;
                    printf("Client %d registered successfully.\n", client_count);
                } else {
                    printf("Max client limit reached.\n");
                }
            } 
            else if (strncmp(msg.msg_text, "EXIT", 4) == 0) {
                printf("Client disconnected.\n");
                client_count--;
            } 
            else if (strncmp(msg.msg_text, "LIST", 4) == 0) {
                bool any_visible = false;
                printf("Active clients:\n");
                for (int i = 0; i < client_count; i++) {
                    if (!clients[i].is_hidden) {
                        printf("Client %d\n", clients[i].id);
                        any_visible = true;
                    }
                }
                if (!any_visible) {
                    printf("All Clients Are Hidden...\n");
                }
            } 
            else if (strncmp(msg.msg_text, "HIDE", 4) == 0) {
                printf("Client requested to be hidden.\n");
                for (int i = 0; i < client_count; i++) {
                    if (!clients[i].is_hidden) {
                        clients[i].is_hidden = true;
                        printf("You Are Now Hidden...\n");
                        break;
                    }
                }
            } 
            else if (strncmp(msg.msg_text, "UNHIDE", 6) == 0) {
                printf("Client requested to be unhidden.\n");
                for (int i = 0; i < client_count; i++) {
                    if (clients[i].is_hidden) {
                        clients[i].is_hidden = false;
                        printf("You Are Now Visible Again...\n");
                        break;
                    }
                }
            } 
            else if (strncmp(msg.msg_text, "exit", 4) == 0) {
                printf("Ignored 'exit' command as it may exit the shell session...\n");
            } 
            else if (strncmp(msg.msg_text, "SHUTDOWN", 8) == 0) {
                printf("Server shutting down...\n");
                msgctl(msg_id, IPC_RMID, NULL);
                exit(0);
            } 
            else {
                // If it's not a user-defined command, treat it as a shell command
                system(msg.msg_text);
            }
        }
    }
    return NULL;
}


int main() 
{
    pthread_t thread_id; // Thread identifier
    int count_limit = 10; // Argument to pass to the child thread

    key_t key = ftok("server", 65);
    msg_id = msgget(key, 0666 | IPC_CREAT);
    signal(SIGINT, handle_signal);
    // Create a child thread

    if (pthread_create(&thread_id, NULL, client_handler, &count_limit) != 0) 
    {
        perror("Failed to create client handler thread");
        exit(EXIT_FAILURE);
    }
    // Detach the thread so it can run independently
    // Explore the differnce between pthread_detach() and pthread_join()
    if (pthread_detach(thread_id) != 0) 
    {
        perror("Failed to detach thread");
        exit(EXIT_FAILURE);
    }


    // Main thread continues working independently
    printf("Main thread: Child thread is detached and running in background.\n");
    // Simulate main thread work
    for (int i = 1; i <= 5; i++) {
    printf("Main thread: Doing work %d...\n", i);
    sleep(1);}
    printf("Main thread: Finished. Exiting without waiting for the child thread");
    return 0;
    // printf("Server started. Waiting for clients...\n");
    // while (1) pause();
    // return 0;
}





