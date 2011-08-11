CC = g++
CFLAGS = -Wall -c -O2
LDFLAGS = -lopencv_highgui -lopencv_core -lopencv_calib3d


PROG = stereoCalib
UDIS = undistort
SCAL = singleCalib
HDRS = stereovision.h CommandlineUtils.h
SRCS = main.cpp stereovision.cpp CommandlineUtils.cpp

OBJS = main.o stereovision.o CommandlineUtils.o
UOBJ = undistort.o stereovision.o CommandlineUtils.o

PROJECT : $(PROG) $(UDIS)

$(UDIS) : $(UOBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(PROG) : $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

undistort.o : undistort.cpp
	$(CC) $(CFLAGS) $^

main.o : main.cpp
	$(CC) $(CFLAGS) $^ 

stereovision.o : stereovision.h stereovision.cpp
	$(CC) $(CFLAGS) $^ 

CommandlineUtils.o : CommandlineUtils.h CommandlineUtils.cpp
	$(CC) $(CFLAGS) $^


clean:
	rm *.o $(PROG) $(UDIS)