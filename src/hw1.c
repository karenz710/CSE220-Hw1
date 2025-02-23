#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "hw1.h"

// ./build/hw1_solver_tests
int top_key[MAX_LENGTH] = {0};
int bottom_key[MAX_LENGTH] = {0};
int left_key[MAX_LENGTH] = {0};
int right_key[MAX_LENGTH] = {0};

char board[MAX_LENGTH][MAX_LENGTH] = {0};

bool possible_pieces[MAX_LENGTH][MAX_LENGTH][MAX_LENGTH+1] = {0}; // global var to track possible 
// int length = 5;

int board_size; // global var

// initial state "2112" (board state) numbers btwn 1-N or -; keys "12 (top) 21 (bottom) 12 (left) 21 (right)" (number of visible skyscrapers) btwn 0-N, 0 is no clue; size "2" nxn dim board
/*    1 2
	  v v
  1 > 2 1 < 2
  2 > 1 2 < 1
	  ^ ^
	  2 1
*/

// need to implement error return 0
int initialize_board(const char *initial_state, const char *keys, int size)
{
	board_size = size;
	(void)initial_state;
	(void)keys;
	(void)size;
	// set keys by parsing keys string (top,bottom,left,right)
	// 01234567 (01) (23) (45) (67)
	for (int i = 0; i < size; i++){
		// convert char to int
		top_key[i] = keys[i] - '0';
		bottom_key[i] = keys[board_size + i] - '0';
		left_key[i] = keys[2 * board_size + i] - '0';
		right_key[i] = keys[3 * board_size + i] - '0';
	}
	int ptr = 0;
	// initialize board keys
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			board[i][j] = initial_state[ptr];
			ptr += 1;
		}
	}

	// check for duplicates in row
	for (int i = 0; i < board_size; i++) {
        int seen[MAX_LENGTH] = {0};
		for(int j = 0; j < board_size; j++) {
			char current = board[i][j];
			if (current != '-' && seen[current - '0']) {
				return 0;
			}
			if (current != '-') {
                seen[current - '0'] = 1; // mark number as seen
            }
		}
	}
	// check for duplicates in col
	for (int j = 0; j < board_size; j++) {
        int seen[MAX_LENGTH] = {0}; 
        for (int i = 0; i < board_size; i++) {
            char current = board[i][j];
            if (current != '-' && seen[current - '0']) { 
                return 0; 
            }
            if (current != '-') {
                seen[current - '0'] = 1; // mark this number as seen
            }
        }
    }

	// check for key errors:
	for (int i = 0; i < board_size; i++) {
		for(int j = 0; j < board_size; j++){
			if (board[i][j]!='-'){
				char piece = board[i][j];
				board[i][j] = '-';
				if (check_col(piece, i, j) == 0) {
					return 0; // col violation
				}else{
					board[i][j] = piece;
				}
				board[i][j] = '-';
				if (check_row(piece, i, j) == 0) {
					return 0; // row violation
				}else{
					board[i][j] = piece;
				}
			}
		}
	}

	return 1;
}

// helper methods
void print_board()
{
	// print top keys
	printf("    ");
	for (int i = 0; i < board_size; i++)
	{
		printf("%d ", top_key[i]);
	}
	printf("\n    ");
	for (int i = 0; i < board_size; i++)
	{
		printf("v ");
	}
	printf("\n");
	// print a left key then > then that row and then print < and right key go by index: 0, 1, 2, 3, 4
	for (int i = 0; i < board_size; i++)
	{
		printf("%d > ", left_key[i]);
		for (int j = 0; j < board_size; j++)
		{
			printf("%c ", board[i][j]);
		}
		printf("< %d", right_key[i]);
		printf("\n");
	}
	// print bottom row
	printf("    ");
	for (int i = 0; i < board_size; i++)
	{
		printf("^ ");
	}
	printf("\n    ");
	for (int i = 0; i < board_size; i++)
	{
		printf("%d ", bottom_key[i]);
	}
	printf("\n");
}

