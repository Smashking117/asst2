/* 
 * stoplight.c
 *
 * 31-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: You can use any synchronization primitives available to solve
 * the stoplight problem in this file.
 */


/*
 * 
 * Includes
 *
 */

#include <types.h>
#include <lib.h>
#include <test.h>
#include <thread.h>
#include <synch.h>
#include <machine/spl.h>

//#include <synch.h>
#define northWest = "North_West";



struct lock *NW = NULL; //lock_create("North West");
struct lock *NE = NULL;// lock_create("North East");
struct lock *SW = NULL;//lock_create("South West");
struct lock *SE = NULL;//lock_create("South East");

/*Systems of arrays and indexes that will ensure that no
 * Car going the same direction as another car passes a 
 * car that arrived at the intersection before it. 
 
 *The index will represent the order and the data
 inside the array are car numbers.
 */
 unsigned long NorCars[25];
 int nc = 0;
 unsigned long EasCars[25];
 int ec = 0 ;
 unsigned long WesCars[25];
 int wc = 0;
 unsigned long SouCars[25];
 int sc = 0;


/*
 *
 * Constants
 *
 */

/*
 * Number of cars created.
 */

#define NCARS 20


/*
 *
 * Function Definitions
 *
 */


/*
 * gostraight()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement passing straight through the
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
gostraight(unsigned long cardirection,
           unsigned long carnumber)
{
	/*In order to ensure that deadlock doesn't occur, the threads have to grab
	*everything they need at the same time and then release both at the same time. 
	*/
	if(cardirection == 0) {
		kprintf("Car %lu headed straight ahead from the North\n",carnumber);
		lock_acquire(NW);
		lock_acquire(SW);
	}
	else if(cardirection == 1) {
		kprintf("Car %lu headed straight ahead from the East\n", carnumber);
		lock_acquire(NE);
		lock_acquire(NW);
	}
	else if(cardirection == 2) {
		kprintf("Car %lu headed straight ahead from the West\n", carnumber);
		lock_acquire(SW);
		lock_acquire(SE);
	}
	else if(cardirection == 3) {
		kprintf("Car %lu headed straight ahead from the South\n", carnumber);
		lock_acquire(SE);
		lock_acquire(NE);
	}	
}


/*
 * turnleft()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a left turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnleft(unsigned long cardirection,
         unsigned long carnumber)
{
	if(cardirection == 0) {
		kprintf("Car %lu headed left from the North\n", carnumber);
		lock_acquire(NW);
		lock_acquire(SW);
		lock_acquire(SE);
	}
	else if(cardirection == 1) {
		kprintf("Car %lu headed left from the East\n", carnumber);
		lock_acquire(NE);
		lock_acquire(NW);
		lock_acquire(SW);
	}
	else if(cardirection == 2) {
		kprintf("Car %lu headed left from the West\n", carnumber);
		lock_acquire(SW);
		lock_acquire(SE);
		lock_acquire(NE);
	}
	else if(cardirection == 3) {
		kprintf("Car %lu headed left from the South\n", carnumber);
		lock_acquire(SE);
		lock_acquire(NE);
		lock_acquire(NW);
	}
}


/*
 * turnright()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a right turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnright(unsigned long cardirection,
          unsigned long carnumber)
{
      if(cardirection == 0) {
		  kprintf("Car %lu headed right from the North\n", carnumber);
		  lock_acquire(NW);
	  }
	  else if(cardirection == 1) {
		  kprintf("Car %lu headed right from the East\n", carnumber);
		  lock_acquire(NE);
	  }
	  else if(cardirection == 2) {
		  kprintf("Car %lu headed right from the West\n", carnumber);
		  lock_acquire(SW);
	  }
	  else if(cardirection == 3) {
		  kprintf("Car %lu headed right from the South\n", carnumber);
		  lock_acquire(SE);
	  }
}


/*
 * approachintersection()
 *
 * Arguments: 
 *      void * unusedpointer: currently unused.
 *      unsigned long carnumber: holds car id number.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Change this function as necessary to implement your solution. These
 *      threads are created by createcars().  Each one must choose a direction
 *      randomly, approach the intersection, choose a turn randomly, and then
 *      complete that turn.  The code to choose a direction randomly is
 *      provided, the rest is left to you to implement.  Making a turn
 *      or going straight should be done by calling one of the functions
 *      above.
 */
 
