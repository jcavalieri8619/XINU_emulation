/* sleep10.c - sleep10 */


#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>


/*------------------------------------------------------------------------
 * sleep10  --  delay the caller for a time specified in tenths of seconds
 *------------------------------------------------------------------------
 */



SYSCALL sleep10( int n )
{

    sigset_t PS;
    if ( n < 0 || clkruns == 0 )
        return (SYSERR );

    disable( &PS );
    if ( n == 0 ) { /* sleep10(0) -> end time slice	*/
        resched( );
        restore( &PS );
        return (OK );
    }


    insertd( currpid, clockq, n );
    slnempty = TRUE;
    sltop = &q[q[clockq].qnext].qkey;



    proctab[currpid].pstate = PRSLEEP;
    resched( );
    restore( &PS );
    return (OK );
}
