#include "sudoku.h"
#include <stdio.h>
#include <stdlib.h>	// EXIT_FAILURE
#include <time.h>	// clock_t, clock(), CLOCKS_PER_SEC

int main(int argc, char **argv) {
	clock_t begin = clock();
	sudoku *s = sudoku_new();
	if (!sudoku_init(s, argc, argv)) {
		fprintf(stderr, "Usage: %s [-o] < puzzle.txt\n", argv[0]);
		sudoku_destroy(s);
		return EXIT_FAILURE;
	}
	sudoku_print(s);
	if (sudoku_solve(s)) {
		puts("Solution:");
		sudoku_print(s);
		puts("Status:\tSuccess");
	} else {
		puts("Status:\tNo solution");
	}
	sudoku_print_stats(s);
	sudoku_destroy(s);
	printf("Time:\t%f seconds\n", (double)(clock() - begin) / CLOCKS_PER_SEC);
}
