CC = g++

CFLAGS = -g -Wall

OPENGLFLAGS = -lglut -lGL -lGLU 

all: rm main
	
main:
	
	$(CC) $(CFLAGS) Main.cpp $(OPENGLFLAGS) -o main
	rm -f Main.o
	./main
rm: 
	 rm -f ./main