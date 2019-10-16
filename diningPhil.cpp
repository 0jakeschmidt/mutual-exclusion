#include <pthread.h> 
#include <semaphore.h> 
#include <stdio.h> 
#include <unistd.h>

#include <mutex>
using namespace std;
// we have 5 philosiphers and 5 forks
// each need two forks next to them in order to eat.
// each philosipher is a thread
// each fork remains in a shared global vector
//***********************Philosopher*************************
// its a thread that can have a few states, Thinking, and eating 
// needs his left and right fork to eat
// 
//************************forks*******************************
//forks are a vector of items 
// each philosopher needs 2 forks == fork[philID] "right fork" && "left fork" fork[(philID +1 )% 5 ]
// if you dont have both then you wait,
 

//semaphores and one mutex

sem_t forks[5];
mutex multex;
int id =0;

void put_forks(int i){

   int left = (i+1)%5;
   int right =i;
   sem_post(&forks[left]);
   sem_post(&forks[right]);
       

}
 
void takeForks(int i){

   int left = (i+1)%5;
   int right =i;

   multex.lock();
   sem_wait(&forks[left]);
   sem_wait(&forks[right]);
   multex.unlock();

   printf("philosipher %d: is eating with forks:  %d and %d \n",i,left,right);

}



void * phils(void * noparam) {
  int philNum,t,e;

  //geting an id number
  multex.lock();
    philNum = id;
    id++;
  multex.unlock();
  
   int* iD = &philNum;

   printf("philospher %d:  Starting \n",philNum);
   int leftFork = (philNum+1)%5;
   int rightFork = philNum;

  while (1==1)  {

   //work for a while
    t = (int) rand() % 5; 
    printf("philospher %d:  is thinking %d seconds\n",philNum,t);
    sleep (t);

    printf("philosipher %d: needs to eat \n",philNum);
   
    takeForks(*iD);
    e = (int) rand() % 5;
    printf("philosipher %d: is eating for %d\n",philNum,e);
    sleep(e);
    put_forks(*iD);
  
    printf("philosipher %d: put the forks down \n",philNum);

  }
}

int main() 
{ 
 // seed time 
 srand((unsigned)time(0));

 // initialize all semaphores
 for(int i=0; i<5; i++){
     sem_init(&forks[i],0,1);
  }


 pthread_t philosophers[5];

 for (int i =0; i<5; i++) {
    
     if (pthread_create(&philosophers[i], 0, phils, 0)) {
       perror("error creating the thread");
       exit(1);
     }
  }

  printf ("all threads created\n");

  for(int i=0;i<5;i++) {
    pthread_join(philosophers[i], 0);

  }

} 
