SOURCE = LineSweeper.cpp AntSystem.cpp Graph.cpp
OBJS = $(SOURCE:.cpp=.o)
EXECUTABLE = LineSweeper
CC = g++
LIBS = 
OPTFLAGS = -O3
CFLAGS = $(OPTFLAGS) -std=c++11
LDFLAGS = 
.PHONY : all

all : $(EXECUTABLE)
	
$(EXECUTABLE) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $< 

clean : 
	rm -f $(OBJS) $(EXECUTABLE)

LineSweeper.o : AntSystem.o

AntSystem.o : AntSystem.cpp Graph.o

Graph.o : Graph.cpp Graph.h
