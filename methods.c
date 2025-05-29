//
// Created by Kacper Marciszewski on 14/01/2025.
//
#include "methods.h"
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>


key_t return_key(char* filename) {
    key_t key = ftok(filename, 0);

    return key;
}

int get_shared_block(key_t key, int block_size) {


    return shmget(key, block_size, 0644 | IPC_CREAT);
}

char *attach_memory_block(int block_id) {


    char *result = shmat(block_id, NULL, 0);
    if (result == (char *)-1) {
        perror("shmat");
        return NULL;
    }

    return result;
}

bool detach_memory_block(char *block) {
    return shmdt(block) != -1;
}

bool destroy_memory_block(int shared_block_id) {

    return shmctl(shared_block_id, IPC_RMID, NULL) != -1;
}