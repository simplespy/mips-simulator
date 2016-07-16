all : main.o
	g++ main.o -o final
main.o: main.cpp
	g++ -c main.cpp
clean:
	rm -f *o
