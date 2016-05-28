/* insert.c  -  insert */

#include <conf.h>
#include <kernel.h>
#include <q.h>


/*------------------------------------------------------------------------
 * insert.c  --  insert an process into a q list in key order
 *------------------------------------------------------------------------
 */
int insert( int proc, /* process to insert            */
            int head, /* q index of head of list      */
            int key ) /* key to use for this process  */
{
    int next; /* runs through list		*/
    int prev;

    next = q[head].qnext;
    while ( q[next].qkey < key ) /* tail has MAXINT as key	*/
        next = q[next].qnext;
    q[proc].qnext = next;
    q[proc].qprev = prev = q[next].qprev;
    q[proc].qkey = key;
    q[prev].qnext = proc;
    q[next].qprev = proc;
    return (OK );
}
