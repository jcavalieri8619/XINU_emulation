/* send.c - send */


#include <conf.h>
#include <kernel.h>
#include <proc.h>


/*------------------------------------------------------------------------
 *  send  --  send a message to another process
 *------------------------------------------------------------------------
 */
SYSCALL send( int pid, int msg )
{
    struct pentry *pptr; /* receiver's proc. table addr.	*/
    char ps;
    sigset_t PS;
    disable( &PS );
    if ( isbadpid( pid ) || ( ( pptr = &proctab[pid] )->pstate == PRFREE )
         || pptr->phasmsg ) {
        restore( &PS );
        return (SYSERR );
    }
    pptr->pmsg = msg; /* deposit message		*/
    pptr->phasmsg = TRUE;
    if ( pptr->pstate == PRRECV ) /* if receiver waits, start it	*/
        ready( pid, RESCHYES );
    restore( &PS );
    return (OK );
}
