#ifndef FLOWER_H
#define FLOWER_H

#include "message.h"

void handle_req_flower(int rank, int src, Message msg, Message flowerQueue[], int *lmp_clock);
void handle_ack_flower(int rank, int src, Message msg, Message flowerQueue[], Message flowerQueueToSort[], int *lmp_clock, int size, int *reed, int *reedEggs, int *eggsLaid, int *state);

void handle_release_flower(int src, Message flowerQueue[]);

#endif
