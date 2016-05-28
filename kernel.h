/* kernel.h - disable, enable, halt, restore, isodd, min */

/* Symbolic constants used throughout Xinu */

#ifndef _KERNEL_H
#define _KERNEL_H
#include <limits.h>
#include <stdlib.h>

//this is linux signal.h so that each function can
//define sigset_t for disable and restore signals
#include <signal.h>



//include stdlib.h for this marco incase linux system call fails
//like shmget, getcontext, sigprocmask etc
#define handle_error(msg) \
        do { /*perror(msg);*/ exit(879); } while (0)



#ifndef NPROC    /* set the number of processes	*/
#define NPROC  15  /*  allowed if not already done	*/
#endif



#define Bool  char
#define FALSE  0  /* Boolean constants		*/
#define TRUE  1

#define NULLCH  '\0'  /* The null character		*/
#define NULLSTR  ""  /* Pointer to empty string	*/
#define SYSCALL  int  /* System call declaration	*/
#define LOCAL  static  /* Local procedure declaration	*/
#define INTPROC  void  /* Interrupt procedure  "	*/
#define PROCESS  int  /* Process declaration		*/
#define RESCHYES 1  /* tell	ready to reschedule	*/
#define RESCHNO  0  /* tell	ready not to resch.	*/

#define MININT  0100000  /* minimum integer (-32768)	*/
#define MAXINT  0077777  /* maximum integer		*/
#define LOWBYTE  0377  /* mask for low-order 8 bits	*/
#define HIBYTE  0177400  /* mask for high 8 of 16 bits	*/
#define LOW16  0177777  /* mask for low-order 16 bits	*/
#define MINSTK  8192  /* minimum process stack size	*/
#define NULLSTK  8195  /* process 0 stack size		*/



#define MAGIC 0xDEADBEEF

/* Universal return constants */

#define OK   1  /* system call ok		*/
#define SYSERR  -1  /* system call failed		*/

/* Initialization constants */

#define INITARGC 1  /* initial process argc       	*/
#define INITSTK  100000  /* initial process stack	*/
#define INITPRIO 20  /* initial process priority	*/
#define INITNAME "main"  /* initial process name		*/
#define INITRET  userret  /* processes return address	*/
#define QUANTUM  10  /* clock ticks until preemption	*/

/* Miscellaneous utility inline functions */

#define isodd(x) (01&(int)(x))
#define min(a,b) ( (a) < (b) ? (a) : (b) )


extern ucontext_t context_init;
extern sigset_t DISABLE_INTERUPTS;
extern int rdyhead, rdytail;
extern int preempt;

#endif