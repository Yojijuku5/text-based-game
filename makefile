CXX = g++

CXXFLAGS = -O0 -g3 -std=c++17

main: main.cpp game.h json.hpp
	$(CXX) $(CXXFLAGS) main.cpp -o main
