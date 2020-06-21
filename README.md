# Sudoku

A backtracking Sudoku solver

## Installation

- `git clone https://github.com/nayel71/sudoku.git`
- `make`

## Usage

Write the Sudoku in a text file (see `sudoku.txt` for an example) and redirect it into the executable, e.g.

- `./sudoku < sudoku.txt`
- `./sudoku < sudoku.txt > solution.txt`
- `./sudoku -o < sudoku.txt`
- `./sudoku -o < sudoku.txt > solution.txt`

(Use the flag `-o` to display verbose output of the backtracking process.)

## Notes

- The program does not check if the Sudoku is valid.
- If there are multiple solutions, the program will find only one.
