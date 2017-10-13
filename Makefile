SOURCE = LineSweeper.cpp Graph.cpp
OBJS = $(SOURCE:.cpp=.o)
EXECUTABLE = LineSweeper
CC = g++
LIBS = 
OPTFLAGS = -O3
CFLAGS = $(OPTFLAGS)
LDFLAGS = 
.PHONY : all

all : $(EXECUTABLE)
	
$(EXECUTABLE) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $< 

clean : 
	rm -f $(OBJS) $(EXECUTABLE)

LineSweeper.o : Graph.o
