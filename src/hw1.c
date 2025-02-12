#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "hw1.h"

int top_key[MAX_LENGTH] = {0};
int bottom_key[MAX_LENGTH] = {0};
int left_key[MAX_LENGTH] = {0};
int right_key[MAX_LENGTH] = {0};

char board[MAX_LENGTH][MAX_LENGTH] = {0};

int length = 5;


// initial state "2112" (board state) numbers btwn 1-N or -; keys "12 (top) 21 (bottom) 12 (left) 21 (right)" (number of visible skyscrapers) btwn 0-N, 0 is no clue; size "2" nxn dim board
/*    1 2
      v v  
  1 > 2 1 < 2
  2 > 1 2 < 1
	  ^ ^
	  2 1
*/

int initialize_board(const char *initial_state, const char *keys, int size) {
	(void) initial_state;
	(void) keys;
	(void) size;
	
	return 1;
}

void print_board(){

}
int get_move(){

}
int check_dupes_col(int col){

}
int check_dupes_row(int row){

}
int check_row(int row){

}
int check_col(int col){

}
int try_move(char choice, int row, int col){
	
}

int solve(const char *initial_state, const char *keys, int size){
	(void) initial_state;
	(void) keys;
	(void) size;
	
	return 1;
	
}