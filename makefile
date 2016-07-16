all : main.o
	mkdir bin && g++ main.o -o bin/final
main.o: main.cpp
	g++ -c main.cpp
clean:
	rm -rf *o final bin
