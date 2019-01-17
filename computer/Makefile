CXX=g++ 
VPATH= auxiliary/
RM=rm -f

# TODO: Add CPP flag varable here that implements multithreading for the main program
CXXFLAGS=-I auxiliary/

# make
all: train main

train: train.o
	$(CXX) train.o -o train
main: main.o
	$(CXX) main.o -o main

# Create class objects here
train.o: train.cpp
	$(CXX) -c train.cpp
main.o: main.cpp duplex.h
	$(CXX) -c main.cpp

# make clean
clean:
	$(RM) *.o main train