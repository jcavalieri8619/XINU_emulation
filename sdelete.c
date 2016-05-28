/* sdelete.c - sdelete */


#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>


/*------------------------------------------------------------------------
 * sdelete  --  delete a semaphore by releasing its table entry
 *------------------------------------------------------------------------
 */
SYSCALL sdelete( int sem )
{
    char ps;
    int pid;
    struct sentry *sptr; /* address of sem to free	*/
    sigset_t PS;
    disable( &PS );
    if ( isbadsem( sem ) || semaph[sem].sstate == SFREE ) {
        restore( &PS );
        return (SYSERR );
    }
    sptr = &semaph[sem];
    sptr->sstate = SFREE;
    if ( nonempty( sptr->sqhead ) ) { /* free waiting processes	*/
        while ( ( pid = getfirst( sptr->sqhead ) ) != EMPTY )
            ready( pid, RESCHNO );
        resched( );
    }
    restore( &PS );
    return (OK );
}
