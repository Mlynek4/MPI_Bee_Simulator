#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "flower.h"
#include "message.h"
#include "defs.h"
#include "utils.h"



void handle_req_flower(int rank, int src, Message msg, Message flowerQueue[], int *lmp_clock) {
    flowerQueue[src].id = src;
    flowerQueue[src].lmp_clock = msg.lmp_clock;
    (*lmp_clock)++;
    send_message(rank, ACK_FLOWER, src, *lmp_clock);
}

void handle_ack_flower(int rank, int src, Message msg, Message flowerQueue[], Message flowerQueueToSort[], int *lmp_clock, int size, int *reed, int *reedEggs, int *eggsLaid, int *state) {
    printf("%d: Bee %d gets ACK_FLOWER from %d\n", *lmp_clock, rank, src);
    flowerQueue[src].id = src;
    if (flowerQueue[src].lmp_clock == -1) {
        flowerQueue[src].lmp_clock = 10000 + rand() % 1000;
    }

    int allAck = 1;
    for (int i = 0; i < size; i++) {
        if (flowerQueue[i].id == -1) {
            allAck = 0;
            break;
        }
    }

    if (allAck) {
        for (int i = 0; i < size; i++) {
            flowerQueueToSort[i].id = flowerQueue[i].id;
            flowerQueueToSort[i].lmp_clock = flowerQueue[i].lmp_clock;
        }

        qsort(flowerQueueToSort, size, sizeof(Message), compare);

        for (int k = 0; k < K; k++) {
            if (flowerQueueToSort[k].id == rank) {
                *state = SUCC_FLOWER;
                printf("%d: Bee %d moves to state SUCC_FLOWER\n", *lmp_clock, rank);
                (*lmp_clock)++;

                for (int i = 0; i < size; i++) {
                    if (i != rank) {
                        send_message(rank, RELEASE_FLOWER, i, *lmp_clock);
                        send_message(rank, ACK_FLOWER, i, *lmp_clock);
                    }
                }

                *state = LAY_EGG;
                printf("%d: Bee %d moves to state LAY_EGG\n", *lmp_clock, rank);
                (*eggsLaid)++;
                printf("%d: Bee %d laid egg %d\n", *lmp_clock, rank, *eggsLaid);

                if (*eggsLaid == 5) {
                    int reedNumber = 0;
                    for (int i = 0; i < T; i++) {
                        if (reed[i] == rank) {
                            reedNumber = i;
                            reedEggs[i] += 5;
                            break;
                        }
                    }
                    (*lmp_clock)++;
                    for (int i = 0; i < size; i++) {
                        if (i != rank) {
                            send_message(rank, RELEASE_REED, i, reedNumber);
                        }
                    }
                    printf("\n\n%d: Bee %d finished laying eggs and releases reed %d\n\n", *lmp_clock, rank, reedNumber);
                } else {
                    *state = WAIT_FLOWER;
                    printf("%d: Bee %d moves to state WAIT_FLOWER\n", *lmp_clock, rank);
                    (*lmp_clock)++;
                    for (int i = 0; i < size; i++) {
                        if (i != rank) {
                            send_message(rank, REQ_FLOWER, i, *lmp_clock);
                        }
                    }
                    flowerQueue[rank].id = rank;
                    flowerQueue[rank].lmp_clock = *lmp_clock;
                }
            }
        }
    }
}


void handle_release_flower(int src, Message flowerQueue[]) {
    srand(time(NULL) + src);
    flowerQueue[src].lmp_clock = 10000 + rand() % 1000;
}
