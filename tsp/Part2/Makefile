CC = g++
CPPFLAGS = -g -std=c++11 -Wall -O2 
LDFLAGS =

# MATPLOT = -std=c++11  -I/usr/include/python2.7 -lpython2.7

OBJ = GenOp.o CostrSol.o ../DataGenerator.o ../Solution.o main.o GenAlg.o


%.o: %.cpp
		$(CC) $(CPPFLAGS) -c $^ -o $@ 

main: $(OBJ)
		$(CC) $(CPPFLAGS) $(OBJ) -o main 

clean:
		rm -rf $(OBJ) main

.PHONY: clean
