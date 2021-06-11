#include <pthread.h>

struct station {
    int waiting_passengers,empty_seats,onboard_passengers;

    pthread_mutex_t lock ;
    pthread_cond_t wating_for_train;
    pthread_cond_t on_board;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
