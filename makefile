all: Program

rebuild: clean Program

Program: main.o findFile.a
	g++ -std=c++17 -pthread -lm -o Program main.o -L. -lfindFile

main.o: main.cpp
	g++ -std=c++17 -pthread -O -c main.cpp

findmanager.o: findmanager.cpp findmanager.h
	g++ -std=c++17 -pthread -O -c findmanager.cpp
	
findFile.a: findmanager.o
	ar rcs libfindFile.a findmanager.o
	
libs: findFile.a

clean:
	rm -f myProgram *.o *.a *.gch
