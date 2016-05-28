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
    int *link_saddr;


    void INITRET( );

    sigset_t PS;

    disable( &PS );

    ssize = roundew( ssize );



    if ( ssize < MINSTK ||
         ( pid = newpid( ) ) == SYSERR ||
         priority < 1 ||
         ( (int) ( saddr = getstk( ssize ) ) == SYSERR ) ||
         ( (int) ( link_saddr = getstk( MINSTK ) ) == SYSERR ) ) {

        restore( &PS );
        handle_error( "create: " );

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


    pptr->run_env = context_init;

    pptr->rtn_env = context_init;




    stack_t stackobj = { .ss_sp = ( pptr->pbase - ssize + 1 ),
                        .ss_flags = 0,
                        .ss_size = pptr->pstklen };

    stack_t link_stackobj = { .ss_sp = ( link_saddr - MINSTK + 1 ),
                             .ss_flags = 0,
                             .ss_size = MINSTK };


    pptr->run_env.uc_stack = stackobj;
    pptr->rtn_env.uc_stack = link_stackobj;



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
