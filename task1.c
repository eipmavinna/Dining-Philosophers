/*
  Author:  Anna Hutchins, Peter Brumbach
  Course:  COMP 340, Operating Systems
  Date:    3 April 2026
  Description:   This file implements the
                 functionality required for
                 Project 2, Task 1.
  Compile with:  gcc -o task1 task1.c
  Run with:      ./task1 sample1.out
                 (or sample2.out)

*/


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "dp.h"



//function for safely retrieving the next random number from the array
int get_next_number(){
  int ret = 2;
  pthread_mutex_lock(&mutex_rand);
  if(rand_position < 500){
    ret = rand_numbers[rand_position];
    rand_position += 1;
  }
  pthread_mutex_unlock(&mutex_rand);
  return ret;
}


//waits for both chopsticks to be available, then changes state to EATING and decrements neighbors' semaphores
void pickup_chopsticks(int number){
    state[number] = HUNGRY;
    printf("                thread %d\n",number); //hungry
    
    bool eating = false;
    while(!eating){
      sem_wait(&sem_vars[number]); //one signal from either side should allow this code to progress
      
      //once the sem value is greater than 0, check if both neighbors are not eating. If one is, go back to waiting
      pthread_mutex_lock(&mutex_lock);
      if(state[(number+1)%NUMBER]!= EATING && state[(number - 1 + NUMBER) % NUMBER] != EATING){
        //if it's safe to eat, change state and decrement semaphore values on either side
        state[number] = EATING;
        sem_trywait(&sem_vars[(number+1)%NUMBER]);
        sem_trywait(&sem_vars[(number - 1 + NUMBER) % NUMBER]);
        eating = true;
      }
      pthread_mutex_unlock(&mutex_lock);
    }
  
}



//changes state, signals neighbors' semaphores
void return_chopsticks(int number){
  pthread_mutex_lock(&mutex_lock);
  state[number] = THINKING;
  sem_post(&sem_vars[(number+1)%NUMBER]); //signal the left and the right 
  sem_post(&sem_vars[(number - 1 + NUMBER) % NUMBER]);
  sem_post(&sem_vars[number]); //reset the philosopher's own semaphore
  pthread_mutex_unlock(&mutex_lock);
}


//completes philosopher actions
void *philosopher(void* param){
  int* p_num = (int*)(param); 

  //complete thinking -> hungry -> eating cycle 5 times
  for(int i = 0; i < 5; i++){
  
    printf("thread %d\n",*p_num);//thinking
    //think for a random amount of time 
    sleep(get_next_number());
    
    //change state to hungry, try to pick up the two chopsticks next to it
    pickup_chopsticks(*p_num);
    
    //in eating state
    printf("                                 thread %d\n",*p_num); //eating
    //eat for a random amount of time
    sleep(get_next_number());
    
    //finish eating, change state to thinking again
    return_chopsticks(*p_num);
    
    
  }
  
  printf("%d done\n", *p_num);

}


int main(int argc, char *argv[]){

  if(argc < 2){
    printf("Please input a random number filename\n");
    return -1;
  }
  
  char *filename = argv[1];
  FILE *f;
  f = fopen(filename, "r");
  if(f == NULL){
    printf("Could not open file %s\n", filename);
    return -1;
  }
  
  //read file contents into rand_numbers array
  char buffer[500];
  int rand_i = 0;
  while(fgets(buffer, 500, f)){
    char *token = strtok(buffer, " \t\n");
    while(token != NULL){
      rand_numbers[rand_i] = atoi(token);
      rand_i++;
      token = strtok(NULL, " \t\n");
    }
  }
  
  fclose(f); 
  
    
  rand_position = 0;
  
  pthread_mutex_init(&mutex_rand, NULL);
  pthread_mutex_init(&mutex_lock, NULL);
  
  
  printf("THINKING        HUNGRY           EATING\n");
  
  pthread_t threads[NUMBER]; // thread id
  
  for(int i = 0; i < NUMBER; i++){
    //set state to THINKING
    state[i] = THINKING;
    sem_init(&sem_vars[i], 0, 1); //each semaphore starts with two chopsticks available
    thread_id[i] = i;
    pthread_create(&threads[i], NULL, philosopher, (void*)(&thread_id[i]));
  }
  
  
  for(int i=0; i < NUMBER; i++){
   pthread_join(threads[i],NULL); //wait for threads to exit
   }
  
  
  return 0;
}




