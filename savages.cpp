#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <mutex>
#include <vector>

using namespace std;


/*
idea here is to have 10 total savagae threads and one cook thread, 

*/
//full is set to the size of pot

sem_t Mutex;
sem_t empty;
sem_t full;
vector<int> POT(0);


int id =0, sizePot=100;

 
void getFood(int i){

   // lock for size check
   sem_wait(&Mutex);
  
   // we need to check if pot has food:
   if (POT.size() == 0 ){
   	// pot has no food, wake cook
   		
   		sem_post(&empty); //wakes the cook and fills pot
   		sem_wait(&full);
   	}
   		
   		//pot has some food so we want to eat it 
   		int max = POT.size();
   		int min =1;
   	 	int randFood =  rand()%(max+1 - min)+min;
   	 	
   		// use loop to shrink vector to new size
   		printf("savage: %d  is eating %d helpings \n",i,randFood);

   		for(int i=0; i< randFood; i++){
   			POT.pop_back();
   		}
   		
   		printf("there is %d helpings left \n",POT.size());
   		sem_post(&Mutex);

   
}




void * sav(void * noparam) {
  int savNum,t;

  //geting an id number
 	sem_wait(&Mutex);
    savNum = id;
    id++;
    sem_post(&Mutex);
  
  
   int* iD = &savNum;

   printf("savage %d:  Starting \n",savNum);
   sleep(4);
   

  while (1==1)  {

 
  	//savage is hungry
    printf("Savage %d: is hungry \n",savNum);
   
   	//savage goes to get some food
    getFood(*iD);

    t = (int) rand() % 20; 
    //got some food so we will now eat for rand time 
   // printf("Savage %d: is eating for %d \n",savNum,t);
    sleep(t);
  

  }
}

void * cook(void * noparam){
	
	while(1==1){
	sem_wait(&empty);
	printf("SOMEONE WOKE THE COOK \n\n");
	
	 for(int i=0; i<sizePot; i++){
 		
 		POT.push_back(i);
      }
  	sem_post(&full);
  	
  }

}

int main(int argc, char* argv[]) 
{ 
 	int randNum;
 // seed time 
	srand((unsigned)time(0));

	if (argc != 0) {
	    sizePot = atoi( argv[1] );
	  } 


     // initialize all semaphores
     sem_init(&Mutex,0,1);
     sem_init(&empty,0,0);
     sem_init(&full,0,0);

 	pthread_t savages[10];
 	pthread_t chefThread;

// create chef
  if (pthread_create(&chefThread, 0, cook, 0)) {
    perror("error creating the chef thread");
    exit(1);
  }

 for (int i =0; i<10; i++) {
    
     if (pthread_create(&savages[i], 0, sav, 0)) {
       perror("error creating the savages thread");
       exit(1);
     }
  }
//test for change
  printf ("all threads created\n");

  for(int i=0;i<10;i++) {
    pthread_join(savages[i], 0);

  }
   pthread_join(chefThread, 0);

} 