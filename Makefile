CC = g++

CFLAGS = -g -Wall

OPENGLFLAGS = -lglut -lGL -lGLU -lsfml-audio -lsfml-system

all: rm main
	
main:
	
	$(CC) $(CFLAGS) Main.cpp $(OPENGLFLAGS) -o main
	rm -f Main.o
	./main
rm: 
	 rm -f ./main