// keys are stored as ints, board is stored as char
// return 0 means error
/*int get_move(){

	return 0;
}

*/

// check if it violates the column height
// if the key is a 0 then it tells no information, so unnecessary to check
int check_col(char piece, int row, int col){
	// first check if column is full; if it is not full return 1 it is full if count of '-' is more than one
	int count = 0;
	for (int i = 0; i<board_size; i++){
		if(board[i][col]=='-'){
			count+=1;
			if(count == 2)
				return 1;
		}
	}
	// otherwise there is only one "-" remaining and that is where it'll be placed
	board[row][col] = piece;
	// get corresponding top and bottom key 
	int topKeyCheck = top_key[col];
	int bottomKeyCheck = bottom_key[col];
		// count the number of buildings that can be seen from above and below
	/*  1
		v  
		2 
		1
        ^
		2 */ 
	// check top first
	int res = 1;
	char prev = board[0][col];
	for (int i = 1; i<board_size;i++){
		if(board[i][col] > prev){
			res += 1;
			prev = board[i][col];
		} else { // reached a smaller building that can't be seen
			;
		}	
	}
	if(topKeyCheck==0){
		; // don't check
	}else if(res!=topKeyCheck){
		board[row][col] = '-'; // reset board
		return 0; // error
	}
	// right side 
	res = 1;
	prev = board[board_size-1][col];
	for (int i = board_size-2; i>=0; i--) {
		if(board[i][col] > prev){
			res += 1;
			prev = board[i][col];
		} else { // reached a smaller building that can't be seen
			;
		}	
	}
	if(bottomKeyCheck==0){
		;
	} else if(res!=bottomKeyCheck){
		board[row][col] = '-'; // reset board
		return 0; // error
	}
	board[row][col] = '-'; // reset board necessary for other checks !
	return 1;
}

int check_row(char piece, int row, int col){
	// first check if row is full; if it is not full return 1 it is full if count of '-' is more than one
	int count = 0;
	for (int i = 0; i<board_size; i++){
		if(board[row][i]=='-'){
			count+=1;
			if(count == 2)
				return 1;
		}
	}
	// otherwise there is only one '-' remaining and that is where it'll be placed
	board[row][col] = piece;
	// get corresponding top and bottom key 
	int leftKeyCheck = left_key[row];
	int rightKeyCheck = right_key[row];
	// count the number of buildings that can be seen from left
	// 2 > 2 1 4 3 < 0
	// left side
	int res = 1;
	char prev = board[row][0];
	for (int i = 1; i<board_size;i++){
		if(board[row][i] > prev){
			res += 1;
			prev = board[row][i];
		} else { // reached a smaller building that can't be seen
			;
		}	
	}
	if(leftKeyCheck==0){
		;
	} else if(res!=leftKeyCheck){
		board[row][col] = '-'; // reset board
		return 0; // error
	}
	// right side 
	res = 1;
	prev = board[row][board_size-1];
	for (int i = board_size-2; i>=0; i--) {
		if(board[row][i] > prev){
			res += 1;
			prev = board[row][i];
		} else { // reached a smaller building that can't be seen
			;
		}	
	}
	if(rightKeyCheck==0){
		;
	} else if(res!=rightKeyCheck){
		board[row][col] = '-'; // reset board
		return 0; // error
	}
	board[row][col] = '-'; // reset board necessary for other checks !
	return 1;
}

// dupes col means there already exists a piece in the same col
int check_dupes_col(char piece, int col){
	for (int i=0; i<board_size; i++){
		if (board[i][col] == piece)
			return 1; // duplicate found
	}
	return 0;
}
// dupes row means there already exists a piece in the same row, meaning check the same row change col
int check_dupes_row(char piece, int row){
	for (int i=0; i<board_size; i++) {
		if(board[row][i] == piece)
			return 1;
	}
	return 0;
}

int try_move(int row, int col){
	if (board[row][col] == '-')
		return 1;
	else
		return 0;
}

