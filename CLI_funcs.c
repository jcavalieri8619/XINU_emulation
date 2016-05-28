#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <kernel.h>
#include <sem.h>
#include <mem.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <string.h>
#include <stdio.h>
#include <conf.h>



/*

 */


/*

 */

#define BUFFLEN 100
extern sig_atomic_t shutdown;


int show_proc_table( )
{
    int proc_iter = NPROC;
    char str[BUFFLEN];
    int len;

    while ( --proc_iter >= 0 ) {

        switch ( proctab[proc_iter].pstate ) {
            case PRCURR:
                len = sprintf( str, "PROC %d IS CURRENT\n", proc_iter );
                write( CONSOLE, str, len );
                break;
            case PRFREE:
                len = sprintf( str, "PROC %d IS FREE\n", proc_iter );
                write( CONSOLE, str, len );
                break;
            case PRREADY:
                len = sprintf( str, "PROC %d IS READY\n", proc_iter );
                write( CONSOLE, str, len );
                break;
            case PRRECV:
                len = sprintf( str, "PROC %d IS RECEIVING\n", proc_iter );
                write( CONSOLE, str, len );
                break;
            case PRSLEEP:
                len = sprintf( str, "PROC %d IS SLEEPING\n", proc_iter );
                write( CONSOLE, str, len );
                break;
            case PRSUSP:
                len = sprintf( str, "PROC %d IS SUSPENDED\n", proc_iter );
                write( CONSOLE, str, len );
                break;
            case PRWAIT:
                len = sprintf( str, "PROC %d IS SEMAPHORE WAITING\n", proc_iter );
                write( CONSOLE, str, len );
                break;
            default:
                len = sprintf( str, "PROC %d UNKOWN STATE\n", proc_iter );
                write( CONSOLE, str, len );


        }

    }
    return OK;
}


int show_ready_Q( )
{

    int len;
    char str[BUFFLEN];
    int rdy_indx = rdyhead;


    if ( isempty( rdyhead ) ) {
        len = sprintf( str, "READY QUEUE EMPTY --> IDLE THREAD IS CURRENT\n" );
        write( CONSOLE, str, len );

        return OK;
    }


    while ( ( rdy_indx = q[rdy_indx].qnext ) < NPROC ) {


        len = sprintf( str, "PROC %d HAS PRIORITY %d ON READY QUEUE\n",
                       rdy_indx, q[rdy_indx].qkey );


        write( CONSOLE, str, len );




    }


    return OK;
}


int show_sleep_Q( )
{


    int len;
    char str[BUFFLEN];



    int slp_indx = clockq;


    while ( ( slp_indx = q[slp_indx].qnext ) < NPROC ) {

        len = sprintf( str, "PROC %d HAS TIME-DELTA %d ON SLEEP QUEUE\n",
                       slp_indx, q[slp_indx].qkey );
        write( CONSOLE, str, len );

    }

    if ( !slnempty ) {
        len = sprintf( str, "SLEEP QUEUE EMPTY\n" );
        write( CONSOLE, str, len );

    }

    return OK;
}


int resume_proc( int pid )
{
    int len;
    char str[BUFFLEN];



    len = sprintf( str, "resuming PID %d\n", pid );
    write( CONSOLE, str, len );

    return resume( pid );

}


int kill_proc( int pid )
{

    int len;
    char str[BUFFLEN];


    len = sprintf( str, "killing PID %d\n", pid );
    write( CONSOLE, str, len );

    return kill( pid );
}


int suspend_proc( int pid )
{
    int len;
    char str[BUFFLEN];



    len = sprintf( str, "suspending PID %d\n", pid );
    write( CONSOLE, str, len );

    return suspend( pid );
}


int signal_sem( int semID )
{

    int len;
    char str[BUFFLEN];



    len = sprintf( str, "signalling semID %d\n", semID );
    write( CONSOLE, str, len );

    return signal( semID );
}


int loop_program( int loops, int arg )
{
    int len;
    char str[BUFFLEN];



    len = sprintf( str, "loop program PID %d started;"
                   " infinitely looping until killed or suspended\n",
                   getpid( ) );
    write( CONSOLE, str, len );

    while ( TRUE ) {

    }

    return OK;
}


int sleeper_program( int seconds, int arg )
{
    int len;
    char str[BUFFLEN];




    len = sprintf( str, "sleeper program PID %d started with %d seconds\n",
                   getpid( ), seconds );

    write( CONSOLE, str, len );

    sleep( seconds );

    len = sprintf( str, "sleeper program PID %d awake and terminating\n",
                   getpid( ) );

    write( CONSOLE, str, len );



    return OK;
}


int semwaiter_program( int semID, int arg )
{
    int len;
    char str[BUFFLEN];





    len = sprintf( str, "sem waiter program PID %d started with sem ID %d\n",
                   getpid( ), semID );



    write( CONSOLE, str, len );

    wait( semID );

    len = sprintf( str, "sem waiter program PID %d awake from sem ID %d\n",
                   getpid( ), semID );
    write( CONSOLE, str, len );






    return OK;
}


int halt_system( )
{

    int pid = 0;

    while ( !( isbadpid( pid ) && pid == getpid( ) ) ) {

        kill( pid );

        pid++;
    }


    kill( getpid( ) );

    shutdown = TRUE;
}