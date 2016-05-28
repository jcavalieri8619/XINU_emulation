/*create.c create, newpid*/

#include <conf.h>
#include <io.h>

#include <kernel.h>
#include <proc.h>
#include <mem.h>

#include <stdarg.h>

#include <unistd.h>
#include <string.h>





LOCAL int newpid( );


/*------------------------------------------------------------------------
 *  create  -  create a process to start running a procedure
 *------------------------------------------------------------------------
 */
SYSCALL create( void (*procaddr )( ), /* procedure address            */
                int ssize, /* stack size in words          */
                int priority, /* process priority > 0         */
                char *name, /* name (for debugging)         */
                int nargs, int arg1, int arg2 ) /* number of args that follow   */


{
    int pid; /* stores new process id	*/
    struct pentry *pptr; /* pointer to proc. table entry */
    int i;
    int *a; /* points to list of args	*/
    int *saddr; /* stack address		*/


    int INITRET( );

    sigset_t PS;

    disable( &PS );

    ssize = roundew( ssize );



    if ( ssize < MINSTK ||
         ( pid = newpid( ) ) == SYSERR ||
         priority < 1 ||
         (int) ( saddr = getstk( ssize ) ) == SYSERR ) {

        restore( &PS );

        return (SYSERR );
    }

    numproc++;
    pptr = &proctab[pid];
    pptr->pstate = PRSUSP;


    strncpy( pptr->pname, name, PNMLEN );
    pptr->pprio = priority;

    pptr->pstklen = ssize;
    pptr->pbase = saddr;
    pptr->psem = 0;
    pptr->phasmsg = FALSE;
    pptr->plimit = ( (int) pptr->pbase - ssize + sizeof (int ) );
    pptr->pargs = nargs;
    pptr->paddr = procaddr;


    if ( getcontext( &pptr->run_env ) == -1 ) {

        handle_error( "getcontext: " );
    }

    if ( getcontext( &pptr->rtn_env ) == -1 ) {

        handle_error( "getcontext: " );
    }


    stack_t stackobj = { .ss_sp = ( pptr->pbase - ssize + 1 ),
                        .ss_flags = 0,
                        .ss_size = pptr->pstklen };

    pptr->rtn_env.uc_stack = pptr->run_env.uc_stack = stackobj;



    makecontext( &pptr->rtn_env, INITRET, 0 );


    pptr->run_env.uc_link = &pptr->rtn_env;






    makecontext( &pptr->run_env, procaddr, nargs, arg1, arg2 );





    restore( &PS );
    return (pid );
}


/*------------------------------------------------------------------------
 * newpid  --  obtain a new (free) process id
 *------------------------------------------------------------------------
 */

LOCAL int newpid( )
{
    int pid; /* process id to return		*/
    int i;

    for ( i = 0; i < NPROC; i++ ) { /* check all NPROC slots	*/
        if ( ( pid = nextproc-- ) <= 0 )
            nextproc = NPROC - 1;
        if ( proctab[pid].pstate == PRFREE )
            return (pid );
    }
    return (SYSERR );
}
