#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include <exception>
#include "language.h"

#ifndef NDEBUG
#include <iostream>
using std::cerr;
using std::endl;
#endif

namespace Matrix {
	/**
	 * a general matrix exception,
	 * used as a base class for all the other matrix exceptions
	 */
	class MatrixException: std::exception {
		const char *message;
	protected: // prevent external instanciation
		MatrixException() : MatrixException ("") {}
		MatrixException (const char *message) : message (message) {}
	public:
		virtual const char *what() const override {
		    return message;
		}
	};

	class InvalidSize: MatrixException {
	public:
		InvalidSize() : MatrixException (_ ("Invalid matrix size")) {}
	};

	class OutOfBounds: MatrixException {
	public:
		OutOfBounds() : MatrixException (_ ("Index out of bounds")) {}
	};

	class SizeMismatch: MatrixException {
	public:
		SizeMismatch() : MatrixException (_ ("size mismatch")) {}
	};

	class NonSquareMatrix: MatrixException {
	public:
		NonSquareMatrix() : MatrixException (_ ("Non-square matrix")) {}
	};

	class NonRegularMatrix: MatrixException {
	public:
		NonRegularMatrix() : MatrixException (_ ("Non-regular matrix")) {}
	};

}

#endif

