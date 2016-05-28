
#include "kernel.h"
#include "sleep.h"
#include "stdio.h"
#include "conf.h"
#include <io.h>

#include <signal.h>


static sigset_t clock_IO_BLOCK;
static struct sigaction sigdat;


sigset_t DISABLE_INTERUPTS;
struct sigaction sigClockinterupt;
struct sigaction sigIOinterupts;


extern void kill_XINU( int sig )
{
    char s[100];
    int len;

    len = sprintf( s, "Xinu going down on signal %d\n", sig );
    write( CONSOLE, s, len );

    halt_system( );
}


extern void IOdispatcher( int signum )
{
    switch ( signum ) {
        case SIGUSR1:
            ( intmap[CONSOLE].iin )( intmap[CONSOLE].icode );
            break;
        case SIGUSR2:
            ( intmap[CONSOLE].iout )( intmap[CONSOLE].ocode );
            break;
    }
}


extern SYSCALL disable( sigset_t * oldsig )
{


    if ( sigprocmask( SIG_BLOCK, &clock_IO_BLOCK, oldsig ) == -1 ) {
        exit( SYSERR );
    }

}


extern SYSCALL restore( sigset_t * oldsig )
{
    if ( sigprocmask( SIG_SETMASK, oldsig, NULL ) == -1 ) {
        exit( SYSERR );
    }
}


extern SYSCALL enable( )
{
    if ( sigprocmask( SIG_UNBLOCK, &clock_IO_BLOCK, NULL ) == -1 ) {
        exit( SYSERR );
    }

}


extern int interuptinit( )
{
    //sigset_t init_mask;

    sigemptyset( &clock_IO_BLOCK );

    sigfillset( &DISABLE_INTERUPTS );

    /*
        if ( sigprocmask( SIG_SETMASK, NULL, &init_mask ) == -1 ) {
            exit( SYSERR );
        }

        clock_IO_BLOCK = init_mask;
     */
    sigaddset( &clock_IO_BLOCK, SIGVTALRM );
    sigaddset( &clock_IO_BLOCK, SIGUSR1 );
    sigaddset( &clock_IO_BLOCK, SIGUSR2 );




    if ( sigprocmask( SIG_UNBLOCK, &clock_IO_BLOCK, NULL ) == -1 ) {
        exit( SYSERR );
    }






    sigdat.sa_handler = kill_XINU;
    sigdat.sa_mask = DISABLE_INTERUPTS;
    sigdat.sa_flags = SA_RESTART;

    if ( sigaction( SIGTERM, &sigdat, (struct sigaction *) NULL ) == -1 ) {
        handle_error( "sigaction: " );
    }



}