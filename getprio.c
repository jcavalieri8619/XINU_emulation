/* getprio.c - getprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>


/*------------------------------------------------------------------------
 * getprio -- return the scheduling priority of a given process
 *------------------------------------------------------------------------
 */
SYSCALL getprio( int pid )
{
    struct pentry *pptr;
    sigset_t PS;

    disable( &PS );
    if ( isbadpid( pid ) || ( pptr = &proctab[pid] )->pstate == PRFREE ) {
        restore( &PS );
        return (SYSERR );
    }
    restore( &PS );
    return (pptr->pprio );
}
