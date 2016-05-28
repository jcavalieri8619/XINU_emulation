/* freemem.c - freemem */

#include <conf.h>
#include <kernel.h>
#include <mem.h>


/*------------------------------------------------------------------------
 *  freemem  --  free a memory block, returning it to memlist
 *------------------------------------------------------------------------
 */
SYSCALL freemem( struct mblock *block, unsigned size )
{

    struct mblock *p, *q;
    unsigned top;
    sigset_t PS;


    if ( size == 0 || (unsigned) block > (unsigned) maxaddr
         || ( (unsigned) block )<( (unsigned) minaddr ) )
        return (SYSERR );
    size = (unsigned) roundew( size );
    disable( &PS );

    for ( p = memlist.mnext, q = &memlist; p != NULL && p < block;
          q = p, p = p->mnext )
        ;

    if ( ( top = q->mlen + (unsigned) q )>(unsigned) block && q != &memlist ||
         p != NULL && ( size + (unsigned) block ) > (unsigned) p ) {
        restore( &PS );
        return (SYSERR );
    }
    if ( q != &memlist && top == (unsigned) block )
        q->mlen += size;
    else {
        block->mlen = size;
        block->mnext = p;
        q->mnext = block;
        q = block;
    }
    if ( (unsigned) ( q->mlen + (unsigned) q ) == (unsigned) p ) {
        q->mlen += p->mlen;
        q->mnext = p->mnext;
    }

    memlist.mlen += size;
    restore( &PS );
    return (OK );
}
