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
	 * @brief builds a square board
	 * @param size size of the board
	 **/
	Board::Board (const int size, const unsigned int survival,
	              const unsigned int birth) : Board (size, size, survival, birth) {
	}

	/**
	 * @brief builds a w*h board
	 * @param h height
	 * @param w width
	 **/
	Board::Board (const int h, const int w, const unsigned int survival,
	              const unsigned int birth) : board (h, w), survival (survival), birth (birth) {
	}

	Board::~Board() {
	}

	/**
	 * @brief const cell access
	 * @param r row
	 * @param c column
	 * @return cell at r,c (const)
	 **/
	const bool &Board::operator() (const int r, const int c) const {
		return board (r, c);
	}

	/**
	 * @brief cell access
	 * @param r row
	 * @param c column
	 * @return cell at r,c
	 **/
	bool &Board::operator() (const int r, const int c) {
		return board (r, c);
	}

	/**
	 * @brief const cell access
	 * @param p pair of (row, column)
	 * @return cell at row,column (const reference)
	 **/
	const bool &Board::operator() (const pair<int, int> &p) const {
		return (*this) (p.first, p.second);
	}

	/**
	 * @brief cell access
	 * @param p pair of (row,column)
	 * @return cell at row,column
	 **/
	bool &Board::operator() (const pair<int, int> &p) {
		return (*this) (p.first, p.second);
	}

	/**
	 * @brief performs a single step
	 * @return a reference to the board after the step
	 **/
	Board &Board::step() {
		Matrix<bool> result (board);
		for (int i = 0; i < board.getHeight(); i++) {
			for (int j = 0; j < board.getWidth(); j++) {
				int neighbors = countNeighbors (i, j);
				unsigned int mask = 1;
				while (neighbors > 0) {
					mask *= 2;
					neighbors--;
				}
				if (board (i, j)) {
					if ( (mask & survival) == 0) {
						result (i, j) = false;
					}
				} else {
					if ( (mask & birth) != 0) {
						result (i, j) = true;
					}
				}
			}
		}
		board = result;
		return *this;
	}

	/**
	 * @brief counts number of neighbors of the given cell
	 * @param r row
	 * @param c column
	 * @return the number of neighbors
	 **/
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
	 * @brief toggles the given coordinates
	 * @param r row
	 * @param c column
	 * @return *this
	 **/
	Board &Board::toggle (const int r, const int c) {
		(*this) (r, c) = ! (*this) (r, c);
		return *this;
	}

	/**
	 * @brief toggles the given cell
	 * @param p pair of coordinates (row, column)
	 * @return *this
	 **/
	Board &Board::toggle (const pair<int, int> &p) {
		return toggle (p.first, p.second);
	}

	/**
	 * @brief updates a list of coordinates
	 * @param l the list of coordinates
	 * @param toggle sets to true if true, toggles if false (default is true)
	 * @return *this
	 **/
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
	 * @brief resets the board
	 * @return *this
	 **/
	Board &Board::reset() {
		for (int i = 0; i < board.getHeight(); i++) {
			for (int j = 0; j < board.getWidth(); j++) {
				(*this) (i, j) = false;
			}
		}
		return *this;
	}

	/**
	 * @brief returns the height of the board
	 * @return the height of the board
	 **/
	int Board::getHeight() const {
		return board.getHeight();
	}

	/**
	 * @brief returns the width of the board
	 * @return the width of the board
	 **/
	int Board::getWidth() const {
		return board.getWidth();
	}

	/* external functions **/
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
