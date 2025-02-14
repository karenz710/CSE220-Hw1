#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "hw1.h"

int top_key[MAX_LENGTH] = {0};
int bottom_key[MAX_LENGTH] = {0};
int left_key[MAX_LENGTH] = {0};
int right_key[MAX_LENGTH] = {0};

char board[MAX_LENGTH][MAX_LENGTH] = {0};

int length = 5;

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
int solve(const char *initial_state, const char *keys, int size)
{
	(void)initial_state;
	(void)keys;
	(void)size;

	return 1;
}