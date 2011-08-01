CC = g++
CFLAGS = -Wall -c
LDFLAGS = -lopencv_highgui -lopencv_core -lopencv_calib3d


PROG = stereoCalib
RECT = rect
SCAL = singleCalib
HDRS = stereovision.h CommandlineUtils.h
SRCS = main.cpp stereovision.cpp CommandlineUtils.cpp

OBJS = main.o stereovision.o CommandlineUtils.o
ROBJ = rectify.o stereovision.o CommandlineUtils.o
SOBJ = singleCalib.o CommandlineUtils.o

PROJECT : $(PROG) $(RECT) $(SCAL)

$(RECT) : $(ROBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(SCAL) : $(SOBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(PROG) : $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

singleCalib.o : singleCalib.cpp
	$(CC) $(CFLAGS) $^

rectify.o : rectify.cpp
	$(CC) $(CFLAGS) $^

main.o : main.cpp
	$(CC) $(CFLAGS) $^ 

stereovision.o : stereovision.h stereovision.cpp
	$(CC) $(CFLAGS) $^ 

CommandlineUtils.o : CommandlineUtils.h CommandlineUtils.cpp
	$(CC) $(CFLAGS) $^


clean:
	rm *.o $(PROG) $(RECT)