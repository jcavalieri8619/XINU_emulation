/* signal.c - signal */


#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>


/*------------------------------------------------------------------------
 * signal  --  signal a semaphore, releasing one waiting process
 *------------------------------------------------------------------------
 */
SYSCALL signal( register int sem )
{
    register struct sentry *sptr;

    sigset_t PS;
    disable( &PS );
    if ( isbadsem( sem ) || ( sptr = &semaph[sem] )->sstate == SFREE ) {
        restore( &PS );
        handle_error( "semsignal: " );
        return (SYSERR );
    }
    if ( ( sptr->semcnt++ ) < 0 )
        ready( getfirst( sptr->sqhead ), RESCHYES );
    restore( &PS );
    return (OK );
}
