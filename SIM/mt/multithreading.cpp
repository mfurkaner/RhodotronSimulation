#include "multithreadengine.h"


extern mutex mutex_lock;

void interactForSingleThread(InteractArguments interact_arguments){
    //(ia->b)->interact(*(ia->E), *(ia->B), ia->time, ia->time_interval);
    interact_arguments.b->interact(*interact_arguments.E, *interact_arguments.B, interact_arguments.time_interval);
    pthread_exit(NULL);
}