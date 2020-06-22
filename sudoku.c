#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define M 3
#define N (M * M)

// reads a NxN board from stdin
void read(int board[N][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			scanf("%d", &board[i][j]);
		}
	}
}

// prints a NxN board
void print(int board[N][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf("%2d", board[i][j]);
		}
		printf("\n");
	}
}

// initialises the array of valid values
// valid_values[x][y][z] indicates if z + 1 is a valid value for square (x, y) on the board
void initialise(bool valid_values[N][N][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < N; k++) {
				valid_values[i][j][k] = true;
			}
		}
	}
}

// returns the number of valid values for square (x, y)
int remaining_values(bool valid_values[N][N][N], int x, int y) {
	int count = 0;
	for (int i = 0; i < N; i++) {
		if (valid_values[x][y][i]) count++;
	}
	return count;
}

// updates valid values as per the value in square (x, y) on board
void update(bool valid_values[N][N][N], int board[N][N], int x, int y) {
	if (board[x][y]) {
		for (int i = 0; i < N; i++) {
			if (i != x) valid_values[i][y][board[x][y] - 1] = false;
			if (i != y) valid_values[x][i][board[x][y] - 1] = false;
		}
		for (int j = (y / M) * M; j < (y / M) * M + M; j++) {
			for (int i = (x / M) * M; i < (x / M) * M + M; i++) {
				if (i != x && j != y) valid_values[i][j][board[x][y] - 1] = false;
			}
		}
	}
}

// returns the number of unassigned variables on the board constrained with (x, y)
int degree(int board[N][N], int x, int y) {
	int count = 0;
	for (int i = 0; i < N; i++) {
		if (i != y && !board[x][i]) count++;
		else if (i != x && !board[i][y]) count++;
	}
	for (int j = (y / M) * M; j < (y / M) * M + M; j++) {
		for (int i = (x / M) * M; i < (x / M) * M + M; i++) {
			if (i != x && j != y && !board[i][j]) count++;
		}
	}
	return count;
}

// finds an empty cell with the fewest valid values, stores its coordinates at x and y
bool find_best_empty_cell(int board[N][N], bool valid_values[N][N][N], int *x, int *y) {
	int current_min = N;
	*x = -1;
	*y = -1;
	bool empty_found = false;
	for (int j = 0; j < N; j++) {
		for (int i = 0; i < N; i++) {
			if (!board[i][j] && remaining_values(valid_values, i, j) < current_min) { // MRV heuristic
				current_min = remaining_values(valid_values, i, j);
				*x = i;
				*y = j;
				empty_found = true;
			} else if (!board[i][j] && remaining_values(valid_values, i, j) == current_min) { // break tie
				if (degree(board, i, j) < degree(board, *x, *y)) { // degree heuristic
					*x = i;
					*y = j;
					empty_found = true;
				}
			}
		}
	}
	return empty_found;
}

// finds the value for (x, y) that rules out the fewest values for the neighbouring unassigned variables
// using the least constraining value heuristic
bool best_value(int board[N][N], bool valid_values[N][N][N], int x, int y, int *ans) {
	int max_count = N * N;
	bool retval = false;
	for (int value = 0; value < N; value++) {
		if (valid_values[x][y][value]) {
			int count = 0;
			for (int j = 0; j < N; j++) {
				for (int i = 0; i < N; i++) {
					if (i != x && j != y && !board[i][j] && valid_values[i][j][value]) count++;
				}
			}
			for (int j = (y / M) * M; j < (y / M) * M + M; j++) {
				for (int i = (x / M) * M; i < (x / M) * M + M; i++) {
					if (i != x && j != y && !board[i][j] && valid_values[i][j][value]) count++;
				}
			}
			if (count < max_count) {
				max_count = count;
				*ans = value;
				retval = true;
			}
		}
	}
	return retval;
}

// solves the board, records the number of trials
bool solve(int board[N][N], bool valid_values[N][N][N], int *trials, bool display_output) {
	int x, y, value;
	if (!find_best_empty_cell(board, valid_values, &x, &y)) {
		return true;
	}
	while (best_value(board, valid_values, x, y, &value)) {
		board[x][y] = value + 1;
		(*trials)++;
		if (display_output) {
			printf("Assigned %d to (%d, %d):\n", value + 1, x + 1, y + 1);
			print(board);
		}
		// save valid values in case of failure
		bool backup[N][N][N];
		memcpy(backup, valid_values, sizeof(backup));
		update(valid_values, board, x, y);
		if (solve(board, valid_values, trials, display_output)) { // success!
			return true;
		} else { // fail :(
			if (display_output) {
				printf("Backtracking...\n");
			}
			memcpy(valid_values, backup, sizeof(backup));
			valid_values[x][y][value] = false; // remove value from valid values
			board[x][y] = 0;
		} // try again
	}
	return false;
}

int main(int argc, char **argv) {
	bool display_output = false;
	if (argc == 2 && argv[1][0] == '-' && argv[1][1] == 'o' && argv[1][2] == '\0') {
		display_output = true;
	} else if (argc > 1) {
		fprintf(stderr, "Usage: %s [-o] < sudoku.txt\n", argv[0]);
		return EXIT_FAILURE;
	}
	clock_t begin = clock();
	int board[N][N];
	bool valid_values[N][N][N];
	int trials = 0;
	read(board);
	print(board);
	initialise(valid_values);
	for (int y = 0; y < N; y++) {
		for (int x = 0; x < N; x++) {
			update(valid_values, board, x, y);
		}
	}
	if (solve(board, valid_values, &trials, display_output)) {
		if (display_output) {
			printf("Status:\tSuccess!\nTrials:\t%d\n", trials);
		} else {
			printf("Solution:\n");
			print(board);
		}
	} else {
		printf("Status:\tNo solution\n");
	}
	printf("Time:\t%f seconds\n", (double)(clock() - begin) / CLOCKS_PER_SEC);
}
