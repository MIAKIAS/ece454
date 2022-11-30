#ifndef _util_h
#define _util_h

#define BOARD( __board, __i, __j )  (__board[LDA*(__i) + (__j)])

/**
 * C's mod ('%') operator is mathematically correct, but it may return
 * a negative remainder even when both inputs are nonnegative.  This
 * function always returns a nonnegative remainder (x mod m), as long
 * as x and m are both positive.  This is helpful for computing
 * toroidal boundary conditions.
 */
static inline int 
mod (int x, int m)
{
  return (x < 0) ? ((x % m) + m) : (x % m);
}

/**
 * Given neighbor count and current state, return zero if cell will be
 * dead, or nonzero if cell will be alive, in the next round.
 */
static inline char 
alivep (char count, char state)
{
  return (! state && (count == (char) 3)) ||
    (state && (count >= 2) && (count <= 3));
}


static inline void set_cell(char* board, const int row, const int col, const int nrows, const int ncols) {
	const int LDA = nrows;

	// Hard-wiring
	char* cell_ptr = &BOARD(board, row, col);
	int inorth, isouth, jwest, jeast;
	if (row == 0) {
		inorth = nrows * ncols - nrows;
	} else {
		inorth = -1 * nrows;
	}
	if (row == nrows - 1) {
		isouth = -1 * (nrows * ncols - nrows);
	} else {
		isouth = nrows;
	}
	if (col == 0) {
		jwest = ncols - 1;
	} else {
		jwest = -1;
	}
	if (col == ncols - 1) {
		jeast = -1 * (ncols - 1);
	} else {
		jeast = 1;
	}

	
	*cell_ptr |= 0x01;
	*(cell_ptr + jwest) += 2;
	*(cell_ptr + jeast) += 2;
	*(cell_ptr + inorth + jwest) += 2;
	*(cell_ptr + inorth) += 2;
	*(cell_ptr + inorth + jeast) += 2;
	*(cell_ptr + isouth + jwest) += 2;
	*(cell_ptr + isouth) += 2;
	*(cell_ptr + isouth + jeast) += 2;
}

static inline void unset_cell(char* board, const int row, const int col, const int nrows, const int ncols) {
	const int LDA = nrows;

	// Hard-wiring
	char* cell_ptr = &BOARD(board, row, col);
	int inorth, isouth, jwest, jeast;
	if (row == 0) {
		inorth = nrows * ncols - nrows;
	} else {
		inorth = -1 * nrows;
	}
	if (row == nrows - 1) {
		isouth = -1 * (nrows * ncols - nrows);
	} else {
		isouth = nrows;
	}
	if (col == 0) {
		jwest = ncols - 1;
	} else {
		jwest = -1;
	}
	if (col == ncols - 1) {
		jeast = -1 * (ncols - 1);
	} else {
		jeast = 1;
	}

	
	*cell_ptr &= ~0x01;
	*(cell_ptr + jwest) -= 2;
	*(cell_ptr + jeast) -= 2;
	*(cell_ptr + inorth + jwest) -= 2;
	*(cell_ptr + inorth) -= 2;
	*(cell_ptr + inorth + jeast) -= 2;
	*(cell_ptr + isouth + jwest) -= 2;
	*(cell_ptr + isouth) -= 2;
	*(cell_ptr + isouth + jeast) -= 2;
}

#endif /* _util_h */
