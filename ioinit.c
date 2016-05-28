/* ioinit.c - ioinit, iosetvec */

#include <conf.h>
#include <kernel.h>
#include <io.h>


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
int iosetvec( int descrp, int incode, int outcode )
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

    return (OK );
}




/*
    vptr = (struct vector *) devptr->dvivec;
    vptr->vproc = (char *) INTVECI;
    vptr->vps = descrp | DISABLE;
    vptr = (struct vector *) devptr->dvovec;
    vptr->vproc = (char *) INTVECO;
    vptr->vps = descrp | DISABLE;
 */