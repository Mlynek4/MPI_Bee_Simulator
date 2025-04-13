#include <stdio.h>
#include "utils.h"
#include "defs.h"

void initialize_queues(Message reedQueue[], Message flowerQueue[], int size) {
    for (int i = 0; i < size; i++) {
        reedQueue[i].id = -1;
        reedQueue[i].lmp_clock = -1;
        flowerQueue[i].id = -1;
        flowerQueue[i].lmp_clock = -1;
    }
}

int compare(const void *a, const void *b) {
    Message *msgA = (Message *)a;
    Message *msgB = (Message *)b;
    if (msgA->lmp_clock == msgB->lmp_clock) {
        return msgA->id - msgB->id;
    }
    return msgA->lmp_clock - msgB->lmp_clock;
}



// void handle_single_bee(int rank, int *reedEggs, int reed, int *eggsLaid, int *state) {
//     printf("Bee %d is laying an egg in reed %d.\n", rank, reed);
//     reedEggs[reed]++;
//     (*eggsLaid)++;
//     *state = 1;  // np. 1 = zakończyła składanie jajka
// }

void handle_single_bee(int rank, int *reedEggs, int *reed, int *eggsLaid, int *state) {
    if (*state == WAIT_REED) {
        for (int i = 0; i < T; i++) {
            if (reedEggs[i] < 15) {
                reed[i] = rank;
                printf("%d: Bee %d takes reed %d\n", 0, rank, i); // Zakładam, że lmp_clock tu niepotrzebny
                break;
            }
        }
    }

    int eggsLeft = 5 - *eggsLaid;
    for (int i = 0; i < eggsLeft; i++) {
        *state = WAIT_FLOWER;
        printf("Bee %d moves to state WAIT_FLOWER\n", rank);

        *state = SUCC_FLOWER;
        printf("Bee %d moves to state SUCC_FLOWER\n", rank);

        *state = LAY_EGG;
        printf("Bee %d moves to state LAY_EGG\n", rank);

        (*eggsLaid)++;
        printf("Bee %d laid egg %d\n", rank, *eggsLaid);
    }

    if (eggsLeft == 0 || *eggsLaid == 5) {
        printf("\n\nBee %d DIED\n\n", rank);
        for (int i = 0; i < T; i++) {
            if (reed[i] == rank) {
                reedEggs[i] += 5;
                break;
            }
        }
        printf("\n\nTHE END - No more bees\n\n");
    } else {
        *state = WAIT_FLOWER;
        printf("Bee %d moves to state WAIT_FLOWER\n", rank);
    }
}