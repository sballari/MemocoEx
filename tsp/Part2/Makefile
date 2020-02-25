CC = g++
CPPFLAGS = -g -Wall -O2 
LDFLAGS =

CPX_BASE    = /opt/ibm/ILOG/CPLEX_Studio129
CPX_INCDIR  = $(CPX_BASE)/cplex/include
CPX_LIBDIR  = $(CPX_BASE)/cplex/lib/x86-64_linux/static_pic
CPX_LDFLAGS = -std=c++11 -lcplex -lm -pthread -ldl
# MATPLOT = -std=c++11  -I/usr/include/python2.7 -lpython2.7

OBJ = GenOp.o CostrSol.o ../DataGenerator.o ../Solution.o main.o GenAlg.o


%.o: %.cpp
		$(CC) $(CPPFLAGS) -I$(CPX_INCDIR) -c $^ -o $@ 

main: $(OBJ)
		$(CC) $(CPPFLAGS) $(OBJ) -o main -L$(CPX_LIBDIR) $(CPX_LDFLAGS) 

clean:
		rm -rf $(OBJ) main

.PHONY: clean
