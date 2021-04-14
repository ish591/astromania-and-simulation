all:
	g++ -std=c++11 src/main.cpp -o main -I include -l SDL2-2.0.0
	./main