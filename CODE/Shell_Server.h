#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


void *child_thread_function(void *arg);
void *register_funct(void *arg);
void *exit_funct(void *arg);
void *list_funct(void *arg);
void *hide_funct(void *arg);
void *unhide_funct(void *arg);
void *ignore_exit_funct(void *arg);