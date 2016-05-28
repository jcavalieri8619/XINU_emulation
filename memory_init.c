#include <kernel.h>
#include <mem.h>
#include <stdlib.h>

const unsigned int RAMSIZE = 90500000;
int *maxaddr; /* max memory address		*/
int *minaddr;


int meminit( )
{

    int * ram;

    if ( !( ram = (int*) calloc( RAMSIZE, sizeof (int ) ) ) ) {
        return SYSERR;
    }

    minaddr = &ram[0];
    maxaddr = &ram[RAMSIZE - 1];



}
