#ifndef UTILS_H
#define UTILS_H

#include "message.h"

void initialize_queues(Message reedQueue[], Message flowerQueue[], int size);

int compare(const void *a, const void *b);

// void handle_single_bee(int rank, int *reedEggs, int reed, int *eggsLaid, int *state);

void handle_single_bee(int rank, int *reedEggs, int *reed, int *eggsLaid, int *state);

#endif
