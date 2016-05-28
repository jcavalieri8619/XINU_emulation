/* screate.c - screate, newsem */


#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>

LOCAL int newsem( );


/*------------------------------------------------------------------------
 * screate  --  create and initialize a semaphore, returning its id
 *------------------------------------------------------------------------
 */
SYSCALL screate( int count ) /* initial count (>=0)          */
{

    int sem;
    sigset_t PS;
    disable( &PS );
    if ( count < 0 || ( sem = newsem( ) ) == SYSERR ) {
        restore( &PS );
        handle_error( "screate" );
        return (SYSERR );
    }
    semaph[sem].semcnt = count;
    /* sqhead and sqtail were initialized at system startup */
    restore( &PS );
    return (sem );
}


/*------------------------------------------------------------------------
 * newsem  --  allocate an unused semaphore and return its index
 *------------------------------------------------------------------------
 */
LOCAL int newsem( )
{
    int sem;
    int i;

    for ( i = 0; i < NSEM; i++ ) {
        sem = nextsem--;
        if ( nextsem < 0 )
            nextsem = NSEM - 1;
        if ( semaph[sem].sstate == SFREE ) {
            semaph[sem].sstate = SUSED;
            return (sem );
        }
    }
    return (SYSERR );
}
