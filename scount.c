/* scount.c - scount */


#include <conf.h>
#include <kernel.h>
#include <sem.h>


/*------------------------------------------------------------------------
 *  scount  --  return a semaphore count
 *------------------------------------------------------------------------
 */
SYSCALL scount( int sem )
{

    if ( isbadsem( sem ) || semaph[sem].sstate == SFREE ) {
        handle_error( "semcount: " );
        return (SYSERR );
    }
    return (semaph[sem].semcnt );
}
