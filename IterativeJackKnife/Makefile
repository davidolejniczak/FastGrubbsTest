CC = clang
CPP = clang++ 
CFLAGS = -Wall -Wextra -pedantic
CPPFLAGS = -Wall -Wextra -pedantic -std=c++17

CSRC = mainFunctions.c 
CPPSRC = main.CPP
OBJ = mainFunctions.o main.o 
TARGET = IterativeJackKnife

all: $(TARGET)

#if file : changed files -> run 
mainFunctions.o: mainFunctions.c mainFunctions.h 
	$(CC) $(CFLAGS) -c mainFunctions.c -o mainFunctions.o

main.o: main.cpp mainFunctions.h
	$(CPP) $(CPPFLAGS) -c main.cpp -o main.o 

$(TARGET): $(OBJ) 
	$(CPP) $(OBJ) -o $(TARGET)

run: 
	./$(TARGET)

clean: 
	rm -f $(OBJ) $(TARGET)