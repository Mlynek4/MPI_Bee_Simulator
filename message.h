#ifndef MESSAGE_H
#define MESSAGE_H

typedef struct {
    int id;
    int lmp_clock;
} Message;

void send_message(int rank, int type, int dest, int lmp_clock);

#endif
