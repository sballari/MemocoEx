CC = g++
CPPFLAGS = -g -Wall -O2 
LDFLAGS =

CPX_BASE1    = /opt/ibm/ILOG/CPLEX_Studio1210
CPX_BASE    = /home/sballari/Desktop/CPLEX_Studio129
CPX_INCDIR  = $(CPX_BASE)/cplex/include
CPX_LIBDIR  = $(CPX_BASE)/cplex/lib/x86-64_linux/static_pic
CPX_LDFLAGS = -lcplex -lm -pthread -ldl
MATPLOT = -std=c++11  -I/usr/include/python2.7 -lpython2.7

OBJ = main.o ../DataGenerator.o model1.o

%.o: %.cpp
		$(CC) $(CPPFLAGS) -I$(CPX_INCDIR) -c $^ -o $@ $(MATPLOT)

main: $(OBJ)
		$(CC) $(CPPFLAGS) $(OBJ) -o main -L$(CPX_LIBDIR) $(CPX_LDFLAGS) $(MATPLOT)

clean:
		rm -rf $(OBJ) main

.PHONY: clean
