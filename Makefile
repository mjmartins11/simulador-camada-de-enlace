all: simulador

simulador: simulador.o
	g++ simulador.o -o simulador

simulador.o: simulador.cpp
	g++ simulador.cpp -c