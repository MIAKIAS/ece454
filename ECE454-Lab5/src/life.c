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
void update_cell(char* inboard, char* outboard, int* changes, int* next_changes, const int nrows, const int ncols, unsigned int* new_index, int i) {
	// Calculate the surrounding counts
	char neighbor_count = (*(inboard + i)) & 0xf;
	// If alive, but dying
	if (*(inboard + i) & 0x10) {
		if ((neighbor_count != 2) && (neighbor_count != 3)) {
			unset_cell(outboard, i, nrows, ncols, next_changes, new_index);
		}
	// If dead, but reborning
	} else if (neighbor_count == 3){
		set_cell(outboard, i, nrows, ncols, next_changes, new_index);
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
	      const int gens_max,
		  int* changes) {

	/* HINT: in the parallel decomposition, LDA may not be equal to
	nrows! */
	unsigned int new_index;
	unsigned int curgen;
	void* temp;
	int* next_changes = malloc(9*nrows*ncols*sizeof(int));
	int list_size = 0;

	for (int k = 0; changes[k] != -1; ++k) {
		++list_size;
	}
 
	for (curgen = 0; curgen < gens_max; curgen++) {
		memcpy(outboard, inboard, nrows*ncols);
#pragma omp parallel
{
		const int items = list_size / omp_get_num_threads();
		const int begin = items * omp_get_thread_num();
		const int end = (omp_get_thread_num() == omp_get_num_threads() - 1) ? list_size : begin+items;
		int inorth, isouth, jwest, jeast;
		unsigned int i;
		//fprintf(stderr, "end_list: %d, list_size: %d, thread: %d, items: %d; begin: %d; end: %d\n", changes[list_size], list_size, omp_get_thread_num(), items, begin, end);
		for (unsigned int index = begin; index < end ; ++index) {
			i = changes[index];

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
			update_cell(inboard, outboard, changes, next_changes, nrows, ncols, &new_index, i);
			update_cell(inboard, outboard, changes, next_changes, nrows, ncols, &new_index, i+jwest);
			update_cell(inboard, outboard, changes, next_changes, nrows, ncols, &new_index, i+jeast);
			update_cell(inboard, outboard, changes, next_changes, nrows, ncols, &new_index, i+inorth+jwest);
			update_cell(inboard, outboard, changes, next_changes, nrows, ncols, &new_index, i+inorth);
			update_cell(inboard, outboard, changes, next_changes, nrows, ncols, &new_index, i+inorth+jeast);
			update_cell(inboard, outboard, changes, next_changes, nrows, ncols, &new_index, i+isouth+jwest);
			update_cell(inboard, outboard, changes, next_changes, nrows, ncols, &new_index, i+isouth);
			update_cell(inboard, outboard, changes, next_changes, nrows, ncols, &new_index, i+isouth+jeast);
		}
}
		//fprintf(stderr, "finish\n");
		next_changes[new_index] = -1;
		// Swap the boards
		temp = outboard;
		outboard = inboard;
		inboard = temp;
		// Swap the changes
		temp = next_changes;
		next_changes = changes;
		changes = temp;
		// Update the size of the list
		list_size = new_index;
		new_index = 0;
	}
	
	
	// Convert back to original format
	for (int i = 0; i < nrows * ncols; ++i) {
		if (*(inboard+i) == 0) {
			continue;
		}
		*(inboard+i) >>= 4;
	}
	return inboard;
}
