# Sudoku

A backtracking Sudoku solver

## Installation

- `git clone https://github.com/nayel71/sudoku.git`
- `make`

## Usage

Write the Sudoku in a text file (see `d.txt` for the example with diagonal symmetry from [Wikipedia](https://en.wikipedia.org/wiki/Sudoku#Mathematics_of_Sudoku) and redirect it into the executable, e.g.

```
$ ./solver < d.txt
 0 0 0 2 1 0 0 0 0
 0 0 7 3 0 0 0 0 0
 0 5 8 0 0 0 0 0 0
 4 3 0 0 0 0 0 0 0
 2 0 0 0 0 0 0 0 8
 0 0 0 0 0 0 0 7 6
 0 0 0 0 0 0 2 5 0
 0 0 0 0 0 7 3 0 0
 0 0 0 0 9 8 0 0 0
Solution:
 6 4 3 2 1 5 8 9 7
 1 2 7 3 8 9 6 4 5
 9 5 8 7 6 4 1 2 3
 4 3 5 8 7 6 9 1 2
 2 7 6 9 5 1 4 3 8
 8 9 1 4 3 2 5 7 6
 7 8 9 6 4 3 2 5 1
 5 6 4 1 2 7 3 8 9
 3 1 2 5 9 8 7 6 4
Status:	Success
Trials:	7724
Time:	0.042909 seconds
```
Similarly,

```
$ ./solver -o < d.txt > ds.txt
```
will write verbose output of the backtracking process to the file `ds.txt`.

## Notes

- The program does not check if the Sudoku is valid.
- If there are multiple solutions, the program will find only one.
