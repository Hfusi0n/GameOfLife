#ifndef _BOARD_H_
#define _BOARD_H_
#include "matrix.h"

namespace Life {
	using Matrix::Matrix;

	class Board {
		Matrix<bool> board;

		int countNeighbors (const int, const int);
	public:
		Board (const int);

		Board (const int, const int);

		~Board();

		const bool &operator() (const int, const int) const;

		bool &operator() (const int, const int);

		Board &step();
	};
}

#endif
