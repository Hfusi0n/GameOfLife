#include <iostream>
#include <utility>
#include <list>
#include "Board.h"
using Life::Board;
using std::cout;
using std::endl;
using std::pair;
using std::list;

int main (int argc, char **argv) {
	constexpr int height = 11;
	constexpr int width = 38;
	Board b (height, width);
	cout << b << endl;

	list<pair<int, int>> pattern = {
		{5 , 1}, {5, 2}, {6, 1}, {6, 2}
		, {3, 36}, {4, 36}, {3, 35}, {4, 35}
		, {5, 11}, {6, 11}, {7, 11}, {4, 12}, {8, 12}, {3, 13}, {3, 14}, {9, 13}, {9, 14}
		, {6, 15}, {4, 16}, {8, 16}, {5, 17}, {6, 17}, {7, 17}, {6, 18}
		, {3, 21}, {4, 21}, {5, 21}, {3, 22}, {4, 22}, {5, 22}, {2, 23}, {6, 23}
		, {1, 25}, {2, 25}, {6, 25}, {7, 25}
	};
	b.updateList(pattern);

	for (int i = 0; i < 10; i++) {
		cout << b << endl;
		b.step();
	}
	cout << b << endl;
	return 0;
}