static
void
approachintersection(void * unusedpointer,
                     unsigned long carnumber)
{
        int cardirection;
		int carAction;
        /*
         * Avoid unused variable and function warnings.
         */
		
        (void) unusedpointer;
        /*
         * cardirection is set randomly.
		 * 0 - North	1 - East
		 * 2 - West		3 - South
		 *carAction
		 *0 - Straight 	1 - Right 2 - Left
		 */
        cardirection = random() % 4; //0, 1, 2 ,3 
		carAction = (cardirection + carnumber) % 3 ; //0 =  , 1 , 2

		int spl;
		spl = splhigh();

		kprintf("Car %lu Entering the intersection from the %d position. Heading %d\n", carnumber, cardirection, carAction );
		
		//assigns a car an array and position based on it's direction when it reaches the intersection. This will be used to ensure two cars
		//going the same way do not pass each other.
		
		if(cardirection == 0) {
			NorCars[nc] = carnumber;
			nc++;
		}
		else if(cardirection == 1) {
			EasCars[ec] = carnumber;
			ec++;
		}
		else if(cardirection == 2) {
			WesCars[wc] = carnumber;
			wc++;
		}
		else if(cardirection == 3) {
			SouCars[sc] = carnumber;
			sc++;
		}
		
		
		splx(spl);
		
		//the lengths of the arrays after all threads are added to them
		int ncl = nc;
		int ecl = ec;
		int wcl = wc;
		int scl = sc;
		
		//for debugging
		NorCars[ncl + 1] = 50;
		SouCars[ncl + 1] = 50;
		WesCars[ncl + 1] = 50;
		EasCars[ncl + 1]  = 50;
		
		
	//a lot of code but it all pretty much does the same thing...

	/*While The thread hasn't made it out of the intersection
		if it's your turn to run within your specific array
			turn off interrupts and see if what you need is available.
				if it is grab it up, leave, set up for the next thread..
				if not turn back on interrupts and wait.
	*/
	int outOfIntersection = 0;
	while (outOfIntersection != 1) {
		
		if(cardirection == 0 && carAction == 0 && NorCars[ncl - nc] == carnumber) { 
			spl = splhigh();
			if(NW->isHeld + SW->isHeld == 0) {
				gostraight(0, carnumber);
				kprintf("Car %lu Leaving the Intersection\n", carnumber);
				splx(spl);
				lock_release(NW);
				lock_release(SW);
				nc--;
				outOfIntersection = 1;
			}
			else {
				splx(spl);
			}
			
		}
		else if(cardirection == 0 && carAction == 1 && NorCars[ncl - nc] == carnumber) {
			spl = splhigh();
			if(NW->isHeld == 0) {
				turnright(0, carnumber);
				kprintf("Car %lu Leaving the Intersection\n", carnumber);
				splx(spl);
				lock_release(NW);
				nc--;
				outOfIntersection = 1;
			}
			else {
				splx(spl);
			}
		}
		else if (cardirection == 0 && carAction == 2 && NorCars[ncl - nc] == carnumber) {
			spl = splhigh();
			if(NW->isHeld + SW->isHeld + SE->isHeld == 0) {
				turnleft(0, carnumber);
				kprintf("Car %lu Leaving the Intersection\n", carnumber);
				splx(spl);
				lock_release(NW);
				lock_release(SW);
				lock_release(SE);
				nc--;
				outOfIntersection = 1;
			}
			else {
				splx(spl);
			}
		}
		else if(cardirection == 1 && carAction == 0 && EasCars[ecl - ec] == carnumber) {
			spl = splhigh();
			if(NE->isHeld + NW->isHeld == 0) {
				gostraight(1, carnumber);
				kprintf("Car %lu Leaving the Intersection\n", carnumber);
				splx(spl);
				lock_release(NE);
				lock_release(NW);
				ec--;
				outOfIntersection = 1;
			}
			else {
				splx(spl);
			}
		}
		else if(cardirection == 1 && carAction == 1 && EasCars[ecl - ec] == carnumber) {
			spl = splhigh();
			if(NE->isHeld == 0) {
				turnright(1, carnumber);
				kprintf("Car %lu Leaving the Intersection\n", carnumber);
				splx(spl);
				lock_release(NE);
				ec--;
				outOfIntersection = 1;
			}
			else {
				splx(spl);
			}
		}
		else if (cardirection == 1 && carAction == 2 && EasCars[ecl - ec] == carnumber) {
			spl = splhigh();
			if(NW->isHeld + SW->isHeld + NE->isHeld == 0) {
				turnleft(1, carnumber);
				kprintf("Car %lu Leaving the Intersection\n", carnumber);
				splx(spl);
				lock_release(NW);
				lock_release(SW);
				lock_release(NE);
				ec--;
				outOfIntersection = 1;
			}
			else {
				splx(spl);
			}
		}
		else if(cardirection == 2 && carAction == 0 && WesCars[wcl - wc] == carnumber) {
			spl = splhigh();
			if(SW->isHeld + SE->isHeld == 0) {
				gostraight(2, carnumber);
				kprintf("Car %lu Leaving the Intersection\n", carnumber);
				splx(spl);
				lock_release(SW);
				lock_release(SE);
				wc--;
				outOfIntersection = 1;
			}
			else {
				splx(spl);
			}
		}
		else if (cardirection == 2 && carAction == 1 && WesCars[wcl - wc] == carnumber) {
		spl = splhigh();
			if(SW->isHeld == 0) {
				turnright(2, carnumber);
				kprintf("Car %lu Leaving the Intersection\n", carnumber);
				splx(spl);
				lock_release(SW);
				wc--;
				outOfIntersection = 1;
			}
			else {
				splx(spl);
			}	
		}
		else if(cardirection == 2 && carAction == 2 && WesCars[wcl - wc] == carnumber) {
			spl = splhigh();
			if(NE->isHeld + SW->isHeld + SE->isHeld == 0) {
				turnleft(2, carnumber);
				kprintf("Car %lu Leaving the Intersection\n", carnumber);
				splx(spl);
				lock_release(NE);
				lock_release(SW);
				lock_release(SE);
				wc--;
				outOfIntersection = 1;
			}
			else {
				splx(spl);
			}
		}
		else if(cardirection == 3 && carAction == 0 && SouCars[scl - sc]== carnumber) {
			spl = splhigh();
			if(SE->isHeld + NE->isHeld == 0) {
				gostraight(3, carnumber);
				kprintf("Car %lu Leaving the Intersection\n", carnumber);
				splx(spl);
				lock_release(SE);
				lock_release(NE);
				sc--;
				outOfIntersection = 1;
			}
			else {
				splx(spl);
			}
		}
		else if(cardirection == 3 && carAction == 1 && SouCars[scl - sc]== carnumber) {
			spl = splhigh();
			if(SE->isHeld == 0) {
				turnright(3, carnumber);
				kprintf("Car %lu Leaving the Intersection\n", carnumber);
				splx(spl);
				lock_release(SE);
				sc--;
				outOfIntersection = 1;
			}
			else {
				splx(spl);
			}
		}
		else if(cardirection == 3 && carAction == 2 && SouCars[scl - sc] == carnumber) {
			spl = splhigh();
			if(SE->isHeld + NE->isHeld + NW->isHeld == 0) {
				turnleft(3, carnumber);
				kprintf("Car %lu Leaving the Intersection\n", carnumber);
				splx(spl);
				lock_release(NW);
				lock_release(NE);
				lock_release(SE);
				sc--;
				outOfIntersection = 1;
			}
			else {
				splx(spl);
			}
		}
	}

	
	}


/*
 * createcars()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up the approachintersection() threads.  You are
 *      free to modiy this code as necessary for your solution.
 */

int
createcars(int nargs,
           char ** args)
{
        int index, error;

        /*
         * Avoid unused variable warnings.
         */

        (void) nargs;
        (void) args;

        /*
         * Start NCARS approachintersection() threads.
         */
		
		//Initialize the locks which represent the 4 sections of the street.
		NW = lock_create("North_West");
		NE = lock_create("North_East");
		SW = lock_create("South_West");
		SE = lock_create("South_East");

        for (index = 0; index < NCARS; index++) {
				
                error = thread_fork("approachintersection thread",
                                    NULL,
                                    index,
                                    approachintersection,
                                    NULL
                                    );

                /*
                 * panic() on error.
                 */

                if (error) {
                        
                        panic("approachintersection: thread_fork failed: %s\n",
                              strerror(error)
                              );
                }
        }

        return 0;
}
