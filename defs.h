#ifndef DEFS_H
#define DEFS_H

// Dane wejsciowe
#define T 3  // Liczba trzcin
#define K 1  // Liczba kwiatków

// Definicje stanów
#define WAIT_REED 0
#define WAIT_FLOWER 1
#define SUCC_FLOWER 2
#define LAY_EGG 3

// Definicje wiadomości
#define REQ_REED 0
#define ACK_REED 1
#define RELEASE_REED 2
#define REQ_FLOWER 3
#define ACK_FLOWER 4
#define RELEASE_FLOWER 5
#define ACK_DEATH 6

#endif
