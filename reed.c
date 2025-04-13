#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "reed.h"
#include "message.h"
#include "defs.h"
#include "utils.h"



// void initialize_reed(int reed[], int size) {
//     for (int i = 0; i < size; i++) {
//         reed[i] = -1;
//     }
// }

void handle_req_reed(int rank, int src, Message msg, Message reedQueue[], int *lmp_clock) {
    reedQueue[src].id = src;
    reedQueue[src].lmp_clock = msg.lmp_clock;
    (*lmp_clock)++;
    send_message(rank, ACK_REED, src, *lmp_clock);
}

void handle_ack_reed(int rank, int src, Message msg, Message reedQueue[], Message reedQueueToSort[], Message flowerQueue[], int *lmp_clock, int size, int beesCount, int reed[], int reedEggs[], int reedAvail[], int *state) {
            // Sprawdzenie, czy wszystkie ACK_REED zostały odebrane
            // printf("%d: Bee %d gets ACK_REED from %d\n", lmp_clock, rank, src);
            // printf("Bee %d - reedQueue[src].id = %d, reedQueue[src].lmp_clock = %d\n", rank, reedQueue[src].id, reedQueue[src].lmp_clock);
            if(reedQueue[src].id == -1){
                reedQueue[src].id = src;
                reedQueue[src].lmp_clock = 10000 + rand()%1000;
            // } else if(reedQueue[src].id == -3){
            //     reedAvail[reedQueue[src].lmp_clock] = 1;
            //     reedQueue[src].id = -2;
            //     reedQueue[src].lmp_clock = -1;
            }
            if(reedQueue[src].lmp_clock == -1 || msg.lmp_clock == -1){
                reedQueue[src].lmp_clock = 10000 + rand()%1000;
            }
            int allAck = 1;
            for (int i = 0; i < size; i++) {
                if (reedQueue[i].id == -1) {
                    allAck = 0;
                    break;
                }
            }

            // printf("Bee %d - reedAvail = %d, allAck = %d\n", rank, reedAvail[reedQueue[src].lmp_clock], allAck);
            // printf("Bee %d - reedQueue[src].id = %d, reedQueue[src].lmp_clock = %d\n", rank, reedQueue[src].id, reedQueue[src].lmp_clock);
            if (allAck) {
                // Sortowanie kolejki trzcin
                for (int i = 0; i < size; i++) {
                    if(reedQueueToSort[i].id == src){
                    }
                    reedQueueToSort[i].id = reedQueue[i].id;
                    reedQueueToSort[i].lmp_clock = reedQueue[i].lmp_clock;
                    // printf("%d. Trzcina Przed sortem na miejscu %d - %d z clockiem %d \n", rank, i,  reedQueue[i].id, reedQueue[i].lmp_clock);
                }

                for (int i = 0; i < T; i++){
                    // printf("Bee %d - reedAvail = %d, allAck = %d\n", rank, reedAvail[i], allAck);
                }

                qsort(reedQueueToSort, size, sizeof(Message), compare);

                // for (int i = 0; i < P; i++) {
                //     printf("%d. Trzcina Po sorcie na miejscu %d - %d z clockiem %d \n", rank, i,  reedQueueToSort[i].id, reedQueueToSort[i].lmp_clock);
                // }

                if(beesCount == size){
                    // printf("%d: Bee %d First time getting reeds\n", lmp_clock, rank);
                    for (int i = 0; i < T; i++){
                        if (reedQueueToSort[i].id == rank) {
                            for (int j = i; j < T; j++){
                                if(reedEggs[j] < 15){
                                    reed[j] = rank;
                                    printf("%d: Bee %d takes reed %d\n", *lmp_clock, rank, j);
                                    break;
                                }
                            }
                            
                            *state = WAIT_FLOWER;
                            printf("%d: Bee %d moves to state WAIT_FLOWER\n", *lmp_clock, rank);
                            (*lmp_clock)++;
                            for (int j = 0; j < size; j++) {
                                if (j != rank) {
                                    send_message(rank, REQ_FLOWER, j, *lmp_clock);
                                }
                            }
                            flowerQueue[rank].id = rank;
                            flowerQueue[rank].lmp_clock = *lmp_clock;
                            break;
                        }
                    }
                }

                for (int i = 0; i < T; i++) {
                    if(reedAvail[i] > 0){
                        // printf("%d: Bee %d Not first time getting reeds\n", lmp_clock, rank);
                        if (reedQueueToSort[0].id == rank) {
                            if(reedEggs[i] < 15){
                                reed[i] = rank;
                                printf("%d: Bee %d takes reed %d\n", *lmp_clock, rank, i);
                                *state = WAIT_FLOWER;
                                printf("%d: Bee %d moves to state WAIT_FLOWER\n", *lmp_clock, rank);
                                lmp_clock++;
                                for (int j = 0; j < size; j++) {
                                    if (j != rank) {
                                        send_message(rank, REQ_FLOWER, j, *lmp_clock);
                                    }
                                }
                                flowerQueue[rank].id = rank;
                                flowerQueue[rank].lmp_clock = *lmp_clock;
                                break;
                            }
                        } else{
                            // reedAvail[i] = 0;
                            break;
                        }
                    }
                }
            }
}




// void handle_release_reed(int src, Message *reedQueue, int *reedEggs, int *reedCount, int *beesCount) {
//     printf("Bee %d is releasing a reed.\n", src);
//     // Wyczyść dane o trzcinie przypisanej do pszczoły src
//     reedQueue[src].type = -1;
//     reedQueue[src].src = -1;
//     reedEggs[src] = 0;
//     (*reedCount)--;
//     (*beesCount)--;
// }

void handle_release_reed(int src, Message *reedQueue, int *reedEggs, int *reedCount, int *beesCount, int *reedAvail, int rank, int *lmp_clock) {
    printf("Bee %d received RELEASE_REED from Bee %d\n", rank, src);

    srand(time(NULL) + src);
    reedQueue[src].id = -3;
    reedQueue[src].lmp_clock = 10000 + rand() % 1000;

    reedAvail[reedQueue[src].lmp_clock] = 1; // Zakładam, że indeks = id trzciny (tu: lmp_clock jako id)
    (*beesCount)--;
    printf("%d bees left\n", *beesCount);

    reedEggs[reedQueue[src].lmp_clock] += 5;
    if (reedEggs[reedQueue[src].lmp_clock] == 15) {
        (*reedCount)--;
        reedAvail[reedQueue[src].lmp_clock] = -1;
    }

    (*lmp_clock)++;
    send_message(rank, ACK_DEATH, src, *lmp_clock);
}


void handle_ack_death(int rank, int src, int *lmp_clock, Message *reedQueue,  int size) {
    printf("Bee %d received ACK_DEATH from Bee %d\n", rank, src);
    send_message(rank, ACK_REED, src, *lmp_clock);
    reedQueue[src].id = -2;

    int allAck = 1;
    for (int i = 0; i < size; i++) {
        if (i != rank) {
            if (reedQueue[i].id > -2) {
                allAck = 0;
                break;
            }
        }
    }

    if (allAck) {
        printf("\n\nBee %d DIED\n\n", rank);
    }
}
