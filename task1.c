#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "dp.h"

//function for the philosopher to pickup the chopsticks
void pickup_chopsticks(int number){
    state[number] = HUNGRY;
    printf("                thread %d\n",*p_num); //hungry
    bool eating = false;
    while(!eating){
      //printf("thread %d waiting\n", number);
      sem_wait(&sem_vars[number]); //one signal from either side should allow this code to progress
      //still need to check
      
      //once the sem value is greater than 0
      pthread_mutex_lock(&mutex_lock);
      if(state[(number+1)%NUMBER]!= EATING && state[(number - 1 + NUMBER) % NUMBER] != EATING){
        state[number] = EATING;
        sem_trywait(&sem_vars[(number+1)%NUMBER]); //decrement values on either side so they know to wait
        sem_trywait(&sem_vars[(number - 1 + NUMBER) % NUMBER]);
        eating = true;
      }else{
        //printf("said that one of the neighbors was eating, error\n");
        
        //just loop again, wait for someone to signal and check both again
      }

      pthread_mutex_unlock(&mutex_lock);
    }
  
}



//function for the philosopher to return the chopsticks
void return_chopsticks(int number){
  pthread_mutex_lock(&mutex_lock);
  state[number] = THINKING;
  sem_post(&sem_vars[(number+1)%NUMBER]); //signal the left and the right 
  sem_post(&sem_vars[(number - 1 + NUMBER) % NUMBER]);
  sem_post(&sem_vars[number]);
  pthread_mutex_unlock(&mutex_lock);
  //signal both
}


//completes philosopher actions
void *philosopher(void* param){

  int* p_num = (int*)(param); 


  for(int i = 0; i < 5; i++){  //complete cycle 5 times
  
    //begin in the thinking state
    printf("thread %d\n",*p_num);
    
    //TODO: think for a random amount of time 
    sleep(2);
    
    //change state to hungry, try to pick up the two chopsticks next to it
    pickup_chopsticks(*p_num);
    
    //now eating
    printf("                                 thread %d\n",*p_num); //eating
    
    
    //TODO: eat for a random amount of time
    sleep(2);
    
    //finish eating
    return_chopsticks(*p_num);
    
    
    //when successful, change state to eating, eat for a random amount of time, then release.
    
    //end this loop, go back to thinking at the top.

    
    
  }
  
  
  printf("hallo\n");

}


int main(int argc, char *argv[]){

  pthread_mutex_init(&mutex_lock, NULL);
  
  printf("THINKING        HUNGRY           EATING\n");
  
  pthread_t threads[NUMBER]; // thread id 
  
  //initialize all thread states as thinking?
  for(int i = 0; i < NUMBER; i++){
    //set state to THINKING
    state[i] = THINKING;
    sem_init(&sem_vars[i], 0, 1); //each semaphore starts with two chopsticks next to them
    thread_id[i] = i;
    pthread_create(&threads[i], NULL, philosopher, (void*)(&thread_id[i]));
  }
  

  
  //philosopher(&thread_id[0]);
  
  //TODO: join threads to wait for them
  for(int i=0; i < NUMBER; i++){
   pthread_join(threads[i],NULL); //wait for threads to exit
   }
  
  
  return 0;
}