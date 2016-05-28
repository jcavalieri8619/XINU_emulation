/* wait.c - wait */


#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>


/*------------------------------------------------------------------------
 * wait  --  make current process wait on a semaphore
 *------------------------------------------------------------------------
 */
SYSCALL wait( int sem )
{
    char ps;
    register struct sentry *sptr;
    register struct pentry *pptr;
    sigset_t PS;

    disable( &PS );
    if ( isbadsem( sem ) || ( sptr = &semaph[sem] )->sstate == SFREE ) {
        restore( &PS );

        return (SYSERR );
    }
    if ( --( sptr->semcnt ) < 0 ) {
        ( pptr = &proctab[currpid] )->pstate = PRWAIT;
        pptr->psem = sem;
        enqueue( currpid, sptr->sqtail );
        resched( );
    }
    restore( &PS );
    return (OK );
}
