#ifndef _BOARD_H_
#define _BOARD_H_
#include "matrix.h"
#include <iostream>
#include <utility>

namespace Life {
	using Matrix::Matrix;
	using std::ostream;
	using std::pair;

	class Board {
		Matrix<bool> board;

		int countNeighbors (const int, const int);
	public:
		Board (const int);

		Board (const int, const int);

		~Board();

		const bool &operator() (const pair<int, int> &) const;

		bool &operator() (const pair<int, int> &);

		const bool &operator() (const int, const int) const;

		bool &operator() (const int, const int);

		Board &toggle (const int, const int);

		Board &toggle (const pair<int, int> &);

		Board &step();

		friend ostream &operator<< (ostream &, const Board &);
	};
}

#endif
