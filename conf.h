/* conf.h (GENERATED FILE; DO NOT EDIT) */

#ifndef CONF_H
#define CONF_H

#define NULLPTR NULL
#include <slu.h>
#include <kernel.h>
#include <tty.h>


/* Device table declarations */
struct devsw
{ /* device table entry */
    int dvnum;
    int (*dvinit )( );
    int (*dvopen )( );
    int (*dvclose )( );
    int (*dvread )( );
    int (*dvwrite )( );
    int (*dvseek )( );
    int (*dvgetc )( );
    int (*dvputc )( );
    int (*dvcntl )( );
    struct csr* dvcsr;
    int dvivec;
    int dvovec;
    void (*dviint )( );
    void (*dvoint )( );
    void *dvioblk;
    int dvminor;
};

extern struct devsw devtab[]; /* one entry per device */


/* Device name definitions */

#define CONSOLE     0   /* type tty      */
#define OTHER       1   /* type tty      */
#define RING0IN     2   /* type dlc      */
#define RING0OUT    3   /* type dlc      */
#define DISK0       4   /* type dsk      */
#define FILE1       5   /* type df       */
#define FILE2       6   /* type df       */
#define FILE3       7   /* type df       */
#define FILE4       8   /* type df       */

/* Control block sizes */

#define Ntty 1
//#define	Ndlc	2
//#define	Ndsk	1
//#define	Ndf	4

#define NDEVS 1

/* Declarations of I/O routines referenced */

extern int ttyinit( struct devsw *devptr );
extern int ttyopen( struct devsw *devptr, char *nam, char *mode );
extern int ionull( );
extern int ttyread( struct devsw *devptr, char *buff, int count );
extern int ttywrite( struct devsw *devptr, char *buff, int count );
extern int ioerr( );
extern int ttycntl( struct devsw *devptr, int func, char *addr );
extern int ttygetc( struct devsw *devptr );
extern int ttyputc( struct devsw *devptr, char ch );
extern INTPROC ttyiin( struct tty *iptr );
extern INTPROC ttyoin( struct tty *iptr );
//extern int dlcinit( );
//extern int dlcread( );
//extern int dlcwrite( );
//extern int dlccntl( );
//extern int dlcputc( );
//extern int dlciin( );
//extern int dlcoin( );
//extern int dsinit( );
//extern int dsopen( );
//extern int dsread( );
//extern int dswrite( );
//extern int dsseek( );
//extern int dscntl( );
//extern int dsinter( );
//extern int lfinit( );
//extern int lfclose( );
//extern int lfread( );
//extern int lfwrite( );
//extern int lfseek( );
//extern int lfgetc( );
//extern int lfputc( );

/* Configuration and Size Constants */


#define MEMMARK    /* define if memory marking used*/


#ifndef NPROC
#define NPROC   9     /* number of user processes	*/
#endif
#ifndef NSEM
#define NSEM     50   /* number of semaphores		*/
#endif

#endif // CONF_H

