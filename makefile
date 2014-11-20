CC=clang++
CFLAGS=-std=c++11 -Wall 
INC=
LDFLAGS=
SRCS= canvas.cpp coordinate.cpp line.cpp matrix.cpp shapes.cpp colour.cpp image.cpp main.cpp pixel.cpp
OBJS= canvas.o coordinate.o line.o matrix.o shapes.o colour.o image.o main.o pixel.o
PROG= main
$(PROG): $(OBJS) 
	$(CC) $(CFLAGS) $(DFLAGS) -o $(PROG) $(OBJS) $(LDFLAGS)
.cpp.o:
	$(CC) $(CFLAGS) $(DFLAGS) -c $*.cpp 
clean:
	rm $(OBJS) $(PROG)
depend:
	makedepend -- $(CFLAGS) -- $(SRCS)
# DO NOT DELETE

canvas.o: canvas.h coordinate.h matrix.h shapes.h line.h colour.h pixel.h
canvas.o: image.h
coordinate.o: coordinate.h matrix.h
line.o: line.h colour.h pixel.h coordinate.h matrix.h
matrix.o: matrix.h
shapes.o: shapes.h coordinate.h matrix.h line.h colour.h pixel.h
colour.o: colour.h
image.o: image.h colour.h
main.o: polylib.h canvas.h coordinate.h matrix.h shapes.h line.h colour.h
main.o: pixel.h image.h
pixel.o: pixel.h colour.h coordinate.h matrix.h
