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
	const int inorth = mod (row-1, nrows);
	const int isouth = mod (row+1, nrows);
	const int jwest = mod (col-1, ncols);
	const int jeast = mod (col+1, ncols);

	BOARD (board, row, col) |= 0x01;
	BOARD (board, inorth, jwest) += 2; 
	BOARD (board, inorth, col) += 2;
	BOARD (board, inorth, jeast) += 2;
	BOARD (board, row, jwest) += 2;
	BOARD (board, row, jeast) += 2;
	BOARD (board, isouth, jwest) += 2;
	BOARD (board, isouth, col) += 2;
	BOARD (board, isouth, jeast) += 2;
}

static inline void unset_cell(char* board, const int row, const int col, const int nrows, const int ncols) {
	const int LDA = nrows;
	const int inorth = mod (row-1, nrows);
	const int isouth = mod (row+1, nrows);
	const int jwest = mod (col-1, ncols);
	const int jeast = mod (col+1, ncols);

	BOARD (board, row, col) &= ~0x01;
	BOARD (board, inorth, jwest) -= 2;
	BOARD (board, inorth, col) -= 2;
	BOARD (board, inorth, jeast) -= 2;
	BOARD (board, row, jwest) -= 2;
	BOARD (board, row, jeast) -= 2;
	BOARD (board, isouth, jwest) -= 2;
	BOARD (board, isouth, col) -= 2;
	BOARD (board, isouth, jeast) -= 2;
}

#endif /* _util_h */
