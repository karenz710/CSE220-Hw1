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
    while(1){
        char choice;
        int piece, row, col;
    	print_board();
        printf("Choose a piece (1-%d) or q to quit: ", board_size);
        while(1){
            scanf(" %c", &choice); 
            if (choice == 'q'){
                return 0;
            } else if (choice >= '1' && choice <= '0' + board_size) {
                piece = choice - '0';
                break; 
            } else {
                printf("Invalid choice. Choose a piece (1-%d) or q to quit: ", board_size);
            } 
        }
        // ask for row and col
        printf("Choose a row (0-%d): ", board_size-1);
        while(1) {
            scanf("%d", &row);
            if (row >= 0 && row <= board_size-1){
                break;
            } else {
                printf("Invalid choice. Choose a row (0-%d): ", board_size-1);
            }
        }
        printf("Choose a column (0-%d): ", board_size-1);
        while(1) {
            scanf("%d", &col);
            if (col >= 0 && col <= board_size-1){
                break;
            } else {
                printf("Invalid choice. Choose a column (0-%d): ", board_size-1);
            }
        }

        // check if row, col is occupied
        if (try_move(row, col) == 0) {
            printf("Invalid choice. That space is already occupied.\n");
            continue; // go back to start of move
        } 
        
        if (check_dupes_col(piece, col) || check_dupes_row(piece, row)){
            printf("Invalid choice. There is already a building with that height in that row or column.\n");
            continue;
        } 

        board[row][col] = piece + '0';
        
        // check for fullness
        if(isFull()){
            printf("Congratulations, you have filled the board!\n");
            print_board();
            return 0;
        }
    }
    return 0;
}
