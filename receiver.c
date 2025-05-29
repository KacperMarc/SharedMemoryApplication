// SO IS 223A LAB12
// Kacper Marciszewski
// mk55816@zut.edu.pl

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>

#define READY_FLAG (1 << 1)
#define DATA_READY_FLAG (1 << 0)
#define SHM_DATA_OFFSET 3

#define SHM_SIZE 101
#define SEM_NAME "/sem_sync"

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

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <key> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    key_t key = atoi(argv[1]);
    char *output_filename = argv[2];

    int shmid = get_shared_block(key, SHM_SIZE);
    if (shmid == -1) {
        perror("shmget");
        return EXIT_FAILURE;
    }

    char *shared_memory = attach_memory_block(shmid);
    if (!shared_memory) {
        perror("shmat");
        return EXIT_FAILURE;
    }

    printf("Shared memory attached, key=%d\n", key);

    int fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    shared_memory[0] |= READY_FLAG;

    while (true) {
        while (!(shared_memory[0] & DATA_READY_FLAG)) {
            if (!(shared_memory[0] & READY_FLAG)) {
                close(fd);
                detach_memory_block(shared_memory);
                return EXIT_SUCCESS;
            }
            usleep(100);
        }

        uint16_t *bytes_count = (uint16_t *)(shared_memory + 1);
        ssize_t bytes_to_write = *bytes_count;

        ssize_t bytes_written = write(fd, shared_memory + SHM_DATA_OFFSET, bytes_to_write);
        if (bytes_written == -1) {
            perror("write");
            return EXIT_FAILURE;
        }

        printf("%ld bytes received.\n", bytes_written);

        shared_memory[0] &= ~DATA_READY_FLAG;
    }

    close(fd);
    detach_memory_block(shared_memory);

    return EXIT_SUCCESS;
}
