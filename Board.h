#ifndef _BOARD_H_
#define _BOARD_H_
#include "matrix.h"

namespace Life {
	using Matrix::Matrix;

	class Board {
		Matrix<bool> board;
	public:
		/**
		 * builds a square board
		 * @param size size of the board
		 */
		Board (const int);

		/**
		 * builds a w*h board
		 * @param h height
		 * @param w width
		 */
		Board (const int, const int);

		~Board();

		/**
		 * const cell access
		 * @param r row
		 * @param c column
		 * @return cell at r,c (const)
		 */
		const bool &operator() (const int, const int) const;

		/**
		 * cell access
		 * @param r row
		 * @param c column
		 * @return cell at r,c
		 */
		bool &operator() (const int, const int);
	};
}

#endif
