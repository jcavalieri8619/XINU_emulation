/* io.h -  fgetc, fputc, getchar, isbaddev, putchar */

#ifndef _IO_H
#define _IO_H


struct intmap
{ /* device-to-interrupt routine mapping	*/
    void (*iin )( ); /* address of input interrupt routine	*/
    void* icode; /* argument passed to input routine	*/
    void (*iout )( ); /* address of output interrupt routine	*/
    void* ocode; /* argument passed to output routine	*/
};

#ifdef NDEVS
extern struct intmap intmap[NDEVS];
#define isbaddev(f) ( (f)<0 || (f)>=NDEVS )
#endif
#define BADDEV  -1

/* In-line I/O procedures */

#define getchar() getc(CONSOLE)
#define putchar(ch) putc(CONSOLE,(ch))
#define fgetc(unit) getc((unit))
#define fputc(unit,ch) putc((unit),(ch))




#endif