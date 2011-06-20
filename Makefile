CC = g++
CFLAGS = -Wall
LDFLAGS = -lopencv_highgui -lopencv_core -lopencv_calib3d


PROG = stereoCalib
HDRS = stereovision.h
SRCS = main.cpp stereovision.cpp

OBJS = $(SRCS:.c=.o)

$(PROG) : $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(PROG)

main.o : main.cpp
	$(CC) $(CFLAGS) $^ 

stereovision.o : stereovision.h stereovision.cpp
	$(CC) $(CFLAGS) $^ 