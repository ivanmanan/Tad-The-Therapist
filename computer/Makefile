CXX=g++
VPATH= auxiliary/
RM=rm -f

# make
all: chat train main trial

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
main.o: main.cpp duplex.h chatbot.h ml.h
	$(CXX) -c main.cpp

# make clean
clean:
	$(RM) *.o chat main train trial