// returns 0 if it is not full and 1 if it is full
int isFull(){
	for (int i = 0; i < board_size; i++){
		for (int j = 0; j < board_size; j++){
			if (board[i][j] == '-'){
				return 0;
			}
		}
	}
	return 1;
}
 
// solve functions
// create a constraint list for each cell, if only one cell remains then populate the board with the val
int solve(const char *initial_state, const char *keys, int size)
{
	(void)initial_state;
	(void)keys;
	(void)size;
	
	if (initialize_board(initial_state, keys, size)==0) {
        return 0;  // Invalid initial state
    }	

	initialize_possibilities();
	// for (int row = 0; row < board_size; row++) {
    //     for (int col = 0; col < board_size; col++) {
	// 		int piece = board[row][col] - '0';
	// 		apply_constraint_propagation(row, col, piece);
	// 	}
	// }
	print_possible_pieces_state();
	printf("initial board\n");
	print_board();
	// apply heuristics until can't make progress
	bool progress;
	do {
		progress = 0;
		if (apply_edge_constraint_rule()) {
            progress = 1;
        }
	} while (progress == 1 && isFull() == 0);
	printf("solved board\n");
	print_board();
	print_possible_pieces_at_cell(0,1);
	print_possible_pieces_at_cell(0,0);
	print_possible_pieces_at_cell(1,0);
	print_possible_pieces_at_cell(3,0);
	print_possible_pieces_at_cell(3,2);
	return 1;
}

void populate(int row, int col){
	// also check if one key is left then populate it with the board
	int count = 0;
	for(int i = 1; i <= board_size; i++) {
		if(possible_pieces[row][col][i]==true)
			count+=1;
	}
	int piece = 1;
	if(count == 1){ // find the val thats true
		for(int i = 1; i <= board_size; i++){
			if(possible_pieces[row][col][i]==true)
				piece = i;
		}
		// set board to piece as a char
		board[row][col] = piece + '0';

	}

}

bool apply_constraint_propagation(int row, int col, int value){
	printf("%d %d\n", row, col);
	bool changes_made = false;
	// also check if one key is left then populate it with the board
	// remove this value from the same row
    for (int c = 0; c < board_size; c++) {
        if (c != col && board[row][c] == '-') {
            possible_pieces[row][c][value] = false;
			// try populate which checks if only one val
			populate(row, c);
            changes_made = true;
        }
    }
    // Remove this value from the same column
    for (int r = 0; r < board_size; r++) {
        if (r != row && board[r][col] == '-') {
            possible_pieces[r][col][value] = false;
			populate(r,col);
            changes_made = true;
			print_possible_pieces_state();
        }
    }
    
    return changes_made;
}

// initialize possibilities
void initialize_possibilities(){
	for (int row = 0; row < board_size; row++) {
        for (int col = 0; col < board_size; col++) {
            if (board[row][col] != '-') {
                // Cell already has a value, clear all possibilities except the assigned one
                int piece = board[row][col] - '0';
                for (int k = 1; k <= board_size; k++) {
                    possible_pieces[row][col][k] = (k == piece); // index is true if possibility otherwise it is false
					// apply constraint propagation to elimate possibilities
                }
				// apply_constraint_propagation(row, col, piece);
            } else {
                // Cell is empty, all values are possible
                for (int k = 1; k <= board_size; k++) {
                    possible_pieces[row][col][k] = true;
                }
            }
        }
    }
}

// takes in INT val *set all vals to ints*
void set_cell_value(int row, int col, int value){
	board[row][col] = '0' + value;
	// update constraint list for cells
	for (int k = 1; k <= board_size; k++) {
		possible_pieces[row][col][k] = (k == value); // index is true if possibility otherwise it is false
	}
	apply_constraint_propagation(row, col, value);
}

