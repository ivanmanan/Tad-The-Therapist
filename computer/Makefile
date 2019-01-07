# make
all: train main

train: train.o
	$(CXX) train.o -o train
main: main.o
	$(CXX) main.o -o main

# Create class objects here
train.o: train.cpp
	$(CXX) -c train.cpp
main.o: main.cpp
	$(CXX) -c main.cpp

# make clean
clean:
	rm -f *.o main train