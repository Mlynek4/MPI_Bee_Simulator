#ifndef REED_H
#define REED_H

#include "message.h"

void initialize_reed(int reed[], int size);
void handle_req_reed(int rank, int src, Message msg, Message reedQueue[], int *lmp_clock);
// void handle_release_reed(int src, Message *reedQueue, int *reedEggs, int *reedCount, int *beesCount);
void handle_ack_reed(int rank, int src, Message msg, Message reedQueue[], Message reedQueueToSort[], Message flowerQueue[], int *lmp_clock, int size, int beesCount, int reed[], int reedEggs[], int reedAvail[], int *state);
void handle_release_reed(int src, Message *reedQueue, int *reedEggs, int *reedCount, int *beesCount, int *reedAvail, int rank, int *lmp_clock);
void handle_ack_death(int rank, int src, int *lmp_clock, Message *reedQueue,  int size);


#endif
