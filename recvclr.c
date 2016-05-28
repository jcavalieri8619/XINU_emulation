/* recvclr.c - recvclr */


#include <conf.h>
#include <kernel.h>
#include <proc.h>


/*------------------------------------------------------------------------
 *  recvclr  --  clear messages, returning waiting message (if any)
 *------------------------------------------------------------------------
 */
SYSCALL recvclr( )
{
    char ps;
    int msg;
    sigset_t PS;
    disable( &PS );
    if ( proctab[currpid].phasmsg ) { /* existing message?	*/
        proctab[currpid].phasmsg = FALSE;
        msg = proctab[currpid].pmsg;
    } else
        msg = OK;
    restore( &PS );
    return (msg );
}
