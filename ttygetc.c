/* ttygetc.c - ttygetc */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>


/*------------------------------------------------------------------------
 *  ttygetc - read one character from a tty device
 *------------------------------------------------------------------------
 */
int ttygetc( struct devsw *devptr )
{

    int ch;
    struct tty *iptr;
    sigset_t PS;
    disable( &PS );
    iptr = &tty[devptr->dvminor];
    wait( iptr->isem ); /* wait	for a character	in buff	*/
    ch = LOWBYTE & iptr->ibuff[iptr->itail++];
    if ( iptr->itail >= IBUFLEN )
        iptr->itail = 0;
    restore( &PS );
    return (ch );
}
