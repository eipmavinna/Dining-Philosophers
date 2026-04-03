#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "dp.h"

//function for the philosopher to pickup the chopsticks
void pickup_chopsticks(int number){
    //lock
    //check to see if semaphore value > 0. if not, release lock and loop again. if so, ea
    //wait for philosopher semaphore. when get it,  (could be -2, could be 0)
    //grab lock
    //check left and right philosopher to see if they're eating, if not, grab and eat
    //release lock
    
    
    //Q: does it matter if I'm changing between hungry and thinking while reading this? do I       need to lock those too
    
  //loop until can eat:
  
    //lock (so they don't change to eating while I'm picking up)
    //check left and right; if neither is eating, change status to eating and release lock.
    //if one is eating, release lock and loop again
    
    //Q: why do I need a semaphore? 
    //So I don't take up time slice
    
    //bool eating = false;
    //while(!eating){

      //wait for both chopsticks to be set down
      sem_wait(&sem_vars[number]);
      sem_wait(&sem_vars[number]);
      pthread_mutex_lock(&mutex_lock);
      
      //TODO: don't need this if statement if it works correctly
      if(state[(number+1)%NUMBER]!= EATING && state[(number - 1 + NUMBER) % NUMBER] != EATING){
        printf("Thread %d eating\n", number);
        state[number] = EATING;
        
        
        //eating = true;
      }else{
        printf("said that one of the neighbors was eating, error\n");
      }

      pthread_mutex_unlock(&mutex_lock);
    //} 
  
}



//function for the philosopher to return the chopsticks
void return_chopsticks(int number){

}


//completes philosopher actions
void *philosopher(void* param){

  int* p_num = (int*)(param); 


  for(int i = 0; i < 5; i++){  //complete cycle 5 times
  
    //begin in the thinking state
      //think for a random amount of time (bounded by what? don't want infinite time)
    
    
    //TODO: Change
    int lower = 1, upper = 3;
    int num = (rand() % (upper - lower + 1)) + lower;
    sleep(num);
    
    //change state to hungry, try to pick up the two chopsticks next to it
    state[p_num] = HUNGRY;
    
    //try to eat
    pickup_chopsticks(*p_num);
    
    //now eating
    
    //check if they're eating or not?
      //check (p+1)%N, and (p-1)%N at the same time, wait until they're free. How use locks for         this?
    
    //when successful, change state to eating, eat for a random amount of time, then release.
    
    //end this loop, go back to thinking at the top.
    state[p_num] = THINKING;
    
    //tell the philosophers on either side that you've put down the chopsticks
    signal((number - 1 + NUMBER) % NUMBER)
    sem_post(&sem_vars[number - 1 + NUMBER) % NUMBER]); //left one
    sem_post(&sem_vars[number+1) % NUMBER]); //right phil one
    
    
  }
  
  
  printf("hallo\n");

}


int main(int argc, char *argv[]){

  pthread_mutex_init(&mutex_lock, NULL);
  
  //initialize all thread states as thinking?
  for(int i = 0; i < NUMBER; i++){
    //set state to THINKING
    state[i] = THINKING;
    sem_init(&sem_vars[i], 0, 2); //each semaphore starts with two chopsticks next to them
    thread_id[i] = i;
    //TODO: create thread with philosopher number == i
  }
  
  //TODO: join threads to wait for them
  
  philosopher(&thread_id[0]);
  
  
  return 0;
}