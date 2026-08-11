CXX = g++
CXXFLAGS = -std=c++11 -O2

all: game level obstacle

game: game.cpp
	$(CXX) $(CXXFLAGS) game.cpp -o game.out

level: level.cpp
	$(CXX) $(CXXFLAGS) level.cpp -o level.out

obstacle: obstacle.cpp
	$(CXX) $(CXXFLAGS) obstacle.cpp -o obstacle.out

clean:
	rm -f *.out *.exe
