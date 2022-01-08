#ifndef MTENGINE_H
    #include "multithreadengine.h"
#endif



void* interactForSingleThread(void* interact_arguments){
    InteractArguments* ia = ((InteractArguments*)interact_arguments);
    (ia->b)->interact(*(ia->E), *(ia->B), ia->time, ia->time_interval);
    pthread_exit(NULL);
}