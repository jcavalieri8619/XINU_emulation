/* chprio.c - chprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>


/*------------------------------------------------------------------------
 * chprio  --  change the scheduling priority of a process
 *------------------------------------------------------------------------
 */


SYSCALL chprio( int pid, int newprio )
{
    int oldprio;
    struct pentry *pptr;

    sigset_t PS;
    disable( &PS );
    if ( isbadpid( pid ) || newprio <= 0 ||
         ( pptr = &proctab[pid] )->pstate == PRFREE ) {
        restore( &PS );

        return (SYSERR );
    }
    oldprio = pptr->pprio;
    pptr->pprio = newprio;

    switch ( pptr->pstate ) {
        case PRCURR:
            if ( newprio < oldprio )
                resched( );
            break;
        case PRREADY:
            dequeue( pid );
            insert( pid, rdyhead, newprio );
            if ( proctab[currpid].pprio < newprio )
                resched( );
            break;
    }



    restore( &PS );
    return (oldprio );
}
