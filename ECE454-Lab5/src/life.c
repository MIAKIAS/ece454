/*****************************************************************************
 * life.c
 * Parallelized and optimized implementation of the game of life resides here
 ****************************************************************************/
#include "life.h"
#include "util.h"
#include <string.h>
#include <stdio.h>

/*****************************************************************************
 * Helper function definitions
 ****************************************************************************/
void print_grid(char* board, const int nrows, const int ncols) {
	// const int LDA = nrows;
	fprintf (stderr,"\n=======================================\n");
	for (int i = 0; i < nrows * ncols; i++) {
		if (i%32 == 0){
			fprintf (stderr,"\n");
		}
		if (board[i] & 0x01) {
			fprintf (stderr," 1 ");
		} else {
			fprintf (stderr," 0 ");
		}
	}
}
/*****************************************************************************
 * Game of life implementation
 ****************************************************************************/
char*
game_of_life (char* outboard, 
	      char* inboard,
	      const int nrows,
	      const int ncols,
	      const int gens_max) {

	/* HINT: in the parallel decomposition, LDA may not be equal to
	nrows! */
    // const int LDA = nrows;
	unsigned int curgen, i, j;
	char neighbor_count;
	//print_grid(inboard, nrows, ncols);
	for (curgen = 0; curgen < gens_max; curgen++) {
		memcpy(outboard, inboard, nrows*ncols);
		int index = 0;
		for (i = 0; i < nrows; ++i) {
			for (j = 0; j < ncols; ++j) {
				// We do not bother totally dead cells
				if (*(inboard + index) == 0) {
					++index;
					continue;
				}
				// Calculate the surrounding counts
				neighbor_count = (*(inboard + index)) & 0xf;
				// If alive, but dying
				if (*(inboard + index) & 0x10) {
					if ((neighbor_count != 2) && (neighbor_count != 3)) {
						unset_cell(outboard, i, j, nrows, ncols);
					}
				// If dead, but reborning
				} else if (neighbor_count == (char)3){
					set_cell(outboard, i, j, nrows, ncols);
				}
				++index;
			}
		}
		// Swap the boards
		char* temp = outboard;
		outboard = inboard;
		inboard = temp;
	}
	
	// Convert back to original format
	for (i = 0; i < nrows * ncols; ++i) {
		if (*(inboard+i) == 0) {
			continue;
		}
		*(inboard+i) >>= 4;
	}
	return inboard;
}
