#ifndef _MATRIX_H
#define _MATRIX_H

#include <iostream>
#include <algorithm>
#include <limits>
#include <assert.h>
#include "exceptions.h"
using std::ostream;
using std::endl;
using std::numeric_limits;

#ifndef NDEBUG
using std::cerr;
#endif

namespace Matrix {
	/**
	 * requirements from T:
	 * T(0), T(1) (assign add itentity & multiply identity)
	 * ==, !=
	 * +=
	 * *=
	 * + (arithmetic)
	 * - (arithmetic)
	 * - (negation)
	 * * (multiply)
	 */
	template<class T> class Matrix {
		// zero and one constants (for a little bit of efficiency)
		T const zero = T (0);
		T const one = T (1);

		T **matrix;
		int height, width;

		bool checkRow (const int row) const {
			return (0 <= row && row < height);
		}

		bool checkColumn (const int col) const {
			return (0 <= col && col < width);
		}

		/**
		 * workaround - gets the item at (i,j)
		 */
		T &at (const int row, const int col) const {
			if (!checkColumn (col) || !checkRow (row)) {
				throw OutOfBounds();
			}
			return matrix[row][col];
		}

		/**
		 * assigns the given matrix to the current one.
		 * assignment of a 0*0 matrix:
		 * if reset == true: resets the matrix to 0*0
		 * if reset == false: nothing
		 * @param m the matrix to assign
		 * @param resize will resize the matrix if true (and reset its contents!)
		 * @param offsetHeight the offset height to assign
		 * @param offsetWidth the offset width to assign
		 */
		void assign (const Matrix &m, const bool reset = true, const int offsetHeight = 0, const int offsetWidth = 0) {
			if (reset && (width != m.width || height != m.height)) {
				resize (m.height, m.width);
			} else {
				if (m.width == 0 && m.height == 0) {
					return;
				}
				if (!checkRow (offsetHeight) || !checkColumn (offsetWidth)) {
					throw OutOfBounds();
				}
			}
			for (int i = offsetHeight; i < height; i++) {
				for (int j = offsetWidth; j < width; j++) {
					at (i, j) = m (i, j);
				}
			}
		}

		/**
		 * resizes the matrix to a new size (removes the old one)
		 * if the new size is 0x0, just deallocates everything
		 * @param newHeight the new height
		 * @param newWidth the new width
		 */
		void resize (const int newHeight, const int newWidth) {
			if (newHeight < 0 || newWidth < 0) {
				throw InvalidSize();
			}
			assert ( (newHeight == 0 && newWidth == 0) || (newHeight > 0 && newWidth > 0));
			if ( (newHeight == 0 || newWidth == 0) && newHeight + newWidth != 0) {
				throw InvalidSize();
			}
			if (getWidth() != 0 || getHeight() != 0) {
				for (int i = 0; i < getHeight(); i++) {
					delete[] matrix[i];
				}
				delete[] matrix;
				width = 0;
				height = 0;
			}
			if (newHeight != 0 && newWidth != 0) {
				matrix = new T*[newHeight];
				for (int i = 0; i < newHeight; i++) {
					matrix[i] = new T[newWidth];
					for (int j = 0; j < newWidth; j++) {
						matrix[i][j] = 0;
					}
				}
				height = newHeight;
				width = newWidth;
			}
		}

		static bool isEqual (const T &value1, const T &value2) {
			auto difference = abs (value1 - value2);
			auto epsilon = numeric_limits<decltype (difference) >::epsilon();
			return difference < epsilon;
		}
	public:
		/**
		 * creates a new h*w matrix
		 * @param height
		 * @param width
		 */
		Matrix (const int height, const int width) : height (0), width (0) {
			resize (height, width);
		}

		/**
		 * creates a size*size square matrix
		 * @param size the size of the matrix
		 */
		Matrix (const int size) : Matrix (size, size) {}

		/**
		 * copy constructor
		 * @param m the matrix to copy
		 */
		Matrix (const Matrix<T> &m) : Matrix (m.height, m.width) {
			assign (m);
		}

		/**
		 * default c-tor - creates a withered (0*0) matrix
		 */
		Matrix() : Matrix (0) {}

		~Matrix() {
			resize (0, 0);
		}

		int getWidth() const {
			return this->width;
		}

		int getHeight() const {
			return this->height;
		}

		/**
		 * returns a transposed copy of the given matrix
		 * @return a transposed version of the current matrix
		 */
		Matrix transpose() const {
			Matrix<T> ret (width, height);
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					ret (j, i) = at (i, j);
				}
			}
			return ret;
		}

		/**
		 * gets row vector
		 * @param row the row
		 * @return the row vector
		 */
		Matrix getRow (const int row) const {
			if (!checkRow (row)) {
				throw OutOfBounds();
			}
			Matrix<T> ret (width, 1);
			for (int i = 0; i < width; i++) {
				ret (i, 0) = this->at (row, i);
			}
			return ret;
		}

		/**
		 * gets column vector
		 * @param col the column
		 * @return the column vector
		 */
		Matrix getColumn (const int col) const {
			if (!checkColumn (col)) {
				throw OutOfBounds();
			}
			Matrix<T> ret (height, 1);
			for (int i = 0; i < height; i++) {
				ret (i, 0) = this->at (i, col);
			}
			return ret;
		}

		/**
		 * checks if the matrix is square
		 * @return true if square matrix
		 */
		bool isSquare() const {
			return (width == height);
		}

		/**
		 * gets a minor of the matrix
		 * @param row row to remove
		 * @param col column to remove
		 * @return minor of the matrix
		 */
		Matrix getMinor (const int row, const int col) const {
			if (!isSquare()) {
				throw NonSquareMatrix();
			}
			if (!checkColumn (col) || !checkRow (row)) {
				throw OutOfBounds();
			}
			Matrix<T> ret (height - 1, width - 1);
			int targetI, targetJ;
			targetI = targetJ = 0;
			for (int i = 0; i < height; i++) {
				if (i == row) {
					continue;
				}
				targetJ = 0;
				for (int j = 0; j < width; j++) {
					if (j == col) {
						continue;
					}
					ret (targetI, targetJ) = at (i, j);
					targetJ++;
				}
				targetI++;
			}
			return ret;
		}

		/**
		 * returns the determinant of the matrix
		 * @return the determinant
		 */
		T det() const {
			if (!isSquare()) {
				throw NonSquareMatrix();
			}
			int swapCount = 0;
			Matrix<T> dummy;
			Matrix<T> triangular = gaussianElimination (swapCount, dummy);
			T product = one;
			for (int i = 0; i < width; i++) {
				product *= triangular (i, i);
				if (isEqual (product, zero)) {
					break;
				}
			}
			if (swapCount % 2 == 1) {
				product = -product;
			}
			return product;
		}

		/**
		 * gets the inverse of the matrix
		 * @return the inverse of the matrix
		 */
		Matrix inverse() const {
			if (!isSquare()) {
				throw NonSquareMatrix();
			}
			Matrix<T> ret;
			gaussianElimination (ret, true);
			if (ret.width == 0) {
				throw NonRegularMatrix();
			}
			return ret;
		}

		/**
		 * returns the trace of the matrix
		 * @return the trace
		 */
		T trace() const {
			if (!isSquare()) {
				throw NonSquareMatrix();
			}
			T sum = 0;
			for (int i = 0; i < height; i++) {
				sum += at (i, i);
			}
			return sum;
		}

		/**
		 * swaps two rows of the matrix
		 * @param r1 row 1
		 * @param r2 row 2
		 * @return the matrix with the swapped rows
		 */
		Matrix rowSwap (const int r1, const int r2) const {
			if (!checkRow (r1) || !checkRow (r2)) {
				throw OutOfBounds();
			}
			Matrix<T> ret = *this;
			if (r1 != r2) {
				for (int i = 0; i < ret.width; i++) {
					std::swap (ret (r1, i), ret (r2, i));
				}
			}
			return ret;
		}

		/**
		 * multiplies a row by the given scalar
		 * @param r the row
		 * @param s the scalar
		 * @return the matrix with row r multiplied by s
		 */
		Matrix rowMultiply (const int r, const T &s) const {
			return rowAddMultiply (r, r, s - one);
		}

		/**
		 * adds a row with a multiplication of another row
		 * @param destination the destination row
		 * @param source the source row
		 * @param s the scalar to multiply by. default is 1 (T must support this)
		 * @return the matrix with a row added by a multiplication of another row
		 */
		Matrix rowAddMultiply (const int destination, const int source, const T &s = 1) const {
			if (!checkRow (destination) || !checkRow (source)) {
				throw OutOfBounds();
			}
			Matrix<T> ret = *this;
			for (int i = 0; i < ret.width; i++) {
				if (!isEqual (ret (source, i), zero) && !isEqual (s, zero)) {
					ret (destination, i) += ret (source, i) * s;
				}
				if (isEqual (ret (destination, i), zero)) {
					ret (destination, i) = zero;
				}
			}
			return ret;
		}

		/**
		 * adds a column with a multiplication of another column
		 * @param destination the destination column
		 * @param source the source column
		 * @param s the scalar to multiply by. default is 1 (T must support this)
		 * @return the matrix with a column added by a multiplication of another column
		 */
		Matrix colAddMultiply (const int destination, const int source, const T &s) const {
			return transpose().rowAddMultiply (destination, source, s).transpose();
		}

		/**
		 * multiplies a column by the given scalar
		 * @param c the column
		 * @param s the scalar
		 * @return the matrix with column c multiplied by s
		 */
		Matrix colMultiply (const int c, const T &s) const {
			return colAddMultiply (c, c, s - 1);
		}

		/**
		 * swaps two columns
		 * @param c1 the first column
		 * @param c2 the second column
		 * @return the matrix with the two columns swapped
		 */
		Matrix colSwap (int c1, int c2) const {
			return transpose().rowSwap (c1, c2).transpose();
		}

		/**
		 * returns the matrix powered by r
		 * works only for square matrices, throws NonSquareMatrix otherwise
		 * if M is singular, throws NonRegularMatrix for r <= 0
		 * @param r the exponent
		 * @return M^r
		 */
		Matrix power (int r) const {
			if (!isSquare()) {
				throw NonSquareMatrix();
			}
			if (r <= 0 && rank() < width) {
				throw NonRegularMatrix();
			}
			Matrix<T> multiplier;
			Matrix<T> ret = unitMatrix (width);
			if (r <= 0) {
				multiplier = inverse(); // will throw NonRegularMatrix on failure
				r = -r;
			} else {
				multiplier = *this;
			}
			while (r > 0) {
				if (ret == zero) {
					break;
				}
				ret *= multiplier;
				r--;
			}
			return ret;
		}

		/**
		 * returns the matrix after gaussian elimination
		 * @param canonical returns a full canonical form if true, just a triangular form otherwise
		 * @return the matrix after gaussian elimination
		 */
		Matrix gaussianElimination (const bool canonical = false) const {
			Matrix<T> b; // another workaround
			return gaussianElimination (b, canonical);
		}

		/**
		 * returns the matrix after gaussian elimination
		 * @param matrix the matrix which will become inversed (if there's an inverse)
		 * @param canonical returns a full canonical form if true, just a triangular form otherwise
		 * @return the matrix after gaussian elimination
		 */
		Matrix gaussianElimination (Matrix &inverse, const bool canonical = false) const {
			int a; // workaround
			return gaussianElimination (a, inverse, canonical);
		}

		/**
		 * returns the matrix after gaussian elimination
		 * @param swapCount counts how many swaps were during the elimination (by reference)
		 * @param inverse will become the inverse of the matrix (by reference)
		 * 	if non-square - not touched. if singular or non-canonical - undefined matrix
		 * @param canonical returns a full canonical form if true, just a triangular form otherwise
		 * @return the matrix after gaussian elimination
		 */
		Matrix gaussianElimination (int &swapCount, Matrix &inverse, const bool canonical = false) const {
			Matrix<T> ret = *this;
			bool hasInverse = false;
			int rows = std::min<int> (ret.width, ret.height);
			swapCount = 0;
			if (canonical && ret.isSquare()) {
				inverse = scalarMatrix (ret.width, one);
				hasInverse = true;
			}
			for (int i = 0; i < rows; i++) {
				int j;
				T b;
				T a = ret (i, i);
				// if zero in diagonal, find non-zero in column and swap
				if (isEqual (a, zero)) {
					for (j = i + 1; j < ret.height; j++) {
						if (!isEqual (ret (j, i), zero)) {
							ret = ret.rowSwap (i, j);
							if (hasInverse) {
								inverse = inverse.rowSwap (i, j);
							}
							swapCount++;
							break;
						}
					}
					a = ret (i, i);
				}
				// if still zero in diagonal, look for non-zero in row and swap
				// furthermore, we now know the matrix isn't regular,
				// therefore the inverse matrix will be destroyed
				if (isEqual (a, zero)) {
					inverse = Matrix (0);
					hasInverse = false;
					for (j = i + 1; j < ret.width; j++) {
						b = ret (i, j);
						if (!isEqual (b, zero)) {
							ret = ret.colSwap (i, j);
							if (hasInverse) {
								inverse = inverse.colSwap (i, j);
							}
						}
					}
					a = ret (i, i);
				}
				// if non-zero, eliminate below triangle
				if (!isEqual (a, zero)) {
					for (j = i + 1; j < ret.height; j++) {
						b = ret (j, i);
						if (!isEqual (b, zero)) {
							ret = ret.rowAddMultiply (j, i, -b / a);
							if (hasInverse) {
								inverse = inverse.rowAddMultiply (j, i, -b / a);
							}
						}
					}
					// if we want canonical form, eliminate above diagonal
					if (canonical) {
						ret = ret.rowMultiply (i, one / a);
						if (hasInverse) {
							inverse = inverse.rowMultiply (i, one / a);
						}
						for (j = 0; j < i; j++) {
							b = ret (j, i);
							if (!isEqual (b, zero)) {
								ret = ret.rowAddMultiply (j, i, -b);
								if (hasInverse) {
									inverse = inverse.rowAddMultiply (j, i, -b);
								}
							}
						}
					}
				}
			}
			return ret;
		}

		/**
		 * returns the rank of the matrix
		 * @return rank of the matrix
		 */
		int rank() const {
			auto triangled = gaussianElimination();
			int ret = std::min (width, height);
			int size = ret;
			for (int i = 0; i < size; i++) {
				if (isEqual (triangled (i, i), zero)) {
					ret--;
				}
			}
			return ret;
		}

		/* static functions */
		/**
		 * returns the unit matrix
		 * @param size the size of the unit matrix
		 * @return I(n*n)
		 */
		static Matrix unitMatrix (const int size) {
			return scalarMatrix (size, 1);
		}

		/**
		 * returns a scalar matrix
		 * @param size size
		 * @param s the scalar
		 * @return sI
		 */
		static Matrix scalarMatrix (const int size, const T &s) {
			Matrix<T> ret (size);
			for (int i = 0; i < ret.getWidth(); i++) {
				ret (i, i) = s;
			}
			return ret;
		}

		/**
		 * returns a jordan block
		 * @param size the size of the block
		 * @param s the value of the diagonal
		 * @return the jordan block
		 */
		static Matrix jordanBlock (const int size, const T &s) {
			Matrix<T> ret = scalarMatrix (size, s);
			for (int i = 1; i < ret.getWidth(); i++) {
				ret (i - 1, i) = 1;
			}
			return ret;
		}

		/**
		 * returns a full matrix (a matrix in which all the fields have the same value
		 * @param height the height of the matrix
		 * @param width the width of the matrix
		 * @param s the value of each cell
		 * @return a full matrix with the given size and value
		 */
		static Matrix fullMatrix (const int height, const int width, const T &s) {
			Matrix<T> ret (height, width);
			for (int i = 0; i < ret.getHeight(); i++) {
				for (int j = 0; j < ret.getWidth(); j++) {
					ret (i, j) = s;
				}
			}
			return ret;
		}

		/* operators */
		/**
		 * gets the value at (i,j)
		 * @param row row
		 * @param col column
		 * @return the value at (i,j)
		 */
		const T &operator() (const int row, const int col) const {
			return at (row, col);
		}
		T &operator() (const int row, const int col) {
			return at (row, col);
		}

		/**
		 * matrix comparison
		 * @param m the matrix to compare
		 * @return true if they're equal
		 */
		bool operator== (const Matrix &m) const {
			if (this == &m) { // reflexive relation
				return true;
			}
			if (width != m.width || height != m.height) {
				throw SizeMismatch();
			}
			bool ret = true;
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					if (this->at (i, j) != m (i, j)) {
						ret = false;
						break;
					}
				}
			}
			return ret;
		}

		/**
		 * matrix !=
		 * @param m the matrix to compare with
		 * @return true if not equal
		 */
		bool operator!= (const Matrix &m) const {
			return ! (*this == m);
		}

		Matrix &operator= (const Matrix &m) {
			if (this == &m) {
				return *this;
			}
			assign (m);
			return *this;
		}

		/**
		 * matrix +=
		 */
		Matrix &operator+= (const Matrix &m) {
			if (getWidth() != m.getWidth() || getHeight() != m.getHeight()) {
				throw SizeMismatch();
			}
			for (int i = 0; i < getHeight(); i++) {
				for (int j = 0; j < getWidth(); j++) {
					at (i, j) += m (i, j);
				}
			}
			return *this;
		}

		/**
		 * matrix -=
		 */
		Matrix &operator-= (const Matrix &m) {
			*this += -m;
			return *this;
		}

		/**
		 * scalar *=
		 */
		Matrix &operator*= (const T &s) {
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					at (i, j) *= s;
				}
			}
			return *this;
		}

		/**
		 * matrix *=
		 * will throw exception on size mismatch
		 * @param m the matrix to multiply with
		 * @return this*m
		 */
		Matrix &operator*= (const Matrix &m) {
			*this = *this * m;
			return *this;
		}

		/**
		 * returns the negative of the matrix
		 * @return -matrix
		 */
		Matrix operator-() const {
			Matrix<T> ret (height, width);
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					ret (i, j) = -at (i, j);
				}
			}
			return ret;
		}
	};

	/* class stuff prototypes */

	/**
	 * matrix substraction
	 * @param m1 matrix 1
	 * @param m2 matrix 2
	 * @return m1-m2
	 */
	template<class T> Matrix<T> operator- (const Matrix<T> &, const Matrix<T> &);

	/**
	 * matrix-scalar substraction
	 * @param m the matrix
	 * @param s the scalar
	 * @return m-sI
	 */
	template<class T> Matrix<T> operator- (const Matrix<T> &, const T &);

	/**
	 * scalar-matrix substraction
	 * @param s the scalar
	 * @param m the matrix
	 * @return sI-m
	 */
	template<class T> Matrix<T> operator- (const T &, const Matrix<T> &);

	/**
	 * matrix-scalar comparison
	 * works only for square matrices
	 * @param m the matrix
	 * @param s the scalar to compare with
	 * @return m == sI
	 */
	template<class T> bool operator== (const Matrix<T> &, const T &);

	/**
	 * matrix-scalar comparison
	 * works only for square matrices
	 * @param m the matrix
	 * @param s the scalar to compare with
	 * @return sI == m
	 */
	template<class T> bool operator== (const T &, const Matrix<T> &);

	/**
	 * matrix-scalar !=
	 * @param m the matrix
	 * @param s the scalar
	 * @return m != sI
	 */
	template<class T> bool operator!= (const Matrix<T> &, const T &);

	/**
	 * matrix-scalar !=
	 * @param s the scalar
	 * @param m the matrix
	 * @return sI != m
	 */
	template<class T> bool operator!= (const T &, const Matrix<T> &);

	/**
	 * outputs a matrix
	 * @param fd the output stream
	 * @param m the matrix to print
	 * @return reference to fd
	 */
	template<class T> ostream &operator<< (ostream &, const Matrix<T> &);

	/**
	 * outputs a matrix
	 * @param fd the output stream
	 * @param m the matrix to print
	 * @return reference to fd
	 */
	template<class T> ostream &operator<< (ostream &, const Matrix<T> &);

	/**
	 * matrix-scalar multiplication
	 * @param s the scalar
	 * @param m the matrix
	 * @return m*sI
	 */
	template<class T> Matrix<T> &operator* (const Matrix<T> &, const T &);

	/**
	 * matrix-scalar multiplication
	 * @param m the matrix
	 * @param s the scalar
	 * @return sI*m
	 */
	template<class T> Matrix<T> &operator* (const T &, const Matrix<T> &);

	/**
	 * matrix multiplication
	 * @param m1 matrix 1
	 * @param m2 matrix 2
	 * @return the multiplication of the two matrices
	 */
	template<class T> Matrix<T> operator* (const Matrix<T> &, const Matrix<T> &);

	/**
	 * matrix addition
	 * @param m1 matrix 1
	 * @param m2 matrix 2
	 * @return m1+m2
	 */
	template<class T> Matrix<T> operator+ (const Matrix<T> &, const Matrix<T> &);

	/**
	 * scalar matrix addition
	 * @param m matrix
	 * @param s scalar
	 * @return m+sI
	 */
	template<class T> Matrix<T> operator+ (const Matrix<T> &, const T &);

	/**
	 * scalar matrix addition
	 * @param s scalar
	 * @param m matrix
	 * @return sI+m
	 */
	template<class T> Matrix<T> operator+ (const T &, const Matrix<T> &);

	template<class T> Matrix<T> operator- (const Matrix<T> &m1, const Matrix<T> &m2) {
		Matrix<T> ret = m1;
		ret -= m2;
		return ret;
	}

	template<class T> Matrix<T> operator- (const Matrix<T> &m, const T &s) {
		return m + (-Matrix<T>::fullMatrix (m.getHeight(), m.getWidth(), s));
	}

	template<class T> Matrix<T> operator- (const T &s, const Matrix<T> &m) {
		return -m + s;
	}

	template<class T> bool operator== (const Matrix<T> &m, const T &s) {
		return m == Matrix<T>::scalarMatrix (m.getWidth(), s);
	}

	template<class T> bool operator== (const T &s, const Matrix<T> &m) {
		return m == s;
	}

	template<class T> bool operator!= (const Matrix<T> &m, const T &s) {
		return ! (m == s);
	}

	template<class T> bool operator!= (const T &s, const Matrix<T> &m) {
		return (m != s);
	}

	template<class T> ostream &operator<< (ostream &fd, const Matrix<T> &m) {
		for (int i = 0; i < m.getHeight(); i++) {
			for (int j = 0; j < m.getWidth(); j++) {
				fd << m (i, j) << " ";
			}
			fd << endl;
		}
		return fd;
	}

	template<class T> Matrix<T> &operator* (const Matrix<T> &m, const T &s) {
		Matrix<T> ret = m;
		return ret *= s;
	}

	template<class T> Matrix<T> &operator* (const T &s, const Matrix<T> &m) {
		Matrix<T> ret = m;
		return ret *= s;
	}

	template<class T> Matrix<T> operator* (const Matrix<T> &m1, const Matrix<T> &m2) {
		if (m1.getWidth() != m2.getHeight()) {
			throw SizeMismatch();
		}
		int height = m1.getHeight();
		int width = m2.getWidth();
		Matrix<T> ret (height, width);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				T sum = 0;
				for (int k = 0; k < m1.getWidth(); k++) {
					sum += m1 (i, k) * m2 (k, j);
				}
				ret (i, j) = sum;
			}
		}
		return ret;
	}

	template<class T> Matrix<T> operator+ (const Matrix<T> &m1, const Matrix<T> &m2) {
		Matrix<T> ret = m1;
		ret += m2;
		return ret;
	}

	template<class T> Matrix<T> operator+ (const Matrix<T> &m, const T &s) {
		return Matrix<T>::fullMatrix (m.getHeight(), m.getWidth(), s) + m;
	}

	template<class T> Matrix<T> operator+ (const T &s, const Matrix<T> &m) {
		return m + s;
	}

}

#endif
