/* kill.c - kill */

#include <conf.h>
#include <io.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <q.h>
#include <sleep.h>


/*------------------------------------------------------------------------
 * kill  --  kill a process and remove it from the system
 *------------------------------------------------------------------------
 */



SYSCALL kill( int pid ) /* process to kill              */
{
    struct pentry *pptr; /* points to proc. table for pid*/
    sigset_t PS;


    disable( &PS );
    if ( isbadpid( pid ) || ( pptr = &proctab[pid] )->pstate == PRFREE ) {
        restore( &PS );
        return (SYSERR );
    }

    numproc--;
    freestk( pptr->pbase, pptr->pstklen );

    switch ( pptr->pstate ) {

        case PRCURR:
            pptr->pstate = PRFREE; /* suicide */

            resched( );


        case PRWAIT:
            semaph[pptr->psem].semcnt++;
            dequeue( pid );

            break;

        case PRSLEEP:
        {
            int nxt;


            if ( ( nxt = q[pid].qnext ) < NPROC ) { /*is next a valid proc--not tail? */
                q[nxt].qkey += q[pid].qkey;


            }

            dequeue( pid );

            if ( slnempty = nonempty( clockq ) )
                sltop = (int*) &q[q[clockq].qnext].qkey;
            else sltop = NULL;

        }
            break;
        case PRREADY:
            dequeue( pid );

            break;

        default:
            ;

    }

    pptr->pstate = PRFREE;


    restore( &PS );
    return (OK );
}
