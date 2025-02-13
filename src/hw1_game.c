#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "hw1.h"

int main(int argc, char **argv) {
    // argv[1] is the size of the board, argv[2] is the pieces on the initial board, argv[3] is the number of visible skyscrapers provided in command-line args
    assert(argc == 4); //we want a size, board state, and a list of keys
    // returns 0 if fail, 1 if works
	if (initialize_board(argv[2], argv[3], (int)strtoul(argv[1], NULL, 10)) == 0){
        printf("Invalid initial board state.");
    }
	print_board();
    return 0;
}
