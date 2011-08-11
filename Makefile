CC = g++
CFLAGS = -Wall -c -O2
LDFLAGS = -lopencv_highgui -lopencv_core -lopencv_calib3d

PROG = stereoCalib
UDIS = undistort
TGPU = toGPU

CSTV = stereovision.h stereovision.cpp
CMMD = CommandlineUtils.h CommandlineUtils.cpp

MAIN = main.cpp
MGPU = ToGPU.cpp
MUDI = undistort.cpp

OBJS = main.o stereovision.o CommandlineUtils.o
UOBJ = undistort.o stereovision.o CommandlineUtils.o
TOBJ = ToGPU.o stereovision.o CommandlineUtils.o

PROJECT : $(PROG) $(UDIS) $(TGPU)

$(PROG) : $(OBJS) 
	$(CC) $(LDFLAGS) $^ -o $@

$(UDIS) : $(UOBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(TGPU) : $(TOBJ)
	$(CC) $(LDFLAGS) $^ -o $@

undistort.o : undistort.cpp
	$(CC) $(CFLAGS) $^

main.o : main.cpp
	$(CC) $(CFLAGS) $^

stereovision.o : stereovision.h stereovision.cpp
	$(CC) $(CFLAGS) $^

CommandlineUtils.o : CommandlineUtils.h CommandlineUtils.cpp
	$(CC) $(CFLAGS) $^

ToGPU.o : ToGPU.cpp
	$(CC) $(CFLAGS) $^


clean:
	rm *.o $(PROG) $(UDIS) $(TGPU)