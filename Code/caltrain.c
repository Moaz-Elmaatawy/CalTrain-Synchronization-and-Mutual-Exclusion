#include <pthread.h>
#include "caltrain.h"



void station_init(struct station *station)
{
    station->waiting_passengers=0,station->empty_seats=0,station->onboard_passengers=0;

    station->lock = PTHREAD_MUTEX_INITIALIZER;
    station->wating_for_train =PTHREAD_COND_INITIALIZER;
    station->on_board =PTHREAD_COND_INITIALIZER;
}

void
station_load_train(struct station *station, int count)
{
    pthread_mutex_lock(&station->lock);
        station->empty_seats=count;
        pthread_cond_broadcast(station->wating_for_train);

        while (count != 0 && station->waiting_passengers != 0 && station->on_board!=count){
            pthread_cond_wait (&station->on_board, &station.lock);
        }

    pthread_mutex_unlock (&station->lock);
}

void
station_wait_for_train(struct station *station)
{
    pthread_mutex_lock (&station->lock);
        station->waiting_passengers++;
        while(station->empty_seats > 0){
            pthread_cond_wait (&station->wating_for_train, &station->lock);
        }
        station->empty_seats--;
        pthread_cond_signal(&station->on_board,&station->lock);
    pthread_mutex_unlock (&station->lock);
}

void
station_on_board(struct station *station)
{
    pthread_mutex_lock (&station->lock);
        station->waiting_passengers--;
        station->onboard_passengers++;
        if(station->empty_seats==0 ||station->waiting_passengers==0)
            pthread_cond_signal(&station->on_board,&station->lock);
    pthread_mutex_unlock (&station->lock);

	// FILL ME IN
}
