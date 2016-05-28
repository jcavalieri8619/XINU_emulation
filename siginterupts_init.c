
#include "kernel.h"
#include "sleep.h"
#include "stdio.h"
#include "conf.h"
#include <io.h>

#include <signal.h>




static sigset_t init_mask;
static sigset_t CLOCKSIG;
static sigset_t full_block;
static sigset_t masksigs;
LOCAL struct sigaction sigclockdat;
LOCAL struct sigaction sigdat;
LOCAL struct sigaction sigiintdat;
LOCAL struct sigaction sigointdat;


void kill_terminal( int sig )
{
    char s[100];
    int len;

    len = sprintf( s, "Xinu going down on signal %d\n", sig );
    write( CONSOLE, s, len );

    halt_system( );
}


void inputDispatcher( int sig )
{
    ( *intmap[CONSOLE].iin )( intmap[CONSOLE].icode );

}


void outputDispatcher( int sig )
{
    ( *intmap[CONSOLE].iout )( intmap[CONSOLE].ocode );
}


extern SYSCALL disable( sigset_t * oldsig )
{

    sigemptyset( oldsig );
    if ( sigprocmask( SIG_BLOCK, &CLOCKSIG, oldsig ) == -1 ) {
        exit( SYSERR );
    }

}


extern SYSCALL restore( sigset_t * oldsig )
{
    if ( sigprocmask( SIG_UNBLOCK, oldsig, NULL ) == -1 ) {
        exit( SYSERR );
    }
}


extern SYSCALL enable( )
{


    if ( sigprocmask( SIG_UNBLOCK, &full_block, NULL ) == -1 ) {
        exit( SYSERR );
    }

}


extern int interuptinit( )
{



    sigemptyset( &full_block );

    sigaddset( &full_block, SIGVTALRM );
    sigaddset( &full_block, SIGUSR1 );
    sigaddset( &full_block, SIGUSR2 );


    sigemptyset( &CLOCKSIG );
    sigaddset( &CLOCKSIG, SIGVTALRM );

    if ( sigprocmask( SIG_BLOCK, &full_block, &init_mask ) == -1 ) {
        handle_error( "sigprocmask: " );
    }

    sigfillset( &masksigs );

    sigdat.sa_handler = kill_terminal;
    sigdat.sa_mask = masksigs;
    sigdat.sa_flags = SA_RESTART;

    if ( sigaction( SIGTERM, &sigdat, (struct sigaction *) NULL ) == -1 ) {
        handle_error( "sigaction: " );
    }

    sigclockdat.sa_handler = clkint;
    sigclockdat.sa_mask = masksigs;
    sigclockdat.sa_flags = SA_RESTART;

    if ( sigaction( SIGVTALRM, &sigclockdat, (struct sigaction *) 0 ) == -1 ) {
        handle_error( "sigaction: " );
    }


    sigiintdat.sa_handler = inputDispatcher;
    sigiintdat.sa_mask = masksigs;
    sigiintdat.sa_flags = SA_RESTART;

    if ( sigaction( SIGUSR1, &sigiintdat, (struct sigaction *) NULL ) == -1 ) {
        handle_error( "sigaction: " );
    }


    sigointdat.sa_handler = outputDispatcher;
    sigointdat.sa_mask = masksigs;
    sigointdat.sa_flags = SA_RESTART;

    if ( sigaction( SIGUSR2, &sigointdat, (struct sigaction *) NULL ) == -1 ) {
        handle_error( "sigaction: " );
    }



}