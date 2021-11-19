C	= gcc
CFLAGS	= -g -Wall
RLINE	= -lreadline
RTHREAD = -lpthread
RMATH	= -lm

#---------------------------------------------

ep2: ep2.c lista.o
	$(C) -o ep2 ep2.c lista.o $(RTHREAD) $(RMATH)

lista.o: lista.c
	$(C) $(CFLAGS) -c lista.c

clean:
	$(RM) *~ *.o