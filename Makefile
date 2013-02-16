OUTPUT = gameoflife
CXX = g++
DEBUG = -g
CXXFLAGS = -std=c++11 -Werror -Wall -pedantic-errors $(DEBUG)
BUILDDIR=build/

$(OUTPUT): Board.o matrix.o main.o
	mkdir -p $(BUILDDIR)
	$(CXX) $^ -o $(BUILDDIR)/$@

Board.o: Board.cpp Board.h matrix.h exceptions.h language.h
	$(CXX) $(CXXFLAGS) -c $^
main.o: main.cpp Board.h matrix.h exceptions.h language.h
	$(CXX) $(CXXFLAGS) -c $^
matrix.o: matrix.cpp
	$(CXX) $(CXXFLAGS) -c $^

clean_o:
	rm -f *.o
clean_gch:
	rm -f *.gch
clean: clean_o clean_gch
	rm $(BUILDDIR)/$(OUTPUT)
