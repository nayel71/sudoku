#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdbool.h>

typedef struct sudoku sudoku;

// allocates memory for a new sudoku object
sudoku *sudoku_new(void);

// frees the memory allocated for a sudoku object
void sudoku_destroy(sudoku *s);

// tries to read a sudoku board from stdin
bool sudoku_init(sudoku *s, int argc, char **argv);

// prints a sudoku board
void sudoku_print(sudoku *s);

// prints current game stats
void sudoku_print_stats(sudoku *s);

// updates valid values for all squares as per the value in square (x, y)
void sudoku_update(sudoku *s, int x, int y);

// tries to solve the sudoku
bool sudoku_solve(sudoku *s);

#endif /* SUDOKU_H */
