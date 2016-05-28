/* getmem.c - getmem */

#include <conf.h>
#include <kernel.h>
#include <mem.h>


/*------------------------------------------------------------------------
 * getmem  --  allocate heap storage, returning lowest integer address
 *------------------------------------------------------------------------
 */
SYSCALL *getmem( unsigned nbytes )
{

    struct mblock *p, *q, *leftover;
    sigset_t PS;


    disable( &PS );
    if ( nbytes == 0 || memlist.mnext == NULL ) {
        restore( &PS );
        return ( (int *) SYSERR );
    }
    nbytes = (unsigned) roundew( nbytes );
    for ( q = &memlist, p = memlist.mnext; p != NULL; q = p, p = p->mnext )
        if ( p->mlen == nbytes ) {
            q->mnext = p->mnext;
            restore( &PS );
            return ( (int *) p );
        } else if ( p->mlen > nbytes ) {
            leftover = (struct mblock *) ( (unsigned) p + nbytes );
            q->mnext = leftover;
            leftover->mnext = p->mnext;
            leftover->mlen = p->mlen - nbytes;
            restore( &PS );
            return ( (int *) p );
        }
    restore( &PS );
    return ( (int *) SYSERR );
}
