#include <iostream>
#include "Board.h"
using Life::Board;
using std::cout;
using std::endl;

int main (int argc, char **argv) {
	constexpr int height = 10;
	constexpr int width = 10;
	Board b (height, width);
	cout << b << endl;
	b (0, 1) = true;
	b (1, 2) = true;
	b (2, 2) = true;
	b (2, 1) = true;
	b (2, 0) = true;
	for (int i = 0; i < 10; i++) {
		cout << b << endl;
		b.step();
	}
	cout << b << endl;
	return 0;
}