// return 1 if made some progress 0 if no progress
bool apply_edge_constraint_rule(void){
	bool changes_made = 0;
	//process top keys for 1 or for n
	for (int col = 0; col < board_size; col++){
		int key = top_key[col];
		if (key == 1){
			// tallest building must be at top (N)
			if(board[0][col]=='-'){
				set_cell_value(0, col, board_size);
				changes_made = true;
			}
		} else if (key==board_size){ // 5 > 1 2 3 4 5
			for(int row=0; row<board_size; row++){
				if(board[row][col] == '-'){
					set_cell_value(row, col, row+1);
					changes_made = true;
				}
			}
		} else if (key > 1 && key < board_size) {
			// Apply edge constraint rule
			// find the key and then iterate through the col eliminating possibilities
			// iterate thru each pos
			for (int d = 0; d < board_size; d++){
				int lower_bound = board_size - key + 2 + d;
				for(int i = lower_bound; i <= board_size; i++)
					possible_pieces[d][col][i] = false;
				// set lower_bound to board_size inclusive to false
			}
		}
	}
	// process bottom keys
	for (int col = 0; col < board_size; col++){
		int key = bottom_key[col];
		if (key == 1){
			// tallest building must be at bottom (N)
			if(board[board_size-1][col]=='-'){
				set_cell_value(board_size-1, col, board_size);
				changes_made = true;
			}
		} else if (key==board_size){ // 5 > 1 2 3 4 5
			for(int row=board_size-1; row>0; row--){
				if(board[row][col] == '-'){
					set_cell_value(row, col, board_size-row);
					changes_made = true;
				}
			}
		}
	}
	//process left keys
	for (int row = 0; row < board_size; row++) {
		int key = left_key[row];
		if (key == 1) {
			if (board[row][0] == '-') {
				set_cell_value(row, 0, board_size);
				changes_made = true;
			}
		} else if (key == board_size) {
			for (int col = 0; col < board_size; col++) {
				if (board[row][col] == '-') {
					set_cell_value(row, col, col + 1);
					changes_made = true;
				}
			}
		}	
	}
	//process right keys
	for (int row = 0; row < board_size; row++) {
		int key = right_key[row];
		if (key == 1) {
			// Tallest building must be at the right
			if (board[row][board_size - 1] == '-') {
				set_cell_value(row, board_size - 1, board_size);
				changes_made = true;
			}
		} else if (key == board_size) {
			// Buildings must be in ascending order from right to left
			for (int col = board_size - 1; col >= 0; col--) {
				if (board[row][col] == '-') {
					set_cell_value(row, col, board_size - col);
					changes_made = true;
				}
			}
		}
	}
	return changes_made;
}

// look for 1 and size N
/*
bool apply_constraint_propagation(int row, int column, int piece);
void apply_process_of_elimination(int row, int column, int piece);
*/

// Testing functions
void print_possible_pieces_state(void) {
   printf("Possible pieces state:\n");
    for (int row = 0; row < board_size; row++) {
        for (int col = 0; col < board_size; col++) {
            printf("(%d,%d): ", row, col);
            if (board[row][col] != '-') {
                // cell is already solved
                printf("%c ", board[row][col]);
            } else {
                printf("[");
                int count = 0;
                for (int piece = 1; piece <= board_size; piece++) {
                    if (possible_pieces[row][col][piece]) {
                        if (count > 0) {
                            printf(", ");
                        }
                        printf("%d", piece);
                        count++;
                    }
                }
                printf("] ");
            }
        }
        printf("\n");
    }
}
void print_possible_pieces_at_cell(int row, int column){
	printf("Possible pieces at (%d,%d): ", row, column);
    
    // if the cell already has a value, show that
    if (board[row][column] != '-') {
        printf("placed val [%c]\n", board[row][column]);
        return;
    }
    
    // print all possible values
    printf("[");
    int count = 0;
    for (int piece = 1; piece <= board_size; piece++) {
        if (possible_pieces[row][column][piece]) { // if it is true then it is a possibility
            if (count > 0) {
                printf(", ");
            }
            printf("%d", piece);
            count++;
        }
    }
    printf("]\n");
}