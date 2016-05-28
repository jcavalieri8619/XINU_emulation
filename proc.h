/* proc.h - isbadpid */

#ifndef _PROC_H
#define _PROC_H

#include <ucontext.h>


/* process table declarations and defined constants			*/
#ifndef NPROC    /* set the number of processes	*/
#define NPROC  9  /*  allowed if not already done	*/
#endif

/* process state constants */

#define PRCURR  '\001'  /* process is currently running	*/
#define PRFREE  '\002'  /* process slot is free		*/
#define PRREADY  '\003'  /* process is on ready queue	*/
#define PRRECV  '\004'  /* process waiting for message	*/
#define PRSLEEP  '\005'  /* process is sleeping		*/
#define PRSUSP  '\006'  /* process is suspended		*/
#define PRWAIT  '\007'  /* process is on semaphore queue*/

/* miscellaneous process definitions */


#define PNMLEN  8  /* length of process "name"	*/
#define NULLPROC 0  /* id of the null process; it	*/
/*  is always eligible to run	*/
#define BADPID  -1  /* used when invalid pid needed	*/

#define isbadpid(x) (x<=0 || x>=NPROC)


/* process table entry */

struct pentry
{
    char pstate; /* process state: PRCURR, etc.	*/
    short pprio; /* process priority		*/
    short psem; /* semaphore if process waiting	*/
    short pmsg; /* message sent to this process	*/
    Bool phasmsg; /* True iff pmsg is valid	*/
    int* pbase; /* base of run time stack	*/
    int pstklen; /* stack length			*/
    int plimit; /* lowest extent of stack	*/
    char pname[PNMLEN]; /* process name			*/
    int pargs; /* initial number of arguments	*/
    void (*paddr )( ); /* initial code address		*/
    ucontext_t run_env;
    ucontext_t rtn_env;
};

extern struct pentry proctab[];
extern int numproc; /* currently active processes	*/
extern int nextproc; /* search point for free slot	*/
extern int currpid; /* currently executing process	*/


#endif