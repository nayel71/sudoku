#include "sudoku.h"
#include <stdio.h>
#include <stdlib.h>	// malloc(), free()
#include <string.h>	// memcpy()

#define M 3
#define N (M * M)

struct sudoku {
	int board[N][N];
	bool valid_values[N][N][N]; // valid_values[x][y][z] indicates if z + 1 is a valid value for square (x, y)
	int trials;
	bool display_output;
};

sudoku *sudoku_new(void) {
	sudoku *s = malloc(sizeof(sudoku));
	return s;
}

void sudoku_destroy(sudoku *s) {
	free(s);
}

void sudoku_print_stats(sudoku *s) {
	printf("Trials:\t%d\n", s->trials);
}

bool sudoku_init(sudoku *s, int argc, char **argv) {
	s->trials = 0;
	s->display_output = false;
	if (argc == 2 && argv[1][0] == '-' && argv[1][1] == 'o' && argv[1][2] == '\0') {
		s->display_output = true;
	} else if (argc > 1) {
		return false;
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			scanf("%d", &s->board[i][j]);
			for (int k = 0; k < N; k++) {
				s->valid_values[i][j][k] = true;
			}
		}
	}
	for (int y = 0; y < N; y++) {
		for (int x = 0; x < N; x++) {
			sudoku_update(s, x, y);
		}
	}
	return true;
}

void sudoku_print(sudoku *s) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf("%2d", s->board[i][j]);
		}
		printf("\n");
	}
}

void sudoku_update(sudoku *s, int x, int y) {
	if (s->board[x][y]) {
		for (int i = 0; i < N; i++) {
			if (i != x) {
				s->valid_values[i][y][s->board[x][y] - 1] = false;
			}
			if (i != y) {
				s->valid_values[x][i][s->board[x][y] - 1] = false;
			}
		}
		for (int j = (y / M) * M; j < (y / M) * M + M; j++) {
			for (int i = (x / M) * M; i < (x / M) * M + M; i++) {
				if (i != x && j != y) {
					s->valid_values[i][j][s->board[x][y] - 1] = false;
				}
			}
		}
	}
}

// returns the number of valid values for square (x, y)
static int remaining_values(sudoku *s, int x, int y) {
	int count = 0;
	for (int i = 0; i < N; i++) {
		if (s->valid_values[x][y][i]) {
			count++;
		}
	}
	return count;
}

// if ignore_value, returns the number of unassigned variables on the board constrained with (x, y)
// otherwise, returns the number of such variables for which value is valid
static int degree(sudoku *s, int x, int y, bool ignore_value, int value) {
	int count = 0;
	for (int i = 0; i < N; i++) {
		if (i != y && !s->board[x][i] && (ignore_value || s->valid_values[x][i][value])) {
			count++;
		} else if (i != x && !s->board[i][y] && (ignore_value || s->valid_values[i][y][value])) {
			count++;
		}
	}
	for (int j = (y / M) * M; j < (y / M) * M + M; j++) {
		for (int i = (x / M) * M; i < (x / M) * M + M; i++) {
			if (i != x && j != y && !s->board[i][j] && (ignore_value || s->valid_values[i][y][value])) {
				count++;
			}
		}
	}
	return count;
}

// tries to find an empty cell with the fewest valid values, stores its coordinates at x and y
static bool find_best_empty_cell(sudoku *s, int *x, int *y) {
	int min = N;
	bool found = false;
	for (int j = 0; j < N; j++) {
		for (int i = 0; i < N; i++) {
			if (!s->board[i][j]) {
				int count = remaining_values(s, i, j);
				if (count < min) { // MRV heuristic
					min = count;
					found = true;
					*x = i;
					*y = j;
				} else if (count == min) { // break tie
					if (degree(s, i, j, true, 0) < degree(s, *x, *y, true, 0)) { // degree heuristic
						found = true;
						*x = i;
						*y = j;
					}
				}
			}
		}
	}
	return found;
}

// tries to find the value for (x, y) that rules out the fewest values for the neighbouring unassigned variables
// (neighbouring in the sense of the dependency graph) -- least constraining value heuristic
static bool find_best_value(sudoku *s, int x, int y, int *ans) {
	int min = N * N;
	bool found = false;
	for (int value = 0; value < N; value++) {
		if (s->valid_values[x][y][value]) {
			int count = degree(s, x, y, false, value);
			if (count < min) {
				min = count;
				found = true;
				*ans = value;
			}
		}
	}
	return found;
}

bool sudoku_solve(sudoku *s) {
	int x, y, value;
	if (!find_best_empty_cell(s, &x, &y)) {
		return true;
	}
	while (find_best_value(s, x, y, &value)) {
		s->board[x][y] = value + 1;
		s->trials++;
		if (s->display_output) {
			printf("Assigned %d to (%d, %d):\n", value + 1, x + 1, y + 1);
			sudoku_print(s);
		}
		// save valid values in case of failure
		bool backup[N][N][N];
		memcpy(backup, s->valid_values, sizeof(backup));
		sudoku_update(s, x, y);
		if (sudoku_solve(s)) { // success!
			return true;
		} else { // fail :(
			if (s->display_output) {
				printf("Backtracking...\n");
			}
			memcpy(s->valid_values, backup, sizeof(backup));
			s->valid_values[x][y][value] = false; // remove value from valid values
			s->board[x][y] = 0;
		} // try again
	}
	return false;
}
