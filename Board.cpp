#include "Board.h"

using namespace Life;
using Matrix::Matrix;

Board::Board (const int size) : Board (size, size) {
}

Board::Board (const int h, const int w) : board (h, w) {
}

const bool &Board::operator() (const int r, const int c) const {
	return board (r, c);
}

bool &Board::operator() (const int r, const int c) {
	return board (r, c);
}
