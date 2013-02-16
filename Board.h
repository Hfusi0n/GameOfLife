#ifndef _BOARD_H_
#define _BOARD_H_
#include "literals.h"
#include "matrix.h"
#include <iostream>
#include <utility>
#include <list>

// output conversion
#define LIVING_CELL '*'
#define DEAD_CELL ' '

// default rules
#define DEFAULT_BIRTH 000001000_b
#define DEFAULT_SURVIVAL 000001100_b

namespace Life {
	using Matrix::Matrix;
	using std::ostream;
	using std::pair;
	using std::list;

	class Board {
		Matrix<bool> board;

		// binary rules - the i-th binary bit represents i neighbors to apply
		unsigned int survival, birth;

		int countNeighbors (const int, const int);
	public:
		Board (const int, const unsigned int = DEFAULT_SURVIVAL, const unsigned int = DEFAULT_BIRTH);

		Board (const int, const int, const unsigned int = DEFAULT_SURVIVAL, const unsigned int = DEFAULT_BIRTH);

		~Board();

		const bool &operator() (const pair<int, int> &) const;

		bool &operator() (const pair<int, int> &);

		const bool &operator() (const int, const int) const;

		bool &operator() (const int, const int);

		Board &toggle (const int, const int);

		Board &toggle (const pair<int, int> &);

		Board &updateList (const std::list< std::pair< int, int > > &, const bool = true);

		Board &step();

		Board &reset();

		int getWidth() const;

		int getHeight() const;

		friend ostream &operator<< (ostream &, const Board &);
	};
}

#endif
