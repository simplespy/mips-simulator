all : main.o
	mkdir bin && g++ main.o -o bin/final -std=c++11
main.o: main.cpp
	g++ -c main.cpp -std=c++11
clean:
	rm -rf *o bin
