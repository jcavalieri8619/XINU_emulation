/* sleep.h */
#ifndef _SLEEP_H
#define _SLEEP_H


extern int clkruns; /* 1 iff clock exists; 0 otherwise	*/
/* Set at system startup.		*/
extern int clockq; /* q index of sleeping process list	*/
extern int count6; /* used to ignore 5 of 6 interrupts	*/
extern int *sltop; /* address of first key on clockq	*/
extern int slnempty; /* 1 iff clockq is nonempty		*/

extern int defclk; /* >0 iff clock interrupts are deferred	*/
extern int clkdiff; /* number of clock ticks deferred	*/
extern void clkint( int signum ); /* clock interrupt handler	*/

#endif