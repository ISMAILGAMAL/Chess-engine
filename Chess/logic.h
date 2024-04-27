#pragma once
#include<iostream>
#include<map>
#include <chrono>
#include "dataStructures.h"

using namespace std;

bool in_board(int x, int y);
string to_uci(int from_x, int from_y, int target_x, int target_y);
myPair<int, int> to_index(char file, char rank);
char match_to_char(int piece);

// A struct that encapsulates an entire game state which helps us to copy and pass 
// new game states to the searching Alpha-beta pruned minimax algorithm without 
// needing complex logic to handle special moves and also allows us to interface with the gui.

struct GameState {
    int player = 1;
    int board[8][8];
    myPair<int, int> black_king, white_king;

    // Two boolean arrays to check for castling rights.
    bool rook_moved[4], king_moved[2];

    // A custom pair that keeps track of available en passants which can only be one at a time.
    // The first pair stores the x and y coordinates and the bool is like a one turn counter for resetting the move.
    myPair<myPair<int, int>, bool> en_passant = { {-1, -1}, 0 };

    // Moves are stored in a dynamic array containing UCI strings of the pseudo-legal
    // that the specific white or black player can do.
    myVector<string> white_possible_moves, black_possible_moves;

    // The pieces are encoded as follows:
    //// 1 -> king
    //// 2 -> queen
    //// 3 -> rook
    //// 4 -> knight
    //// 5 -> bishop
    //// 6 -> pawn 
    //// Negative represents black and positive represents white.

    void initialize_board();
    void initialize_board(string FEN);
    void pawn_moves(int x, int y, int team);
    void rook_moves(int x, int y, int team);
    void king_moves(int x, int y, int team);
    void knight_moves(int x, int y, int team);
    void bishop_moves(int x, int y, int team);
    void queen_moves(int x, int y, int team);
    void generate_piece_moves(int x, int y, int team, int type);
    void generate_all_possible_moves(int team);
    void display_possible_moves();
    void move_piece(int from_x, int from_y, int target_x, int target_y, int piece);
    void promotion(int x, int y);
    bool checked(int kingx, int kingy, int type);
    void update_board(int from_x, int from_y, int target_x, int target_y);
    GameState simulate_move(int from_x, int from_y, int target_x, int target_y);
    bool check_legal(int from_x, int from_y, int target_x, int target_y, int team);
    bool checkmate(int team);
    bool stalemate(int team);
    void show();
};



struct Minimax {
    map<int, int> piece_values = { {2, 900}, {3, 500}, {4, 300}, {5, 320}, {6, 100},
                          {-2, -900}, {-3, -500}, {-4, -300}, {-5, -320}, {-6, -100} };

    int gamephaseInc[7] = { 0, 0, 4, 2, 1, 1, 0 };

    const int passedPawnBonuses[7] = { 0, 120, 80, 50, 30, 15, 15 };
    const int isolatedPawnPenaltyByCount[9] = { 0, -10, -25, -50, -75, -75, -75, -75, -75 };

    myVector<myPair<int, string>> move_scores, move_scores_in_loop;
    string best_move;
    int node_counter = 0, reached_depth, time_limit = 2500, least_depth = 3;
    long long best_score;
    double time_in_seconds;
    chrono::steady_clock::time_point start_time, current_time;
    chrono::milliseconds duration;
    bool broke_early;

    void assign_best_move(GameState& state);
    int get_pcsq_value(int x, int y, int piece, bool endgame);
    int evaluation(GameState& state, int depth);
    int evaluate_pawns(int team, int white_pawns_row[], int black_pawns_row[]);
    int minimax(GameState& state, int depth = 0, int end_depth = 3, int alpha = INT_MIN, int beta = INT_MAX);
    void iterative_deepening(GameState& state);

};
