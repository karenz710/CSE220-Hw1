#ifndef __HW1_H
#define __HW1_H

#define INFO(...) do {fprintf(stderr, "[          ] [ INFO ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr);} while(0)
#define ERROR(...) do {fprintf(stderr, "[          ] [ ERR  ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr);} while(0) 


#define MAX_LENGTH 8
#define MAX_SEQUENCE_CAP  40320

extern int board_size; // global var
extern char board[MAX_LENGTH][MAX_LENGTH]; // global

//Required:
int initialize_board(const char *initial_state, const char *keys, int size);
int solve(const char *initial_state, const char *keys, int size);

//Part 1
void print_board();
int isFull();
int get_move();
int check_dupes_col(char piece, int col);
int check_dupes_row(char piece, int row);
int check_row(char piece, int row, int col);
int check_col(char piece, int row, int col);
int try_move(int row, int col);

//pt2
bool apply_edge_constraint_rule(void);
void set_cell_value(int row, int col, int value);
//testing funcs
void print_possible_pieces_state(void);
void print_possible_pieces_at_cell(int row, int column);
/* Hints from our solution (feel free to use or ignore):
Part 2

void edge_clue_initialization(void);

bool apply_constraint_propagation(int row, int column, int piece);
void apply_process_of_elimination(int row, int column, int piece);

Part 3

void generate_valid_sequences(int valid_sequences[MAX_SEQUENCE_CAP][MAX_LENGTH], bool is_horizontal, int index);
void generate_valid_sequences_helper(int valid_sequences[MAX_SEQUENCE_CAP][MAX_LENGTH], bool is_horizontal, int index, int sequence[MAX_LENGTH], int starts_at);
void generate_filtered_sequences(int filtered_sequences[MAX_SEQUENCE_CAP][MAX_LENGTH], bool is_horizontal, int index);
bool sequence_filtration(bool is_horizontal, int index);
bool apply_sequence_filtration(void);
*/


#endif // HW1_H