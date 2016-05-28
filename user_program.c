
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

    char strnum[3] = { 0, NULLCH, NULLCH };

    strnum[0] = ( getpid( ) + 48 );
    len = genoutput( str, "A: process A with pid", strnum, " alive\n" );
    write( CONSOLE, str, len );

    strnum[0] = ( ( sem / 10 ) + 48 );
    strnum[1] = ( ( sem % 10 ) + 48 );
    len = genoutput( str, "A: about to wait on semid ", strnum, "\n" );
    write( CONSOLE, str, len );



    if ( wait( sem ) == SYSERR )
        return;

    len = genoutput( str, "A: awake from semid ", strnum,
                     "; now waiting for msg\n" );
    write( CONSOLE, str, len );
    strnum[1] = NULLCH;


    short msg;
    if ( ( msg = receive( ) ) == SYSERR )
        return;

    strnum[0] = ( msg + 48 );
    len = genoutput( str, "A: just received message ", strnum,
                     "; now terminating\n" );
    write( CONSOLE, str, len );

    return;
}


void funcB( int sem, int pid )
{




    unsigned len;
    char str[BUFFLEN];
    char strnum[3] = { 0, NULLCH, NULLCH };

    strnum[0] = ( getpid( ) + 48 );
    len = genoutput( str, "B: process B with pid ", strnum, " alive\n" );
    write( CONSOLE, str, len );

    len = genoutput( str, "B: going to sleep for 2 sec ", "", "\n" );
    write( CONSOLE, str, len );

    if ( sleep( 2 ) == SYSERR )
        return;

    strnum[0] = ( ( sem / 10 ) + 48 );
    strnum[1] = ( ( sem % 10 ) + 48 );
    len = genoutput( str, "B: awake and signalling semid ", strnum, "\n" );
    write( CONSOLE, str, len );
    strnum[1] = NULLCH;

    if ( signal( sem ) == SYSERR )
        return;

    len = genoutput( str, "B: going to sleep for 2 sec ", "", "\n" );
    write( CONSOLE, str, len );

    if ( sleep( 2 ) == SYSERR )
        return;


    strnum[0] = ( pid + 48 );
    len = genoutput( str, "B: awake and sending msg 9 to pid ",
                     strnum, "\n" );
    write( CONSOLE, str, len );

    short msg = 9;
    if ( send( pid, msg ) == SYSERR )
        return;


    len = genoutput( str, "B: going to sleep for 2 sec ", "", "\n" );
    write( CONSOLE, str, len );

    if ( sleep( 2 ) == SYSERR )
        return;

    len = genoutput( str, "B: awake and now terminating ", "", "\n" );
    write( CONSOLE, str, len );


    return;
}


void funcC( int pid, int seconds )
{




    unsigned len;
    char str[BUFFLEN];
    char strnum[3] = { 0, NULLCH, NULLCH };

    strnum[0] = ( getpid( ) + 48 );
    len = genoutput( str, "C: process C with pid ", strnum, " alive\n" );
    write( CONSOLE, str, len );

    strnum[0] = ( ( seconds / 10 ) + 48 );
    strnum[1] = ( ( seconds % 10 ) + 48 );
    len = genoutput( str, "C: going to sleep for ", strnum, " seconds\n" );
    write( CONSOLE, str, len );
    strnum[1] = NULLCH;

    if ( sleep( seconds ) == SYSERR )
        return SYSERR;

    strnum[0] = ( pid + 48 );
    len = genoutput( str, "C: awake; now resuming pid ",
                     strnum, "\n" );
    write( CONSOLE, str, len );


    if ( resume( pid ) == SYSERR )
        return SYSERR;

    len = genoutput( str, "C: now terminating ", "", "\n" );
    write( CONSOLE, str, len );

    return;
}


void CLI( int a1, int a2 )
{



    char str[BUFFLEN];
    char argstr[BUFFLEN];
    memset( str, 0, BUFFLEN );
    memset( argstr, 0, BUFFLEN );
    unsigned len;
    unsigned arglen;


    len = genoutput( str, "CLI: started and initializing CLI", "", "\n" );
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
                goto JUNK_;


        }


    }




}


void initial_process( )
{
    int semid;
    int pidA, pidB, pidC;
    unsigned len;
    char str[BUFFLEN];
    char strnum[3] = { 0, NULLCH, NULLCH };

    strnum[0] = ( getpid( ) + 48 );
    len = genoutput( str, "INIT: initial process with pid ", strnum, " alive\n" );

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


    len = genoutput( str, "INIT: process A , B, C created", "", "\n" );
    write( CONSOLE, str, len );

    len = genoutput( str, "INIT: starting CLI", "", "\n" );
    write( CONSOLE, str, len );


    if ( resume( pidA ) == SYSERR ||
         resume( pidB ) == SYSERR ||
         resume( pidC ) == SYSERR ||
         resume( pidCLI ) == SYSERR ) {
        write( CONSOLE, "INIT ERROR1\n", strlen( "INIT ERROR1\n" ) );
        return;
    }




    len = genoutput( str, "INIT: about to suspend myself\n", "", "" );
    write( CONSOLE, str, len );

    if ( suspend( getpid( ) ) == SYSERR ) {
        write( CONSOLE, "INIT ERROR2\n", strlen( "INIT ERROR2\n" ) );
        return;
    }



    len = genoutput( str, "INIT: I've been resumed", "", "\n" );
    write( CONSOLE, str, len );


    while ( numproc > 2 ) {


        sleep( 3 );
    }

    len = genoutput( str, "INIT: only initial proc and idle proc remaining. ",
                     "", "terminating\n" );
    write( CONSOLE, str, len );

    shutdown = TRUE;

    return;
}
