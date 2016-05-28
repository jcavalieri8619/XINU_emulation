/* initialize.c - nulluser, sysinit */


#include <conf.h>
#include <io.h>
#include <slu.h>
#include <tty.h>

#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <sleep.h>
#include <mem.h>
#include <q.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#define  SHKEY       1577389

#define VERSION     "JPC_BETA_1.15 (2016)"
#define BUFFLEN 100



void nulluser( );
extern void initial_process( int ); /* address of user's main prog	*/

/* Declarations of major kernel variables */

struct pentry proctab[NPROC]; /* process table			*/
int nextproc; /* next process slot to use in create	*/
struct sentry semaph[NSEM]; /* semaphore table			*/
int nextsem; /* next semaphore slot to use in screate*/
struct qent q[NQENT]; /* q table (see queue.c)		*/
int nextqueue; /* next slot in q structure to use	*/
extern int *maxaddr; /* max memory address (set by sizmem)	*/
extern const int RAMSIZE; /*total size of system RAM */
struct mblock memlist; /* list of free memory blocks		*/

struct csr *tty0_csr;

#ifdef NDEVS
struct intmap intmap[NDEVS]; /* interrupt dispatch table		*/
#endif


#ifdef Ntty
struct tty tty[Ntty]; /* SLU buffers and mode control		*/
#endif




/* active system status */
volatile sig_atomic_t shutdown = FALSE;
int numproc; /* number of live user processes	*/
int currpid; /* id of currently running process	*/
int reboot = 0; /* non-zero after first boot		*/

int rdyhead, rdytail; /* head/tail of ready list (q indexes)	*/
char vers[] = VERSION; /* Xinu version printed at startup	*/

/************************************************************************/
/***				NOTE:				      ***/
/***								      ***/
/***   This is where the system begins after the C environment has    ***/
/***   been established.  Interrupts are initially DISABLED, and      ***/
/***   must eventually be enabled explicitly.  This routine turns     ***/
/***   itself into the null process after initialization.  Because    ***/
/***   the null process must always remain ready to run, it cannot    ***/
/***   execute code that might cause it to be suspended, wait for a   ***/
/***   semaphore, or put to sleep, or exit.  In particular, it must   ***/
/***   not do I/O unless it uses kprintf for polled output.           ***/
/***								      ***/

/************************************************************************/


/*------------------------------------------------------------------------
 *  sysinit  --  initialize all Xinu data structures and devices
 *------------------------------------------------------------------------
 */

LOCAL int sysinit( )
{
    int i;
    struct pentry *pptr;
    struct sentry *sptr;
    struct mblock *mptr;
    const char * idleName = "IDLE";
    numproc = 0; /* initialize system variables */
    nextproc = NPROC - 1;
    nextsem = NSEM - 1;
    nextqueue = NPROC; /* q[0..NPROC-1] are processes */


    int shmid;




    if ( SYSERR == meminit( ) ||
         SYSERR == interuptinit( ) )
        return SYSERR;

    memlist.mnext = mptr = /* initialize free memory list */
            (struct mblock *) roundew( minaddr );

    //memory length expressed in bytes (not words)
    memlist.mlen = RAMSIZE * sizeof (int );
    mptr->mnext = (struct mblock *) NULL;
    mptr->mlen = truncew( (unsigned) maxaddr - (unsigned) NULLSTK -
                          (unsigned) minaddr + sizeof (int ) );




    for ( i = 0; i < NPROC; i++ ) /* initialize process table */
        proctab[i].pstate = PRFREE;

    pptr = &proctab[NULLPROC]; /* initialize null process entry */
    pptr->pstate = PRCURR;
    pptr->pprio = 0;
    strncpy( pptr->pname, idleName, strlen( idleName ) );
    pptr->pbase = maxaddr;
    pptr->plimit = (int) pptr->pbase - NULLSTK + sizeof (int );
    pptr->pstklen = NULLSTK;


    *(int*) pptr->pbase = MAGIC;

    pptr->paddr = nulluser;
    pptr->phasmsg = FALSE;
    pptr->pargs = 0;
    numproc++;

    if ( getcontext( &pptr->run_env ) == -1 ) {
        handle_error( "getcontext: " );
    }

    if ( getcontext( &pptr->rtn_env ) == -1 ) {
        handle_error( "getcontext: " );
    }


    stack_t stackobj = { .ss_sp = pptr->pbase - NULLSTK + 1,
                        .ss_flags = 0,
                        .ss_size = pptr->pstklen };

    pptr->run_env.uc_stack = stackobj;


    makecontext( &pptr->run_env, nulluser, 0 );



    currpid = NULLPROC;

    for ( i = 0; i < NSEM; i++ ) { /* initialize semaphores */
        ( sptr = &semaph[i] )->sstate = SFREE;
        sptr->sqtail = 1 + ( sptr->sqhead = newqueue( ) );
    }

    rdytail = 1 + ( rdyhead = newqueue( ) ); /* initialize ready list */




#ifdef RTCLOCK
    clkinit( ); /* initialize r.t.clock	*/

#endif

    if ( ( shmid = shmget( SHKEY, 4096, IPC_CREAT | 0666 ) ) == -1 ) {
        handle_error( "shmget: " );
    }

    if ( ( tty0_csr = (struct csr *) shmat( shmid, 0, 0 ) ) == (void*) - 1 ) {
        handle_error( "shmat: " );
    }


    tty0_csr->crstat = SLUENABLE;
    tty0_csr->ctstat = SLUENABLE;
    tty0_csr->crbuf = 0;
    tty0_csr->ctbuf = 0;


    devtab[CONSOLE].dvcsr = tty0_csr;





    return (OK );
}


/*------------------------------------------------------------------------
 *  nulluser  -- initialize system and become the null process (id==0)
 *------------------------------------------------------------------------
 */


void nulluser( ) /* babysit CPU when no one home */
{
    int userpid;



    sysinit( ); /* initialize all of Xinu */



    init( CONSOLE );





    enable( ); /* enable interrupts */


    /* create a process to execute the user's main program */


    userpid = create( initial_process, INITSTK, INITPRIO, INITNAME, INITARGC );



    resume( userpid );





    while ( TRUE ) { /* run forever without actually */

        char s[BUFFLEN];
        int len;

        if ( shutdown == TRUE ) {

            len = sprintf( s, "IDLE: system shutting down\n" );
            write( CONSOLE, s, len );
            return ( OK );
        }
    }



    return; /* unreachable			*/
}


