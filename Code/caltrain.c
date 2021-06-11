#include <pthread.h>
#include "caltrain.h"


void station_init(struct station *station)
{
    //set the varibles
    station->waiting_passengers=0,station->onboard_passengers=0, station->empty_seats=0;

    //initialize the mutex and the condition varibles
    pthread_mutex_init(&station->lock,NULL);
    pthread_cond_init(&station->on_board,NULL);
    pthread_cond_init(&station->wating_for_train,NULL);
}

void station_load_train(struct station *station, int count)
{
    // start the critical section
    pthread_mutex_lock(&station->lock);
        // let the empty seates of the station equal the empty seats of the train
        station->empty_seats=count;
        // signal to all passengers that a train has been arrived
        pthread_cond_broadcast(&station->wating_for_train);

        // Not return untill all passengers in their seats or there is not any waiting passengers or the train came full.
        while ( (station->empty_seats > 0 && station->waiting_passengers > 0) || station->onboard_passengers>0){
            pthread_cond_wait (&station->on_board, &station->lock);
        }

        station->empty_seats=0;

    // end the critical section
    pthread_mutex_unlock (&station->lock);
}

void station_wait_for_train(struct station *station)
{
    pthread_mutex_lock (&station->lock);
        //a new passenger has arrived at the station
        station->waiting_passengers++;
        // wait untill there is an empty seat
        while(station->empty_seats == 0){
            pthread_cond_wait (&station->wating_for_train, &station->lock);
        }
        station->empty_seats--;
        station->waiting_passengers--;
        station->onboard_passengers++;
        //pthread_cond_signal(&station->on_board);
    pthread_mutex_unlock (&station->lock);
}

void station_on_board(struct station *station)
{
    pthread_mutex_lock (&station->lock);
        // if all new passengers in the train are seated then sinal the train t leave
        if(--station->onboard_passengers == 0)
            pthread_cond_signal(&station->on_board);
    pthread_mutex_unlock (&station->lock);

}
