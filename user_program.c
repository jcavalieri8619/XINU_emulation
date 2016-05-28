
/*
 * File:   user_program.c
 * Author: jcavalie
 *
 * Created on March 17, 2016, 5:50 PM
 */

#include <conf.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <cli_symbols.h>
#include <kernel.h>
#include <sem.h>
#include <mem.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <string.h>

#include <x86_64-linux-gnu/bits/fcntl-linux.h>
#include <stdio.h>
#include <stdarg.h>


extern volatile sig_atomic_t shutdown;
extern char* yytext;
#define BUFFLEN 100



#define PRINTPROMPT() do {\
    char * prompt = "XINU_CLI >>  ";\
    write( CONSOLE, prompt, strlen( prompt ) );        \
    } while(0)


extern int sleeper_program( int seconds, int arg );
extern int loop_program( int loops, int arg );
extern int semwaiter_program( int semID, int arg );


unsigned genoutput( char * str, char * left, char* mid, char* right )
{

    memset( str, 0, BUFFLEN );

    strncpy( str, left, strlen( left ) );
    strncat( str, mid, strlen( mid ) );
    strncat( str, right, strlen( right ) );

    return strlen( str );
}


/*
int atoi( const char *str )
{
    return atoi( str );
}
 */


void funcA( int sem, int arg )
{



    unsigned len;
    char str[BUFFLEN];

    memset( str, 0, BUFFLEN );
    len = sprintf( str, "A: pid %d alive\n", getpid( ) );

    write( CONSOLE, str, len );


    len = sprintf( str, "A: wait semid %d\n", sem );

    write( CONSOLE, str, len );



    if ( wait( sem ) == SYSERR )
        return;


    len = sprintf( str, "A: awake semid %d\n", sem );
    write( CONSOLE, str, len );



    short msg;
    if ( ( msg = receive( ) ) == SYSERR )
        return;

    len = sprintf( str, "A: rcved message %d\n", msg );

    write( CONSOLE, str, len );

    len = sprintf( str, "A: finished\n" );

    write( CONSOLE, str, len );

    return;
}


void funcB( int sem, int pid )
{




    unsigned len;
    char str[BUFFLEN];
    memset( str, 0, BUFFLEN );

    len = sprintf( str, "B: pid %d alive\n", getpid( ) );

    write( CONSOLE, str, len );

    len = sprintf( str, "B: sleep 2 sec\n" );
    write( CONSOLE, str, len );

    if ( sleep( 2 ) == SYSERR )
        return;


    len = sprintf( str, "B: signal semid %d\n", sem );
    write( CONSOLE, str, len );

    if ( signal( sem ) == SYSERR )
        return;


    len = sprintf( str, "B: sleep 2 sec\n" );
    write( CONSOLE, str, len );

    if ( sleep( 2 ) == SYSERR )
        return;


    len = sprintf( str, "B: msg 9 to pid %d\n", pid );
    write( CONSOLE, str, len );

    short msg = 9;
    if ( send( pid, msg ) == SYSERR )
        return;


    len = sprintf( str, "B: sleep 2 sec\n" );
    write( CONSOLE, str, len );

    if ( sleep( 2 ) == SYSERR )
        return;


    len = sprintf( str, "B: finished\n" );
    write( CONSOLE, str, len );


    return;
}


void funcC( int pid, int seconds )
{




    unsigned len;
    char str[BUFFLEN];
    memset( str, 0, BUFFLEN );

    len = sprintf( str, "C: pid %d alive\n", getpid( ) );
    write( CONSOLE, str, len );



    len = sprintf( str, "C: sleep %d sec\n", seconds );
    write( CONSOLE, str, len );


    if ( sleep( seconds ) == SYSERR )
        return SYSERR;


    len = sprintf( str, "C: resume pid %d\n", pid );
    write( CONSOLE, str, len );


    if ( resume( pid ) == SYSERR )
        return SYSERR;

    len = sprintf( str, "C: finished\n" );
    write( CONSOLE, str, len );

    return;
}


