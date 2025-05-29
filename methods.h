//
// Created by Kacper Marciszewski on 14/01/2025.
//

#ifndef METHODS_H
#define METHODS_H

#include <stdbool.h>
#include <sys/ipc.h>


#define SHM_SIZE 101
#define SEM_NAME "/sem_sync"
key_t return_key(char* filename);

int get_shared_block(key_t key, int block_size);
char *attach_memory_block(int block_id);
bool detach_memory_block(char *block);
bool destroy_memory_block(int shared_block_id);

#endif //METHODS_H
