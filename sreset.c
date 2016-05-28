/* sreset.c - sreset */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>


/*------------------------------------------------------------------------
 *  sreset  --  reset the count and queue of a semaphore
 *------------------------------------------------------------------------
 */
SYSCALL sreset( int sem, int count )
{
    struct sentry *sptr;
    int pid;
    int slist;
    sigset_t PS;
    disable( &PS );
    if ( isbadsem( sem ) || count < 0 || semaph[sem].sstate == SFREE ) {
        restore( &PS );
        handle_error( "semreset: " );
        return (SYSERR );
    }
    sptr = &semaph[sem];
    slist = sptr->sqhead;
    while ( ( pid = getfirst( slist ) ) != EMPTY )
        ready( pid, RESCHNO );
    sptr->semcnt = count;
    resched( );
    restore( &PS );
    return (OK );
}
