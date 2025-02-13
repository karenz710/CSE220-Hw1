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
	for (int i = 0; i < size; i++)
	{
		// convert char to int
		top_key[i] = keys[i] - '0';
		bottom_key[i] = keys[i + 2] - '0';
		left_key[i] = keys[i + 4] - '0';
		right_key[i] = keys[i + 6] - '0';
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

int check_col(int col){
	return 0;
	return 1;
}


int check_row(int row){

	return 0;
}

*/
// dupes col means there already exists a piece in the same col
int check_dupes_col(char piece, int col){
	for (int i=0; i<board_size; i++){
		if (board[i][col] == piece + '0')
			return 1; // duplicate found
	}
	return 0;
}
// dupes row means there already exists a piece in the same row, meaning check the same row change col
int check_dupes_row(char piece, int row){
	for (int i=0; i<board_size; i++) {
		if(board[row][i] == piece + '0')
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