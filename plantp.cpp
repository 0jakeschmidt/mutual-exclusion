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
idea here is to have client, help desk, and tech all as a different threads.

****************************Client process **************************
This process will work for random amounts of time until something breaks, then contact the help desk
one process 

****************************Help Desk*********************************
one process that waits on a call from the client and only when it recieves the call from the client does it wake up
this process will signal that there is a job for the techs to go work on


***************************Techs*************************************
there will be 5 tech process total
processes will all be having coffee for random amounts of time.
when a call comes in they can only go on the job when at least three of the processes are finished with their coffee 
on the job they work for a random time and signal to the client that the job is complete 
after job they go back to all drinking coffee for random amount of time.


*/
bool flag = false;
sem_t Mutex;
sem_t client,tech,jobs,help;
sem_t threeReady;
sem_t working;
vector<int> ready(0);


int id =0;


void readyToFixStuff(int i){
    int t;
   // lock for size check 
   sem_wait(&Mutex);
  
   // we need to check if 2or more techs are ready has food:
   if (ready.size() >= 2 && flag){
    // ready queue is set and techs can go to work
      flag =false;
      sem_post(&threeReady);
      sem_post(&threeReady);
      int tech1 = ready.at(0);
      ready.pop_back();
      int tech2 = ready.at(0);
      ready.pop_back();
      printf("techs %d , %d, %d: working on problem \n",i,tech2,tech1);
      sem_post(&Mutex);
      t = (int) rand() % 20; 
      printf("techs working for %d seconds\n",t);
      sleep(t);

      //release workers
      sem_post(&working);
      sem_post(&working);
      sem_post(&client);

    }else{
      // add yourself to the queue and wait for the signal to be triggered
      ready.push_back(i);
      sem_post(&Mutex);
      sem_wait(&threeReady);
      //released from queue now go to work
      sem_wait(&working);

    }
      
      printf("Techs fixed client can work \n");
      

   
}

void * clientProcess(void * noparam) {
   int t;

   printf("client is starting \n");

  while (1==1)  {

  	//client is going to work
    //work for random amount of time
    t = (int) rand() % 20; 
    printf("Client is starting to work for %d: seconds \n",t);
    sleep(t);
    // something will now break for the client to signal helpdesk

    printf("Something broke, client will call help desk \n");

    sem_wait(&Mutex);
    flag = true;
    sem_post(&Mutex);

    sem_post(&help);
    sem_wait(&client);

    printf("Fixed!\n");


  }
}

void * helpdesk(void * noparam){
	
	while(1==1){
	sem_wait(&help);
	printf("Help Desk has been called your problem will be fixed shortly \n");
	
  //signal techs
  sem_post(&tech);

  //go back to waiting for another call
  
  	
  }

}

void * techProcess(void * noparam){
  //get process an id number
    int techNum,t;

  //geting an id number
    sem_wait(&Mutex);
    techNum = id;
    id++;
    sem_post(&Mutex);

    printf("tech number: %d has been created \n",techNum);

    int* iD = &techNum;
  while(1==1){
  // drink coffee for random amount of time 
   t = (int) rand() % 20; 
   printf("tech %d: is drinking coffee for %d seconds \n",techNum,t);
   sleep(t);
   // done with coffee queue up and wait for a call 

   //go fix stuff pass ID
  readyToFixStuff(*iD);

    
  }

}



int main(int argc, char* argv[]) 
{ 
 	int randNum;
 // seed time 
	srand((unsigned)time(0));


     // initialize all semaphores
     sem_init(&Mutex,0,1);
     sem_init(&client,0,0);
     sem_init(&tech,0,0);
     sem_init(&jobs,0,0);
     sem_init(&threeReady,0,0);
     sem_init(&help,0,0);
     sem_init(&working,0,0);

 	pthread_t techs[5];
 	pthread_t helpDesk;
  pthread_t clientThread;

// create client desk
  if (pthread_create(&clientThread, 0, clientProcess, 0)) {
    perror("error creating the helpDesk thread thread");
    exit(1);
  }
// create help desk
  if (pthread_create(&helpDesk, 0, helpdesk, 0)) {
    perror("error creating the helpDesk thread thread");
    exit(1);
  }
// creat the techs 
 for (int i =0; i<5; i++) {
    
     if (pthread_create(&techs[i], NULL, techProcess, 0)) {
       perror("error creating the tech thread");
       exit(1);
     }
  }
//announce all threads created
  printf ("all threads created\n");

  for(int i=0;i<10;i++) {
    pthread_join(techs[i], 0);

  }
   pthread_join(helpDesk, 0);
   pthread_join(clientThread, 0);

} 

/*
wait_{
  while (sem =0)
  sem--;
}
post{
  
  sem++;
}
*/
