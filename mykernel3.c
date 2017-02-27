/* mykernel.c: your portion of the kernel
 *
 *	Below are procedures that are called by other parts of the kernel. 
 *	Your ability to modify the kernel is via these procedures.  You may
 *	modify the bodies of these procedures any way you wish (however,
 *	you cannot change the interfaces).  
 */

#include "aux.h"
#include "sys.h"
#include "mykernel3.h"

#define FALSE 0
#define TRUE 1

/*	A sample semaphore table. You may change this any way you wish.  
 */

static struct {
	int valid;			// Is this a valid entry (was sem allocated)?
	int value;			// value of semaphore
	int procs[MAXPROCS];		// each semaphore has an array of blocked processes
	int last_unblocked_index; 	// each sem keeps track of which process was last unblocked
} semtab[MAXSEMS];



/*	InitSem () is called when kernel starts up.  Initialize data
 *	structures (such as the semaphore table) and call any initialization
 *	procedures here. 
 */

void InitSem ()
{
	int s;

	/* modify or add code any way you wish */

	for (s = 0; s < MAXSEMS; s++) {		// mark all sems free
		semtab[s].valid = FALSE;
		semtab[s].last_unblocked_index = -1;
		for (int i = 0; i < MAXPROCS; i++) {
			semtab[s].procs[i] = -1;
		}

	}

	
}

/*	MySeminit (p, v) is called by the kernel whenever the system
 *	call Seminit (v) is called.  The kernel passes the initial
 * 	value v, along with the process ID p of the process that called
 *	Seminit.  MySeminit should allocate a semaphore (find a free entry
 *	in semtab and allocate), initialize that semaphore's value to v,
 *	and then return the ID (i.e., index of the allocated entry). 
 */

int MySeminit (int p, int v)
{
	int s;

	/* modify or add code any way you wish */

	for (s = 0; s < MAXSEMS; s++) {
		if (semtab[s].valid == FALSE) {
			break;
		}
	}
	if (s == MAXSEMS) {
		Printf ("No free semaphores\n");
		return (-1);
	}

	semtab[s].valid = TRUE;
	semtab[s].value = v;



	return (s);
}

/*	MyWait (p, s) is called by the kernel whenever the system call
 *	Wait (s) is called.  
 */

void MyWait (p, s)
	int p;				// process
	int s;				// semaphore
{
	/* modify or add code any way you wish */
	int tmp;
	tmp = semtab[s].value;
	semtab[s].value--;
	if (semtab[s].value < 0) {
		//add calling process to list of blocked process
		//get the list of blocked process for the specific semaphore

		//finds the first available spot
		for (int i = 0; i < MAXPROCS; i++) {
			if (semtab[s].procs[i] == -1) {
				semtab[s].procs[i] = p;
				break;
			}
		}

		Block (p);
			
	}
}

/*	MySignal (p, s) is called by the kernel whenever the system call
 *	Signal (s) is called.  
 */

void MySignal (p, s)
	int p;				// process
	int s;				// semaphore
{
	/* modify or add code any way you wish */
	semtab[s].value++;
	int tmp;

	int unblocked = 0;
	for (int i = semtab[s].last_unblocked_index+1; i < MAXPROCS; i++) {
		if (semtab[s].procs[i] != -1) {
			tmp = semtab[s].procs[i];
			semtab[s].procs[i] = -1;	
			semtab[s].last_unblocked_index = i;
		
			unblocked = 1;

			Unblock (tmp);
			break;
		}
	}
	if (unblocked == 0) {
		for (int i = 0; i < MAXPROCS; i++) {
			if (semtab[s].procs[i] != -1) {
				tmp = semtab[s].procs[i];
				semtab[s].procs[i] = -1;	
		
				semtab[s].last_unblocked_index = i;
		
				Unblock (tmp);
				break;
			}
		}
	}
}





