#ifndef _util_h
#define _util_h
#include <omp.h>
#include <stdatomic.h>
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

static inline void init_cell(char* board, const int i, const int nrows, const int ncols, int* changes, unsigned int* index) {
	// Hard-wiring
	char* cell_ptr = board + i;

	int inorth, isouth, jwest, jeast;
	// Within first row
	if (i < ncols) {
		inorth = nrows * ncols - nrows;
	} else {
		inorth = -1 * nrows;
	}
	// Within last row
	if (i >= nrows * ncols - ncols) {
		isouth = -1 * (nrows * ncols - nrows);
	} else {
		isouth = nrows;
	}
	// Within first column
	if (i % ncols == 0) {
		jwest = ncols - 1;
	} else {
		jwest = -1;
	}
	// Within last column
	if (i % ncols == ncols - 1) {
		jeast = -1 * (ncols - 1);
	} else {
		jeast = 1;
	}

	*cell_ptr |= 0x10;
	cell_ptr[jwest]++;
	cell_ptr[jeast]++;
	cell_ptr[inorth+jwest]++;
	cell_ptr[inorth]++;
	cell_ptr[inorth+jeast]++;
	cell_ptr[isouth+jwest]++;
	cell_ptr[isouth]++;
	cell_ptr[isouth+jeast]++;

	changes[(*index)++] = i;
}

static inline void set_cell(char* board, const int i, const int nrows, const int ncols, int* changes, unsigned int* index) {
	// const int LDA = nrows;
	// Hard-wiring
	atomic_char* cell_ptr = (atomic_char*)board + i;
	char state = atomic_fetch_or(board + i, (char)0x10);
	state &= 0x10;
	if (state) {
		return;
	}

	atomic_uint* atomic_index = (atomic_uint*)index;
	int inorth, isouth, jwest, jeast;
	// Within first row
	if (i < ncols) {
		inorth = nrows * ncols - nrows;
	} else {
		inorth = -1 * nrows;
	}
	// Within last row
	if (i >= nrows * ncols - ncols) {
		isouth = -1 * (nrows * ncols - nrows);
	} else {
		isouth = nrows;
	}
	// Within first column
	if (i % ncols == 0) {
		jwest = ncols - 1;
	} else {
		jwest = -1;
	}
	// Within last column
	if (i % ncols == ncols - 1) {
		jeast = -1 * (ncols - 1);
	} else {
		jeast = 1;
	}

	cell_ptr[jwest]++;
	cell_ptr[jeast]++;
	cell_ptr[inorth+jwest]++;
	cell_ptr[inorth]++;
	cell_ptr[inorth+jeast]++;
	cell_ptr[isouth+jwest]++;
	cell_ptr[isouth]++;
	cell_ptr[isouth+jeast]++;

	changes[(*atomic_index)++] = i;
}

static inline void unset_cell(char* board, const int i, const int nrows, const int ncols, int* changes, unsigned int* index) {
	// const int LDA = nrows;

	// Hard-wiring
	atomic_char* cell_ptr = (atomic_char*)board + i;
	char state = atomic_fetch_and(board + i, (char)~0x10);
	state &= 0x10;
	if (!state) {
		return;
	}

	atomic_uint* atomic_index = (atomic_uint*)index;
	int inorth, isouth, jwest, jeast;
	// Within first row
	if (i < ncols) {
		inorth = nrows * ncols - nrows;
	} else {
		inorth = -1 * nrows;
	}
	// Within last row
	if (i >= nrows * ncols - ncols) {
		isouth = -1 * (nrows * ncols - nrows);
	} else {
		isouth = nrows;
	}
	// Within first column
	if (i % ncols == 0) {
		jwest = ncols - 1;
	} else {
		jwest = -1;
	}
	// Within last column
	if (i % ncols == ncols - 1) {
		jeast = -1 * (ncols - 1);
	} else {
		jeast = 1;
	}

	cell_ptr[jwest]--;
	cell_ptr[jeast]--;
	cell_ptr[inorth+jwest]--;
	cell_ptr[inorth]--;
	cell_ptr[inorth+jeast]--;
	cell_ptr[isouth+jwest]--;
	cell_ptr[isouth]--;
	cell_ptr[isouth+jeast]--;

	changes[(*atomic_index)++] = i;

}

#endif /* _util_h */
