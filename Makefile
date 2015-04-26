Driver: Driver.o Enigma.o
	g++ -std=c++11 -o Driver Driver.o Enigma.o
Driver.o: Enigma.o Driver.cpp
	g++ -std=c++11 -c Driver.cpp
Enigma.o: Enigma.h Enigma.cpp
	g++ -std=c++11 -c Enigma.cpp
