OUTPUT = gameoflife
CXX = g++
DEBUG = -g
CXXFLAGS = -std=c++11 -Werror -Wall -pedantic-errors $(DEBUG)
BUILDDIR=build/

$(OUTPUT): Board.o main.o literals.o
	mkdir -p $(BUILDDIR)
	$(CXX) $^ -o $(BUILDDIR)/$@

Board.o: Board.cpp Board.h literals.h matrix.h exceptions.h language.h
	$(CXX) $(CXXFLAGS) -c $^
literals.o: literals.cpp literals.h
	$(CXX) $(CXXFLAGS) -c $^
main.o: main.cpp Board.h literals.h matrix.h exceptions.h language.h
	$(CXX) $(CXXFLAGS) -c $^

clean_o:
	rm -f *.o
clean_gch:
	rm -f *.gch
clean: clean_o clean_gch
	rm $(BUILDDIR)/$(OUTPUT)
