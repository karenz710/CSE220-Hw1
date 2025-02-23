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
	for (int row = 0; row < board_size; row++) {
        for (int col = 0; col < board_size; col++) {
			if (board[row][col] != '-') {
				int piece = board[row][col] - '0';
				apply_constraint_propagation(row, col, piece);
			}
		}
	}
	print_possible_pieces_state();
	printf("initial board\n");
	print_board();
	// apply heuristics until can't make progress
	bool progress;
	do {
		progress = 0;

		if(apply_sequence_filtration()){
			progress = 1; 
		}


		if (apply_edge_constraint_rule()) {
            progress = 1;
        }
		for (int row = 0; row < board_size; row++) {
			for (int col = 0; col < board_size; col++) {
				for (int piece = 1; piece <= board_size; piece++) {
					if(board[row][col] == '-'){
						if(apply_process_of_elimination(row,col, piece)){
							progress = 1;
						}
					}	
				}
			}
		}

		
				
	} while (progress == 1 && isFull() == 0);
	printf("solved board\n");
	print_board();
	print_possible_pieces_state();
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
			// print_possible_pieces_state();
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

int apply_process_of_elimination(int row, int col, int piece){
	// rows
	bool hasFoundPiece = false;
	for (int c = 0; c < board_size; c++) {
		if(c != col && possible_pieces[row][c][piece] == true){
			hasFoundPiece = true;
		}
	}
	if(!hasFoundPiece){
		// We havent found the piece so we can insert it.
		set_cell_value(row, col, piece);
		return 1;
	}
	hasFoundPiece = false;
	for (int r = 0; r < board_size; r++) {
		if(r != row && possible_pieces[r][col][piece] == true){
			hasFoundPiece = true;
		}
	}
	if(!hasFoundPiece){
		// We havent found the piece so we can insert it.
		set_cell_value(row, col, piece);
		return 1;
	}
	return 0;
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

// --- Helper globals for sequence generation --- 
// These variables keep track of how many sequences were generated.
static int valid_seq_count = 0;
static int filtered_seq_count = 0;

//helper
static int visible_count(const int sequence[], int length, bool from_left) {
    int count = 0;
    int max = 0;
    if (from_left) {
        for (int i = 0; i < length; i++) {
            if (sequence[i] > max) {
                count++;
                max = sequence[i];
            }
        }
    } else {  // from right
        for (int i = length - 1; i >= 0; i--) {
            if (sequence[i] > max) {
                count++;
                max = sequence[i];
            }
        }
    }
    return count;
}


void generate_valid_sequences_helper(int valid_sequences[MAX_SEQUENCE_CAP][MAX_LENGTH],
                                     bool is_horizontal, int index,
                                     int sequence[MAX_LENGTH], int starts_at) {
    if (starts_at == board_size) {
        // For a horizontal row, the clues are in left_key and right_key;
        // for a vertical column, they are in top_key and bottom_key.
        int clue1 = is_horizontal ? left_key[index] : top_key[index];
        int clue2 = is_horizontal ? right_key[index] : bottom_key[index];

        if (clue1 != 0 && visible_count(sequence, board_size, true) != clue1)
            return;
        if (clue2 != 0 && visible_count(sequence, board_size, false) != clue2)
            return;
        // Sequence is valid – store it
        if (valid_seq_count < MAX_SEQUENCE_CAP) {
            for (int i = 0; i < board_size; i++) {
                valid_sequences[valid_seq_count][i] = sequence[i];
            }
            valid_seq_count++;
        }
        return;
    }
    // Try every number from 1 to board_size that is not already in sequence.
    for (int num = 1; num <= board_size; num++) {
        bool used = false;
        for (int j = 0; j < starts_at; j++) {
            if (sequence[j] == num) {
                used = true;
                break;
            }
        }
        if (!used) {
            sequence[starts_at] = num;
            generate_valid_sequences_helper(valid_sequences, is_horizontal, index, sequence, starts_at + 1);
        }
    }
}


void generate_valid_sequences(int valid_sequences[MAX_SEQUENCE_CAP][MAX_LENGTH],
                              bool is_horizontal, int index) {
    valid_seq_count = 0;  // reset counter
    int sequence[MAX_LENGTH] = {0};
    generate_valid_sequences_helper(valid_sequences, is_horizontal, index, sequence, 0);
}


void generate_filtered_sequences(int filtered_sequences[MAX_SEQUENCE_CAP][MAX_LENGTH],
                                 bool is_horizontal, int index) {
    int temp_valid_sequences[MAX_SEQUENCE_CAP][MAX_LENGTH];
    generate_valid_sequences(temp_valid_sequences, is_horizontal, index);
    filtered_seq_count = 0;  // reset counter

    // Loop over all valid sequences.
    for (int s = 0; s < valid_seq_count; s++) {
        bool valid = true;
        for (int pos = 0; pos < board_size; pos++) {
            int seq_val = temp_valid_sequences[s][pos];
            if (is_horizontal) {
                // For row 'index' at column 'pos'
                if (board[index][pos] != '-') {
                    if (board[index][pos] - '0' != seq_val) {
                        valid = false;
                        break;
                    }
                } else {
                    if (!possible_pieces[index][pos][seq_val]) {
                        valid = false;
                        break;
                    }
                }
            } else {
                // For column 'index' at row 'pos'
                if (board[pos][index] != '-') {
                    if (board[pos][index] - '0' != seq_val) {
                        valid = false;
                        break;
                    }
                } else {
                    if (!possible_pieces[pos][index][seq_val]) {
                        valid = false;
                        break;
                    }
                }
            }
        }
        if (valid) {
            for (int i = 0; i < board_size; i++) {
                filtered_sequences[filtered_seq_count][i] = temp_valid_sequences[s][i];
            }
            filtered_seq_count++;
        }
    }
}


bool sequence_filtration(bool is_horizontal, int index) {
    int filtered_sequences[MAX_SEQUENCE_CAP][MAX_LENGTH];
    generate_filtered_sequences(filtered_sequences, is_horizontal, index);
    bool progress = false;
    if (filtered_seq_count == 0) {
        return false;
    }
    for (int pos = 0; pos < board_size; pos++) {
        // Skip if cell is already filled.
        if (is_horizontal) {
            if (board[index][pos] != '-') continue;
        } else {
            if (board[pos][index] != '-') continue;
        }
        int common_val = filtered_sequences[0][pos];
        bool all_same = true;
        for (int s = 1; s < filtered_seq_count; s++) {
            if (filtered_sequences[s][pos] != common_val) {
                all_same = false;
                break;
            }
        }
        if (all_same) {
            // Place the forced value.
            if (is_horizontal)
                set_cell_value(index, pos, common_val);
            else
                set_cell_value(pos, index, common_val);
            progress = true;
        }
    }
    return progress;
}

bool apply_sequence_filtration(void) {
    bool progress = false;
    for (int i = 0; i < board_size; i++) {
        if (sequence_filtration(true, i))
            progress = true;
        if (sequence_filtration(false, i))
            progress = true;
    }
    return progress;
}


