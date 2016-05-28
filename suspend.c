/* suspend.c - suspend */


#include <conf.h>
#include <kernel.h>
#include <proc.h>


/*------------------------------------------------------------------------
 *  suspend  --  suspend a process, placing it in hibernation
 *------------------------------------------------------------------------
 */
SYSCALL suspend( int pid ) /* id of process to suspend     */
{
    struct pentry *pptr; /* pointer to proc. tab. entry	*/

    int prio; /* priority returned		*/
    sigset_t PS;
    disable( &PS );
    if ( isbadpid( pid ) || pid == NULLPROC ||
         ( ( pptr = &proctab[pid] )->pstate != PRCURR && pptr->pstate != PRREADY ) ) {
        restore( &PS );
        return (SYSERR );
    }
    if ( pptr->pstate == PRREADY ) {
        dequeue( pid );
        pptr->pstate = PRSUSP;
    } else {
        pptr->pstate = PRSUSP;
        resched( );
    }
    prio = pptr->pprio;
    restore( &PS );
    return (prio );
}
