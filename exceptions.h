#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include <exception>

#ifndef NDEBUG
#include <iostream>
using std::cerr;
using std::endl;
#endif

namespace Matrix {
	class MatrixException: std::exception {
		const char *message;
	protected: // prevent external instanciation
		MatrixException() : MatrixException ("") {}
		MatrixException (const char *message) : message (message) {}

		virtual const char *what() const override {
		    return message;
		}
	};

	class InvalidSize: MatrixException {
	public:
		InvalidSize() : MatrixException ("Invalid matrix size") {}
	};
	class OutOfBounds: MatrixException {};
	class SizeMismatch: MatrixException {};
	class NonSquareMatrix: MatrixException {};
	class NonRegularMatrix: MatrixException {};

}

#endif
