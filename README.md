# Program #3

Name: Jacob Schmidt

How to compile all four programs:
  1. $ g++ diningPhil.cpp -o dining -lpthread -std=c++11 
     $ ./dining
     
  2. $ g++ savages.cpp -o sav -lpthread -std=c++11
     $ ./sav

  3. $ g++ semaphore_mutex_unisex.cpp -o uni -lpthread -std=c++11
     $ ./uni

  4. $ g++ plantp.cpp -o plant -lpthread -std=c++11
     $ ./plant

Anything not working:
 It all works well to the best of my knowledge and testing.
 I will say that with the dining Philosophers problem I found that I 
 didn't use the solution from the slides but it still functions properly
 and avoids starvation and deadlock.

---

grading: /50<BR>
problem1: 14/15<BR>
  * compiles, works yes
  * issues
    * pthreads no the newer threads as exampled in class. -1

problem2: 13/15<BR>
  * compiles, works compiles yes.  runs no.  segmetation fault on start.  no output.
  * issues
    * readme didn't take to send a parameter. wihtout it program dies. -1
    * you can take more servers then are in the pot.  should just take a serving. -1

problem3: 5/5<BR>
  * added correctly? yes
  * still compiles and runs? yes

problem4: 14/15<BR>
  * compiles, works yes
  * issues
    * overly complex code.
    * pthreads no the newer threads as exampled in class. -1
