/* getstk.c - getstk */


#include <kernel.h>
#include <mem.h>


/*------------------------------------------------------------------------
 * getstk  --  allocate stack memory, returning address of topmost int
 *------------------------------------------------------------------------
 */
SYSCALL *getstk( unsigned int nbytes )
{

    struct mblock *p, *q; /* q follows p along memlis  */
    struct mblock *fits, *fitsq;
    unsigned len;
    sigset_t PS;
    disable( &PS );
    if ( nbytes == 0 ) {
        restore( &PS );
        handle_error( "getstk: " );
        return ( (int *) SYSERR );
    }
    nbytes = (unsigned) roundew( nbytes );
    fits = NULL;
    q = &memlist;


    for ( p = q->mnext; p != NULL; q = p, p = p->mnext )
        if ( p->mlen >= nbytes ) {
            fitsq = q;
            fits = p;
        }
    if ( fits == NULL ) {
        restore( &PS );
        handle_error( "getstk: " );
        return ( (int *) SYSERR );
    }
    if ( nbytes == ( len = fits->mlen ) ) {
        fitsq->mnext = fits->mnext;
    } else {
        fits->mlen -= nbytes;
    }
    fits = (struct mblock *) ( ( (unsigned) fits ) + len - sizeof (int ) );
    *( (unsigned *) fits ) = MAGIC; /* put magic at base */
    restore( &PS );
    return ( (int *) fits );
}
