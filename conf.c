/* conf.c (GENERATED FILE; DO NOT EDIT) */

#include <conf.h>
#include <signal.h>

/* device independent I/O switch */

struct devsw dev = { };

struct devsw devtab[NDEVS] = {


                              0,
                              ttyinit, ttyopen, ionull,
                              ttyread, ttywrite, ioerr,
                              ttygetc, ttyputc, ttycntl,
                              NULLPTR, SIGUSR1, SIGUSR2,
                              ttyiin, ttyoin, NULLPTR, 0,

};


/*  Format of entries is:
device-number,
init, open, close,
read, write, seek,
getc, putc, cntl,
device-csr-address, input-vector, output-vector,
iint-handler, oint-handler, control-block, minor-device,
 */

/*  CONSOLE  is tty  */