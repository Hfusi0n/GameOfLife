#include "Board.h"
#include <algorithm>
#include <iostream>

namespace Life {
	using std::max;
	using std::min;
	using std::ostream;

	/**
	 * builds a square board
	 * @param size size of the board
	 */
	Board::Board (const int size) : Board (size, size) {
	}

	/**
	 * builds a w*h board
	 * @param h height
	 * @param w width
	 */
	Board::Board (const int h, const int w) : board (h, w) {
	}

	Board::~Board() {
	}

	/**
	 * const cell access
	 * @param r row
	 * @param c column
	 * @return cell at r,c (const)
	 */
	const bool &Board::operator() (const int r, const int c) const {
		return board (r, c);
	}

	/**
	 * cell access
	 * @param r row
	 * @param c column
	 * @return cell at r,c
	 */
	bool &Board::operator() (const int r, const int c) {
		return board (r, c);
	}

	/**
	 * performs a single step
	 * @return a reference to the board after the step
	 */
	Board &Board::step() {
		Matrix<bool> result (board);
		for (int i = 0; i < board.getHeight(); i++) {
			for (int j = 0; j < board.getWidth(); j++) {
				int neighbors = countNeighbors (i, j);
				switch (neighbors) {
				case 2:
					break;
				case 3:
					if (!board (i, j)) {
						result (i, j) = true;
					}
					break;
				default:
					result (i, j) = false;
				}
			}
		}
		board = result;
		return *this;
	}

	/**
	 * counts number of neighbors of the given cell
	 * @param r row
	 * @param c column
	 * @return the number of neighbors
	 */
	int Board::countNeighbors (const int r, const int c) {
		int count = 0;
		for (int i = max (r - 1, 0); i <= min (r + 1, board.getHeight() - 1); i++) {
			for (int j = max (c - 1, 0); j <= min (c + 1, board.getWidth() - 1); j++) {
				if ( (i == r) && (j == c)) {
					continue;
				}
				if (board (i, j)) {
					count++;
				}
			}
		}
		return count;
	}

	/**
	 * toggles the given coordinates
	 * @param r row
	 * @param c column
	 * @return *this
	 */
	Board &Board::toggle (const int r, const int c) {
		*this (r, c) = !*this (r, c);
		return *this;
	}


	ostream &operator<< (ostream &os, const Board &b) {
		os << b.board;
		return os;
	}

}
