CC = gcc
CFLAGS = -m32 -DRTCLOCK  -g -c 
CXX_CFLAGS = -m32 -I.
LDFLAGS= -m32
SOURCES = user_program.c siginterupts_init.c chprio.c clkinit.c clkint.c create.c ctxsw.c \
freemem.c getitem.c getmem.c memory_init.c startup.c scount.c semsignaln.c sreset.c\
getpid.c getprio.c getstk.c initialize.c insert.c insertd.c kill.c newqueue.c queue.c \
ready.c receive.c recvclr.c resched.c resume.c screate.c sdelete.c send.c semsignal.c \
sleep10.c sleep.c ssclock.c suspend.c userret.c semwait.c wakeup.c CLI_funcs.c lex.yy.c \
close.c conf.c control.c getc.c init.c ioerr.c ioinit.c ionull.c open.c \
putc.c read.c seek.c ttycntl.c ttygetc.c ttyiin.c ttyinit.c ttyoin.c ttyopen.c ttyputc.c \
ttyread.c ttywrite.c write.c 
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = XINU


all: $(SOURCES) $(EXECUTABLE)

	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@ $(CXX_CFLAGS)
clean:
	rm -f *.o $(EXECUTABLE)

depend:
	makedepend --$(SOURCES) -I.
