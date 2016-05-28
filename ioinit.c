/* ioinit.c - ioinit, iosetvec */

#include <conf.h>
#include <kernel.h>
#include <io.h>

extern struct sigaction sigIOinterupts;
extern void IOdispatcher( int signum );


/*------------------------------------------------------------------------
 *  ioinit --  standard interrupt vector and dispatch initialization
 *------------------------------------------------------------------------
 */
int ioinit( int descrp )
{
    int minor;

    if ( isbaddev( descrp ) )
        return (SYSERR );
    minor = devtab[descrp].dvminor;
    iosetvec( descrp, minor, minor );
    return (OK );
}


/*------------------------------------------------------------------------
 *  iosetvec  -  fill in interrupt vectors and dispatch table entries
 *------------------------------------------------------------------------
 */
int iosetvec( int descrp, void* incode, void* outcode )
{
    struct devsw *devptr;
    struct intmap *map;


    if ( isbaddev( descrp ) )
        return (SYSERR );
    devptr = &devtab[descrp];
    map = &intmap[devptr->dvnum]; /* fill in interrupt dispatch	*/
    map->iin = devptr->dviint; /*   map with addresses of high-*/
    map->icode = incode; /*   level input and output	*/
    map->iout = devptr->dvoint; /*   interrupt handlers and	*/
    map->ocode = outcode; /*   minor device numbers	*/

    sigIOinterupts.sa_handler = IOdispatcher;
    sigIOinterupts.sa_mask = DISABLE_INTERUPTS;
    sigIOinterupts.sa_flags = SA_RESTART;


    if ( sigaction( devtab[CONSOLE].dvivec, &sigIOinterupts,
                    (struct sigaction *) NULL ) == -1 ) {
        handle_error( "sigaction: " );
    }


    if ( sigaction( devtab[CONSOLE].dvovec, &sigIOinterupts,
                    (struct sigaction *) NULL ) == -1 ) {
        handle_error( "sigaction: " );
    }



    return (OK );
}
