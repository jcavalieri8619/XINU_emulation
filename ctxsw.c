/* ctxsw.c - ctxsw */
#include <kernel.h>
#include <ucontext.h>


/*------------------------------------------------------------------------
/* ctxsw  --  actually perform context switch, saving/loading registers
/*------------------------------------------------------------------------
 */


void ctxsw( ucontext_t * out, ucontext_t * in )
{
    swapcontext( out, in );

    return;
}
