#pragma once
#include<iostream>
#include <chrono>
#include "dataStructures.h"

using namespace std;

void printBits(uint16_t);
bool in_board(int x, int y);
string to_uci(int from_x, int from_y, int target_x, int target_y);
myPair<int, int> to_index(char file, char rank);
char match_to_char(int piece);

struct Move {
    static constexpr uint16_t None = 0;
    static constexpr uint16_t EnPassant = 1;
    static constexpr uint16_t Castling = 2;
    static constexpr uint16_t Promotion = 3;
    static constexpr uint16_t PawnTwoMoves = 4;

    // Moves are stored as 16 bit numbers divided as follows:
    //    4  +  3  +  3  +  3   +   3  =  16 bits
    // |0000| |000| |000| |000|   |000|
    //  Flag   Toy   ToX  FromY   FromX
    // The flag can be any of the static constexpr above essentially 
    // flagging the move as any of the special moves and the last bit stores if it's a capture or not.
    uint16_t move = 0;

    Move(int fromX, int fromY, int toX, int toY, int flag = 0, bool capture = false);
    Move();

    uint16_t FromX();
    uint16_t FromY();
    uint16_t ToX();
    uint16_t ToY();
    bool IsPromotion();
    bool IsCastle();
    bool IsEnPassant();
    bool IsPawnTwoMoves();
    bool IsCapture();
};

// A struct that encapsulates an entire game state which helps us to copy and pass 
// new game states to the searching Alpha-beta pruned minimax algorithm without 
// needing complex logic to handle special moves and also allows us to interface with the gui.
struct GameState {
    int player = 1;
    int board[8][8] = {};
    myPair<int, int> black_king, white_king;

    uint16_t currentGameState = 0b0000000000001111;
    myVector<uint16_t> gameStateHistory;


    // The first four bits of the currentGameState are the castling rights.
    // |1|  |1|  |1|  |1|
    // BQ   BK   WQ   WK
    static constexpr uint16_t BQueenSide = 8;
    static constexpr uint16_t BKingSide = 4;
    static constexpr uint16_t WQueenSide = 2;
    static constexpr uint16_t WKingSide = 1;

    // Moves are stored in a dynamic array containing 16 bit numbers describing the pseudo-legal
    // moves that the specific white or black player can do.
    myVector<Move> white_possible_moves, black_possible_moves, whiteCaptures, blackCaptures;

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
    void generate_all_possible_moves(int team, bool captures=false);
    //void display_possible_moves();
    bool checked(int kingx, int kingy, int type);
    void makeMove(Move& move);
    void unMakeMove(Move& move);
    bool check_legal(Move& move);
    bool checkMate(int team);
    bool staleMate(int team);
    void show();
    myPair<int, int> enPassant();
    bool canCastle(uint16_t side);
    int capturedPiece();
    Move findMove(int fromX, int fromY, int toX, int toY);
};



struct Minimax {
    int gamephaseInc[7] = { 0, 0, 4, 2, 1, 1, 0 };

    static constexpr int mgValue[7] = { 0, 0, 1025, 477, 337, 365,  82 };
    static constexpr int egValue[7] = { 0, 0, 936, 512, 281, 297,  94 };

    static constexpr int passedPawnBonuses[7] = { 0, 120, 80, 50, 30, 15, 15 };
    static constexpr int isolatedPawnPenaltyByCount[9] = { 0, -10, -25, -50, -75, -75, -75, -75, -75 };

    Minimax();

    myVector<myPair<int, Move>> move_scores, move_scores_in_loop;
    Move best_move;
    int node_counter = 0, reached_depth, time_limit = 3000, least_depth = 4, Q_nodes = 0, quiescenceMaxDepth = 4;
    long long best_score;
    double time_in_seconds;
    chrono::steady_clock::time_point start_time;
    chrono::milliseconds duration;
    bool broke_early = false;

    void display_move_scores();
    void assign_best_move(GameState& state);
    void merge(myVector<myPair<int, Move>>& leftVec, myVector<myPair<int, Move>>& rightVec, myVector<myPair<int, Move>>& vec);
    void mergeSort(myVector<myPair<int, Move>>& vec);
    void sort_moves(GameState& state);
    bool timeLimitExceeded(chrono::steady_clock::time_point& start, chrono::milliseconds& duration, int& depth);
    int get_pcsq_value(int x, int y, int piece, bool endgame);
    int evaluation(GameState& state);
    int evaluate_pawns(int team, int white_pawns_row[], int black_pawns_row[]);
    int minimax(GameState& state, int depth = 0, int end_depth = 3, int alpha = INT_MIN, int beta = INT_MAX);
    void iterative_deepening(GameState& state);
    int quiescenceSearch(GameState& state, int depth, int alpha, int beta);

};

void perftResults(GameState& state, int depth = 0, int end_depth=1);
