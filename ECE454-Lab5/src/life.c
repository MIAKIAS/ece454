/*****************************************************************************
 * life.c
 * Parallelized and optimized implementation of the game of life resides here
 ****************************************************************************/
#include "life.h"
#include "util.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
	unsigned int curgen, i, new_index, index = 0;
	char neighbor_count;
	void* temp;
	int* changes = malloc(9*nrows*ncols*sizeof(int));
	int* next_changes = malloc(9*nrows*ncols*sizeof(int));
	// First pass
	memcpy(outboard, inboard, nrows*ncols);
	for (i = 0; i < nrows*ncols; ++i) {
		// We do not bother totally dead cells
		if (*(inboard + i) == 0) {
			continue;
		}
		// Calculate the surrounding counts
		neighbor_count = (*(inboard + i)) & 0xf;
		// If alive, but dying
		if (*(inboard + i) & 0x10) {
			if ((neighbor_count != 2) && (neighbor_count != 3)) {
				unset_cell(outboard, i, nrows, ncols, changes, &index);
			}
		// If dead, but reborning
		} else if (neighbor_count == (char)3){
			set_cell(outboard, i, nrows, ncols, changes, &index);
		}
	}
	changes[index] = -1;
	// Swap the boards
	temp = outboard;
	outboard = inboard;
	inboard = temp;
	// Following Passes
	for (curgen = 1; curgen < gens_max; curgen++) {
		memcpy(outboard, inboard, nrows*ncols);
		new_index = 0;
		for (index = 0; (i = changes[index]) != -1 ; ++index) {
			// We do not bother totally dead cells
			if (*(inboard + i) == 0) {
				continue;
			}
			// Calculate the surrounding counts
			neighbor_count = (*(inboard + i)) & 0xf;
			// If alive, but dying
			if (*(inboard + i) & 0x10) {
				if ((neighbor_count != 2) && (neighbor_count != 3)) {
					unset_cell(outboard, i, nrows, ncols, next_changes, &new_index);
				}
			// If dead, but reborning
			} else if (neighbor_count == (char)3){
				set_cell(outboard, i, nrows, ncols, next_changes, &new_index);
			}
		}
		next_changes[new_index] = -1;
		// Swap the boards
		temp = outboard;
		outboard = inboard;
		inboard = temp;
		// Swap the changes
		temp = next_changes;
		next_changes = changes;
		changes = temp;
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
