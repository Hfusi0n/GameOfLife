#include "Board.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <list>

namespace Life {
	using std::max;
	using std::min;
	using std::ostream;
	using std::stringstream;
	using std::string;
	using std::pair;
	using std::list;

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
	 * const cell access
	 * @param p pair of (row, column)
	 * @return cell at row,column (const reference)
	 */
	const bool &Board::operator() (const pair<int, int> &p) const {
		return (*this) (p.first, p.second);
	}

	/**
	 * cell access
	 * @param p pair of (row,column)
	 * @return cell at row,column
	 */
	bool &Board::operator() (const pair<int, int> &p) {
		return (*this) (p.first, p.second);
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
		(*this) (r, c) = ! (*this) (r, c);
		return *this;
	}

	/**
	 * toggles the given cell
	 * @param p pair of coordinates (row, column)
	 * @return *this
	 */
	Board &Board::toggle (const pair<int, int> &p) {
		return toggle (p.first, p.second);
	}

	/**
	 * updates a list of coordinates
	 * @param l the list of coordinates
	 * @param toggle sets to true if true, toggles if false (default is true)
	 * @return *this
	 */
	Board &Board::updateList (const list<pair<int, int>> &l, const bool update) {
		for (auto & i: l) {
			if (update) {
				(*this) (i.first, i.second) = true;
			} else {
				toggle (i);
			}
		}
		return *this;
	}

	/**
	 * resets the board
	 * @return *this
	 */
	Board &Board::reset() {
		for (int i = 0; i < board.getHeight(); i++) {
			for (int j = 0; j < board.getWidth(); j++) {
				(*this) (i, j) = false;
			}
		}
		return *this;
	}

	/**
	 * returns the height of the board
	 * @return the height of the board
	 */
	int Board::getHeight() const {
		return board.getHeight();
	}

	/**
	 * returns the width of the board
	 * @return the width of the board
	 */
	int Board::getWidth() const {
		return board.getWidth();
	}

	/* external functions */
	ostream &operator<< (ostream &os, const Board &b) {
		stringstream s;
		char c;
		s << b.board;
		s.read (&c, 1);
		while (!s.eof()) {
			switch (c) {
			case '0':
				c = DEAD_CELL;
				break;
			case '1':
				c = LIVING_CELL;
				break;
			}
			os << c;
			s.read (&c, 1);
		}
		return os;
	}

}
