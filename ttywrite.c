/* ttywrite.c - ttywrite, writcopy */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>
#include <slu.h>

LOCAL int writcopy( );


/*------------------------------------------------------------------------
 *  ttywrite - write one or more characters to a tty device
 *------------------------------------------------------------------------
 */
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

        if ( !writeCount ) {
            ( ttyp->ioaddr )->ctstat = SLUENABLE;
            break;
        }

        if ( !( count -= writeCount ) ) {
            ( ttyp->ioaddr )->ctstat = SLUENABLE;
            isFinished = TRUE;
            break;
        }
    }

    if ( !isFinished ) {
        if ( ( avail = scount( ttyp->osem ) ) > 0 ) {
            writeCount = writcopy( buff + ( origcount - count ), ttyp, avail );
            count -= writeCount;
            ( ttyp->ioaddr )->ctstat = SLUENABLE;
        }
        buff += ( origcount - count );
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
    char *uend;

    int toBuffer = 0;
    int fromUser = 0;
    int ch;

    uend = buff + count;



    while ( buff < uend ) {

        if ( ( ( ch = *buff++ ) == NEWLINE ) && ttyp->ocrlf ) {

            if ( scount( ttyp->osem ) >= toBuffer + 2 ) {

                ttyp->obuff[ttyp->ohead ] = RETURN;
                ttyp->ohead = ( ttyp->ohead + 1 ) % OBUFLEN;
                ttyp->obuff[ttyp->ohead] = ch;
                ttyp->ohead = ( ttyp->ohead + 1 ) % OBUFLEN;
                fromUser++;
                toBuffer += 2;

            } else if ( scount( ttyp->osem ) >= toBuffer + 1 ) {

                ttyp->obuff[ttyp->ohead] = NEWLINE;
                ttyp->ohead = ( ttyp->ohead + 1 ) % OBUFLEN;
                toBuffer++;
                *( buff - 1 ) = RETURN;
                break;

            } else
                break;

        } else if ( scount( ttyp->osem ) >= toBuffer + 1 ) {

            ttyp->obuff[ ttyp->ohead] = ch;
            ttyp->ohead = ( ttyp->ohead + 1 ) % OBUFLEN;
            toBuffer++;
            fromUser++;

        } else {

            break;
        }

    }

    sreset( ttyp->osem, scount( ttyp->osem ) - toBuffer );
    return fromUser;
}
