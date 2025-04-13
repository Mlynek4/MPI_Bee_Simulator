#include <mpi.h>
#include "message.h"

void send_message(int rank, int type, int dest, int lmp_clock) {
    Message msg;
    msg.id = rank;
    msg.lmp_clock = lmp_clock;
    MPI_Send(&msg, 2, MPI_INT, dest, type, MPI_COMM_WORLD);
}


