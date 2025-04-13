#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "message.h"
#include "reed.h"
#include "flower.h"
#include "utils.h"
#include "defs.h"

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int P = size;

    if (size != P) {
        if (rank == 0) {
            printf("This program requires %d processes.\n", P);
        }
        MPI_Finalize();
        return 0;
    }

    srand(time(NULL) + rank);
    int state = WAIT_REED;
    int reed[T];
    for (int i = 0; i < T; i++) {
        reed[i] = -1; // Inicjalizacja pustych trzcin
    }
    int reedEggs[T] = {0};
    int reedCount = T;
    int flowerCount = K;
    int beesCount = P;
    int eggsLaid = 0;
    int lmp_clock = rand() % 1000;  // Zegar Lamporta dla każdej pszczółki
    int reedAvail[T] = {0};
    Message reedQueue[P];  // Kolejka trzcin
    Message flowerQueue[P];  // Kolejka kwiatów
    Message reedQueueToSort[P];  // Kolejka trzcin
    Message flowerQueueToSort[P];  // Kolejka kwiatów

    printf("%d: Bee %d started with state WAIT_REED\n", lmp_clock, rank);

    // Inicjalizacja kolejek trzcin i kwiatów
    initialize_queues(reedQueue, flowerQueue, P);  // Funkcja z pliku utils.c

    // Wysyłanie początkowego żądania REQ_REED
    lmp_clock++;
    for (int i = 0; i < size; i++) {
        if (i != rank) {
            send_message(rank, REQ_REED, i, lmp_clock);  // Funkcja z pliku message.c
        }
    }
    reedQueue[rank].id = rank;
    reedQueue[rank].lmp_clock = lmp_clock;

    while (1) {
        if (reedCount == 0) {
            printf("\n\nBee %d - THE END - No more reeds\n\n", rank);
            break;
        }

        if (beesCount == 1) {
            handle_single_bee(rank, reedEggs, reed, &eggsLaid, &state);  // Funkcja pomocnicza
            break;
        }

        int type, src, msgClock;
        MPI_Status status;
        Message msg;
        MPI_Recv(&msg, 2, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        type = status.MPI_TAG;
        src = msg.id;

        if (lmp_clock < msg.lmp_clock) {
            lmp_clock = msg.lmp_clock + 1;
        }

        if (type == REQ_REED) {
            handle_req_reed(rank, src, msg, reedQueue, &lmp_clock);  // Funkcja z pliku reed.c
        } else if (type == ACK_REED && state == WAIT_REED) {
            handle_ack_reed(rank, src, msg, reedQueue, reedQueueToSort, flowerQueue, &lmp_clock, P, beesCount, reed, reedEggs, reedAvail, &state);
        } else if (type == REQ_FLOWER) {
            handle_req_flower(rank, src, msg, flowerQueue, &lmp_clock);  // Funkcja z pliku flower.c
        } else if (type == ACK_FLOWER && state == WAIT_FLOWER) {
            handle_ack_flower(rank, src, msg, flowerQueue, flowerQueueToSort, &lmp_clock, size, reed, reedEggs, &eggsLaid, &state);
        } else if (type == RELEASE_FLOWER) {
            handle_release_flower(src, flowerQueue);  // Funkcja z pliku flower.c
        } else if (type == RELEASE_REED) {
            // handle_release_reed(src, reedQueue, reedEggs, &reedCount, &beesCount);  // Funkcja z pliku reed.c
            handle_release_reed(src, reedQueue, reedEggs, &reedCount, &beesCount, reedAvail, rank, &lmp_clock);
        } else if (type == ACK_DEATH) {
            // handle_release_reed(src, reedQueue, reedEggs, &reedCount, &beesCount);  // Funkcja z pliku reed.c
            handle_ack_death(rank, src, &lmp_clock, reedQueue, size);
        }
    }

    for (int i = 0; i < T; i++){
        printf("Reed %d has %d eggs\n", i, reedEggs[i]);
        if (reedEggs[i] == 15){
            reedCount--;
        }
    }

    MPI_Finalize();
    return 0;
}
