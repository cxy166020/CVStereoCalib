CC = g++
CFLAGS = -Wall -c
LDFLAGS = -lopencv_highgui -lopencv_core -lopencv_calib3d


PROG = stereoCalib
HDRS = stereovision.h CommandlineUtils.h
SRCS = main.cpp stereovision.cpp CommandlineUtils.cpp

OBJS = main.o stereovision.o CommandlineUtils.o

$(PROG) : $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(PROG)

main.o : main.cpp
	$(CC) $(CFLAGS) $^ 

stereovision.o : stereovision.h stereovision.cpp
	$(CC) $(CFLAGS) $^ 

CommandlineUtils.o : CommandlineUtils.h CommandlineUtils.cpp
	$(CC) $(CFLAGS) $^


clean:
	rm *.o $(PROG)