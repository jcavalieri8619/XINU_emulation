/* ttywrite.c - ttywrite, writcopy */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>
#include <slu.h>

LOCAL int writcopy( );

int ttywrite( struct devsw *devptr, char *buff, int count )
{
    register struct tty *ttyp;
    int avail;

    sigset_t PS;
    if ( count < 0 )
        return (SYSERR );
    if ( count == 0 )
        return (OK );
    disable( &PS );
    ttyp = &tty[devptr->dvminor];
    const int origcount = count;
    int writeCount;
    Bool isFinished = FALSE;
    while ( ( avail = scount( ttyp->osem ) ) >= count ) {

        writeCount = writcopy( buff + ( origcount - count ), ttyp, count );

        if ( !( count -= writeCount ) ) {
            ( ttyp->ioaddr )->ctstat = SLUENABLE;
            isFinished = TRUE;
            break;
        }
    }

    if ( !isFinished ) {
        if ( avail > 0 ) {
            writeCount = writcopy( buff + ( origcount - count ), ttyp, avail );
            buff += writeCount;
            count -= writeCount;
            ( ttyp->ioaddr )->ctstat = SLUENABLE;
        }
        for (; count > 0; count-- )
            ttyputc( devptr, *buff++ );

    }
    restore( &PS );
    return (OK );
}


/*------------------------------------------------------------------------
 *  writcopy - high-speed copy from user's buffer into system buffer
 *------------------------------------------------------------------------
 */
LOCAL int writcopy( register char *buff, struct tty *ttyp, int count )
{
    register char *qhead, *qend, *uend;
    int retCount = 0;
    qhead = &ttyp->obuff[ttyp->ohead];
    qend = &ttyp->obuff[OBUFLEN];
    uend = buff + count;
    int ch;
    while ( buff < uend ) {
        if ( ( ( ch = *buff++ ) == NEWLINE ) && ttyp->ocrlf ) {
            *qhead++ = RETURN;
            retCount++;

        }
        *qhead++ = ch;
        if ( qhead >= qend )
            qhead = ttyp->obuff;
    }
    ttyp->ohead = qhead - ttyp->obuff;
    sreset( ttyp->osem, scount( ttyp->osem ) - count );
    return count - retCount;
}



























/*------------------------------------------------------------------------
 *  ttywrite - write one or more characters to a tty device
 *------------------------------------------------------------------------
 */

/*
int ttywrite( struct devsw *devptr, char *buff, int count )
{
    register struct tty *ttyp;
    int avail;

    sigset_t PS;
    if ( count < 0 )
        return (SYSERR );
    if ( count == 0 )
        return (OK );
    disable( &PS );
    ttyp = &tty[devptr->dvminor];
    int writeCount;
    Bool isFinished = FALSE;
    while ( ( avail = scount( ttyp->osem ) ) >= count ) {

        writeCount = writcopy( buff, ttyp, count );

        if ( !( count -= writeCount ) ) {
            ( ttyp->ioaddr )->ctstat = SLUENABLE;
            isFinished = TRUE;
            break;
        }
    }

    if ( !isFinished || ( avail < count ) ) {
        if ( avail > 0 ) {
            writeCount = writcopy( buff, ttyp, avail );
            buff += avail - writeCount;
            count -= avail + writeCount;
            ( ttyp->ioaddr )->ctstat = SLUENABLE;
        }
        for (; count > 0; count-- )
            ttyputc( devptr, *buff++ );

    }
    restore( &PS );
    return (OK );
}



LOCAL int writcopy( register char *buff, struct tty *ttyp, int count )
{
    register char *qhead, *qend, *uend;
    int retCount = 0;
    qhead = &ttyp->obuff[ttyp->ohead];
    qend = &ttyp->obuff[OBUFLEN];
    uend = buff + count;
    int ch;
    while ( buff < uend ) {
        if ( ( ( ch = *buff++ ) == NEWLINE ) && ttyp->ocrlf ) {
            *qhead++ = RETURN;
            retCount++;

        }
        *qhead++ = ch;
        if ( qhead >= qend )
            qhead = ttyp->obuff;
    }
    ttyp->ohead = qhead - ttyp->obuff;
    sreset( ttyp->osem, scount( ttyp->osem ) - count );
    return count - retCount;
}
*/

