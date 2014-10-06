CC = gcc
CFLAGS = -O2 -Wall
INC = 
LIBDIR  = .
LIB = $(LIBDIR)/libcsapp.a

all: poly_test rowcol_test

libcsapp.a: clock.o fcyc.o cpe.o lsquare.o
	ar rcs libcsapp.a clock.o fcyc.o cpe.o lsquare.o

poly_test: poly_test.c poly.c poly.h $(LIB)
	$(CC) $(CFLAGS) -o poly_test $(INC) poly_test.c poly.c $(LIB)

poly.s: poly.c
	$(CC) $(CFLAGS) $(INC) -S poly.c

rowcol_test: rowcol_test.c rowcol.c rowcol.h $(LIB)
	$(CC) $(CFLAGS) -o rowcol_test rowcol_test.c rowcol.c $(LIB)

rowcol.s: rowcol.c rowcol.h
	$(CC) $(CFLAGS) -S rowcol.c

clock.o: clock.c clock.h
	$(CC) $(CFLAGS) -c clock.c

fcyc.o: fcyc.c fcyc.h
	$(CC) $(CFLAGS) -c fcyc.c

lsquare.o: lsquare.c lsquare.h
	$(CC) $(CFLAGS) -c lsquare.c

cpe.o: cpe.c cpe.h
	$(CC) $(CFLAGS) -c cpe.c

clean:
	rm -f *.o *.a *~ *_test *.exe


