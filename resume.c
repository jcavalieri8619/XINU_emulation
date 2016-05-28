/* resume.c - resume */

#include <conf.h>
#include <kernel.h>
#include <proc.h>


/*------------------------------------------------------------------------
 * resume  --  unsuspend a process, making it ready; return the priority
 *------------------------------------------------------------------------
 */
SYSCALL resume( int pid )
{

    struct pentry *pptr; /* pointer to proc. tab. entry	*/
    int prio; /* priority to return		*/

    char str[100];
    int len;

    sigset_t PS;


    disable( &PS );
    if ( isbadpid( pid ) || ( pptr = &proctab[pid] )->pstate != PRSUSP ) {
        restore( &PS );
        return (SYSERR );
    }
    prio = pptr->pprio;
    ready( pid, RESCHYES );
    restore( &PS );
    return (prio );
}
