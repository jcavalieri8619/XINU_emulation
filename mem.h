/* mem.h - freestk, roundew, truncew */

#ifndef _MEM_H
#define _MEM_H
/*----------------------------------------------------------------------
 * roundew, truncew - round or truncate address to next even word
 *----------------------------------------------------------------------
 */
#define roundew(x) (int )( (7 + (int)(x)) & (~7) )
#define truncew(x) (int )( ((int)(x)) & (~7) )

/*----------------------------------------------------------------------
 *  freestk  --  free stack memory allocated by getstk
 *----------------------------------------------------------------------
 */
#define freestk(p,len) freemem((unsigned)(p)   \
                                - (unsigned)(roundew(len)) \
                                + (unsigned)sizeof(int), \
                                roundew(len) )

struct mblock
{
    struct mblock *mnext;
    unsigned mlen;
};
extern struct mblock memlist; /* head of free memory list	*/
extern int *maxaddr; /* max memory address		*/
extern int *minaddr; /* address beyond loaded memory	*/
extern int *getstk( ); /* declaration			*/
extern int *getmem( ); /* declaration                  */

#endif