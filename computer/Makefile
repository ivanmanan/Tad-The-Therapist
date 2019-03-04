CXX=g++ 
VPATH= auxiliary/
RM=rm -f

# TODO: Add CPP flag varable here that implements multithreading for the main program
CXXFLAGS=-I auxiliary/

# make
all: chat train main

chat: chat.o
	$(CXX) chat.o -o chat
trial: trial.o
	$(CXX) trial.o -o trial
train: train.o
	$(CXX) train.o -o train
main: main.o
	$(CXX) main.o -o main

# Create class objects here
chat.o: chat.cpp chatbot.h
	$(CXX) -c chat.cpp
trial.o: trial.cpp duplex.h ml.h
	$(CXX) -c trial.cpp
train.o: train.cpp duplex.h ml.h
	$(CXX) -c train.cpp
main.o: main.cpp duplex.h conversation.h chatbot.h ml.h
	$(CXX) -c main.cpp

# make clean
clean:
	$(RM) *.o chat main train