void CLI( int a1, int a2 )
{



    char str[BUFFLEN];

    memset( str, 0, BUFFLEN );

    unsigned len;




    len = sprintf( str, "CLI: started and initializing\n" );
    write( CONSOLE, str, len );

    PRINTPROMPT( );

    while ( TRUE ) {





        int tokenID;



        switch ( tokenID = yylex( ) ) {

            case PROMPT:
                PRINTPROMPT( );

                break;


            case SHOW:

                switch ( tokenID = yylex( ) ) {

                    case PROC:
                        show_proc_table( );
                        break;
                    case RDY:
                        show_ready_Q( );
                        break;
                    case SLP:
                        show_sleep_Q( );
                        break;
                    default:
                        goto JUNK_;
                }
                PRINTPROMPT( );
                break;
            case CREATE:
                switch ( tokenID = yylex( ) ) {

                    case SLP:
                        if ( yylex( ) == INTEG ) {
                            resume( create( sleeper_program, INITSTK,
                                            INITPRIO, "sleeper",
                                            2, atoi( yytext ), 0 ) );
                        } else {
                            len = sprintf( str,
                                           "error: sleeper needs seconds arg\n" );
                            write( CONSOLE, str, len );
                        }

                        break;
                    case WTR:
                        if ( yylex( ) == INTEG ) {
                            resume( create( semwaiter_program, INITSTK,
                                            INITPRIO, "semwaiter",
                                            2, atoi( yytext ), 0 ) );
                        } else {
                            len = sprintf( str,
                                           "error: semwaiter sem ID arg\n" );
                            write( CONSOLE, str, len );
                        }

                        break;
                    case LOOP:

                        resume( create( loop_program, INITSTK,
                                        INITPRIO, "looper",
                                        2, 0, 0 ) );





                        break;
                    case SEMAPHORE:
                    {
                        int semid;
                        if ( yylex( ) == INTEG ) {
                            int count = atoi( yytext );
                            semid = screate( count );
                            len = sprintf( str,
                                           "semaphore created with count %d"
                                           " and sem ID %d\n", count, semid );
                            write( CONSOLE, str, len );
                        } else {
                            len = sprintf( str,
                                           "error: need inititial count for sem\n" );
                            write( CONSOLE, str, len );
                        }
                        break;
                    }
                    default:
                        goto JUNK_;
                }
                PRINTPROMPT( );
                break;

            case RESUME:
                switch ( tokenID = yylex( ) ) {
                    case INTEG:
                        resume_proc( atoi( yytext ) );
                        break;
                    default:
                        len = sprintf( str,
                                       "usage: resume command requires int PID\n" );
                        write( CONSOLE, str, len );

                }
                PRINTPROMPT( );
                break;
            case KILL:
                switch ( tokenID = yylex( ) ) {
                    case INTEG:
                        kill_proc( atoi( yytext ) );
                        break;
                    default:
                        len = sprintf( str,
                                       "usage: kill command requires int PID\n" );
                        write( CONSOLE, str, len );

                }
                PRINTPROMPT( );
                break;
            case SEMSIGNL:
                switch ( tokenID = yylex( ) ) {
                    case INTEG:
                        signal_sem( atoi( yytext ) );
                        break;
                    default:
                        len = sprintf( str,
                                       "usage: signal command requires int semID\n" );
                        write( CONSOLE, str, len );

                }
                PRINTPROMPT( );
                break;
            case SUSPEND:

                switch ( tokenID = yylex( ) ) {
                    case INTEG:
                        suspend_proc( atoi( yytext ) );
                        break;
                    default:
                        len = sprintf( str,
                                       "usage: suspend command requires int PID\n" );
                        write( CONSOLE, str, len );

                }
                PRINTPROMPT( );
                break;

            case EXIT:
                len = sprintf( str,
                               "GoodBye--shutting down\n" );
                write( CONSOLE, str, len );
                halt_system( );

            case JUNK:
                JUNK_:
                        len = sprintf( str,
                                       "Unrecognized Command\n\n" );
                write( CONSOLE, str, len );

                PRINTPROMPT( );
                break;

            default:
                ;


        }


    }




}


void initial_process( )
{
    int semid;
    int pidA, pidB, pidC;
    unsigned len;
    char str[BUFFLEN];
    memset( str, 0, BUFFLEN );


    //len = sprintf( str, "INIT: initial process with pid %d alive\n", getpid( ) );
    len = sprintf( str, "INIT %d\n", getpid( ) );
    write( CONSOLE, str, len );

    if ( ( semid = screate( 0 ) ) == SYSERR )
        return;

    if ( ( pidA = create( funcA, INITSTK, INITPRIO, "A", 2,
                          semid, 0 ) ) == SYSERR )
        return;

    if ( ( pidB = create( funcB, INITSTK, INITPRIO, "B", 2,
                          semid, pidA ) ) == SYSERR )
        return;

    int sleeptime = 30;
    if ( ( pidC = create( funcC, INITSTK, INITPRIO, "C", 2,
                          getpid( ), sleeptime ) ) == SYSERR )
        return;

    int pidCLI;
    if ( ( pidCLI = create( CLI, 10 * INITSTK, INITPRIO, "CLI_init", 2, 0, 0 ) ) == SYSERR )
        return;



    len = sprintf( str, "INIT: A,B,C created\n" );
    write( CONSOLE, str, len );


    //  resume( pidCLI ) == SYSERR
    if ( resume( pidA ) == SYSERR ||
         resume( pidB ) == SYSERR ||
         resume( pidC ) == SYSERR ||
         resume( pidCLI ) == SYSERR ) {

        len = sprintf( str, "INIT: ERROR" );
        write( CONSOLE, str, len );
        return;
    }


    len = sprintf( str, "INIT: suspend\n" );
    write( CONSOLE, str, len );

    if ( suspend( getpid( ) ) == SYSERR ) {

        len = sprintf( str, "INIT: ERROR\n" );
        write( CONSOLE, str, len );
        return;
    }


    len = sprintf( str, "INIT: resumed\n" );
    write( CONSOLE, str, len );




    len = sprintf( str, "INIT: finished\n" );
    write( CONSOLE, str, len );



    return;
}
