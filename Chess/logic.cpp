#pragma once
#include <iostream>
#include <map>
#include <chrono>
#include "pcsq.h"
#include "dataStructures.h"
#include "logic.h"
#include <vector>
#include <algorithm>

using namespace std;

// Some helper functions.

void printBits(uint16_t num) {
    for (int i = 15; i >= 0; i--) {
        if (i == 13 || i == 9 || i == 6 || i == 3) cout << " ";
        cout << (((1 << i) & num) != 0);
    }cout << endl;
}

// Checks if index is in range of the board.
bool in_board(int x, int y) {
    if (x < 0 || y < 0 || x>7 || y>7)
        return 0;
    return 1;
}

// A function that takes the move in indices and turns it to Standard chess UCI
// ex: 1 0 3 0 -> a7a5
string to_uci(int from_x, int from_y, int target_x, int target_y) {
    string s = "";
    s += char(from_y + 'a');
    s += char((8 - from_x) + '0');
    s += char(target_y + 'a');
    s += char((8 - target_x) + '0');
    return s;
}

// This does the opposite of the above and turns two chars (the first or second part of uci)
// to indices to be used inside of the board ex: a7 -> 1 0
myPair<int, int> to_index(char file, char rank) {
    int x, y;
    x = 8 - int(rank - '0');
    y = int(file - 'a');
    return { x, y };
}

// Matches a piece type which we wrote in integers to a char to display in the debugging terminal board.
char match_to_char(int piece) {
    char piece_char;
    if (abs(piece) == 1) piece_char = 'k';
    else if (abs(piece) == 2) piece_char = 'q';
    else if (abs(piece) == 3) piece_char = 'r';
    else if (abs(piece) == 4) piece_char = 'n';
    else if (abs(piece) == 5) piece_char = 'b';
    else if (abs(piece) == 6) piece_char = 'p';
    else return '.';
    if (piece > 0) return toupper(piece_char);
    return piece_char;
}


// Some functions to make using moves more convenient handling all the bitwise operations.

Move::Move(int fromX, int fromY, int toX, int toY, int flag, bool capture) {
    move = 0;
    move |= fromX;
    move |= (fromY << 3);
    move |= (toX << 6);
    move |= (toY << 9);
    move |= (flag << 12);
    if (capture) move |= (1 << 15);
}

Move::Move() {};

uint16_t Move::FromX() {
    return move & 7U;
}

uint16_t Move::FromY() {
    return (move >> 3) & 7U;
}

uint16_t Move::ToX() {
    return (move >> 6) & 7U;
}

uint16_t Move::ToY() {
    return (move >> 9) & 7U;
}

bool Move::IsPromotion() {
    return ((move >> 12) & 7U) == Promotion;
}

bool Move::IsCastle() {
    return ((move >> 12) & 7U) == Castling;
}

bool Move::IsEnPassant() {
    return ((move >> 12) & 7U) == EnPassant;
}

bool Move::IsPawnTwoMoves() {
    return ((move >> 12) & 7U) == PawnTwoMoves;
}

bool Move::IsCapture() {
    return ((move >> 15) & 1);
}

// A struct that encapsulates an entire game state which helps us to copy and pass 
// new game states to the searching Alpha-beta pruned minimax algorithm without 
// needing complex logic to handle special moves and also allows us to interface with the gui.

void GameState::initialize_board() {
    
    //  resetting everything by initializing all the board's and array's values to zero.

    player = 1;
    black_king = { 0,4 };
    white_king = { 7,4 };
    memset(board, 0, sizeof(board));
    currentGameState = 0b0000000000001111;;

    // Initializing the new board to standard beginning chess position.
    int z = 0;
    for (int i = 0; i < 8; i++) {
        board[1][i] = -6;
    }
    for (int i = 0; i < 8; i++) {
        board[6][i] = 6;
    }
    board[0][0] = -3; board[0][7] = -3; // rooks
    board[7][0] = 3; board[7][7] = 3;
    board[0][1] = -4; board[0][6] = -4; // knights
    board[7][1] = 4; board[7][6] = 4;
    board[0][2] = -5; board[0][5] = -5; // bishops
    board[7][2] = 5; board[7][5] = 5;
    board[0][3] = -2;  board[7][3] = 2; // queens
    board[0][4] = -1; board[7][4] = 1; // kings
}

// A constructor that allows us to copy any board fen strings from the internet 
// and initialize the board to that state.
void GameState::initialize_board(string FEN) {
    string board_fen = "", player_fen = "", castling_fen = "", en_passant_fen = "";
    int num_break = 0;

    currentGameState = 0;

    // Parses the fen into four strings.
    for (int i = 0; i < FEN.size(); i++) {
        if (FEN[i] == ' ') { num_break++; continue; }
        if (num_break == 0) board_fen.push_back(FEN[i]);
        else if (num_break == 1) player_fen.push_back(FEN[i]);
        else if (num_break == 2) castling_fen.push_back(FEN[i]);
        else if (num_break == 3) en_passant_fen.push_back(FEN[i]);
        else break;
    }

    memset(board, 0, sizeof(board));

    int i = 0, j = 0;

    // Fills the board according to the parsed FEN.
    // You can read more on FENs from here: https://w...content-available-to-author-only...s.com/terms/fen-chess
    for (int k = 0; k < board_fen.size(); k++) {
        if (board_fen[k] == '/') { j = 0; i++; continue; }
        else if (board_fen[k] < 58) { j += int(board_fen[k] - '0'); continue; }// If it's a number skip that amount of squares.
        else if (toupper(board_fen[k]) == 'K') {
            board[i][j] = 1;
            if (board_fen[k] == 'K') white_king = { i, j };
            else black_king = { i, j };
        }
        else if (toupper(board_fen[k]) == 'Q') board[i][j] = 2;
        else if (toupper(board_fen[k]) == 'R') board[i][j] = 3;
        else if (toupper(board_fen[k]) == 'N') board[i][j] = 4;
        else if (toupper(board_fen[k]) == 'B') board[i][j] = 5;
        else if (toupper(board_fen[k]) == 'P') board[i][j] = 6;
        if (islower(board_fen[k])) board[i][j] *= -1;
        j++;
    }

    // Assigning the player from the parsed FEN
    if (player_fen == "w") player = 1;
    else player = -1;

    // Assigning castling rights according to the FEN
    for (int i = 0; i < castling_fen.size(); i++) {
        if (castling_fen[i] == 'q') { currentGameState |= 8; }
        else if (castling_fen[i] == 'k') { currentGameState |= 4; }
        else if (castling_fen[i] == 'Q') { currentGameState |= 2; }
        else if (castling_fen[i] == 'K') { currentGameState |= 1; }
    }

    // en passant
    if (en_passant_fen.size() > 1) {
        int rank = 8 - int(en_passant_fen[1] - '0'), file = int(en_passant_fen[0] - 'a');
        currentGameState |= (rank << 4);
        currentGameState |= (file << 7);
    }
}

// The following functions all Generate pseudo-legal moves for the pieces and push it to the object's move vector.

// Generate pseudo-legal moves for the pawn.
void GameState::pawn_moves(int x, int y, int team) {
    myPair<int, int> enPassantSq = enPassant();
    uint16_t flag = Move::None;

    if (team == 1) {

        if (x - 1 == 0) flag = Move::Promotion;

        if (in_board(x - 1, y) && board[x - 1][y] == 0) // Moving once.
            white_possible_moves.push_back(Move(x, y, x - 1, y, flag));
        if (in_board(x - 2, y) && board[x - 2][y] == 0 && board[x - 1][y] == 0 && x == 6) // Moving twice.
            white_possible_moves.push_back(Move(x, y, x - 2, y, Move::PawnTwoMoves));

        if (in_board(x - 1, y - 1) && board[x - 1][y - 1] < 0)
            white_possible_moves.push_back(Move(x, y, x - 1, y - 1, flag, true));   // Diagonal piece capturing.
        if (in_board(x - 1, y + 1) && board[x - 1][y + 1] < 0)
            white_possible_moves.push_back(Move(x, y, x - 1, y + 1, flag, true));

        if ((enPassantSq.first != 0 || enPassantSq.second != 0) && enPassantSq.first == 1) {
            if (enPassantSq.first == x - 1 && enPassantSq.second == y - 1)
                white_possible_moves.push_back(Move(x, y, x - 1, y - 1, Move::EnPassant, true));  // En passant
            if (enPassantSq.first == x - 1 && enPassantSq.second == y + 1)
                white_possible_moves.push_back(Move(x, y, x - 1, y + 1, Move::EnPassant, true));
        }

    }
    else {

        if (x + 1 == 7) flag = Move::Promotion;

        if (in_board(x + 1, y) && board[x + 1][y] == 0)
            black_possible_moves.push_back(Move(x, y, x + 1, y, flag));
        if (in_board(x + 2, y) && board[x + 2][y] == 0 && board[x + 1][y] == 0 && x == 1)
            black_possible_moves.push_back(Move(x, y, x + 2, y, Move::PawnTwoMoves));

        if (in_board(x + 1, y - 1) && board[x + 1][y - 1] > 0)
            black_possible_moves.push_back(Move(x, y, x + 1, y - 1, flag, true));
        if (in_board(x + 1, y + 1) && board[x + 1][y + 1] > 0)
            black_possible_moves.push_back(Move(x, y, x + 1, y + 1, flag, true));

        if ((enPassantSq.first != 0 || enPassantSq.second != 0) && enPassantSq.first == 6) {
            if (enPassantSq.first == x + 1 && enPassantSq.second == y - 1)
                black_possible_moves.push_back(Move(x, y, x + 1, y - 1, Move::EnPassant, true));
            if (enPassantSq.first == x + 1 && enPassantSq.second == y + 1)
                black_possible_moves.push_back(Move(x, y, x + 1, y + 1, Move::EnPassant, true));
        }
    }
}

void GameState::rook_moves(int x, int y, int team) {
    int x_offsets[] = { 0, 0, 1, -1 }; // defining an array of offsets to loop over and generate moves  
    int y_offsets[] = { 1, -1, 0, 0 }; // in the four vertical and horizontal directions.

    for (int i = 0; i < 4; i++) {
        int target_x = x + x_offsets[i], target_y = y + y_offsets[i];
        while (in_board(target_x, target_y)) {
            int piece = board[target_x][target_y];
            if (piece * team > 0) break; // breaking before pushing the position of a friendly piece.

            if (team == 1)
                white_possible_moves.push_back(Move(x, y, target_x, target_y, Move::None, (piece != 0)));
            else
                black_possible_moves.push_back(Move(x, y, target_x, target_y, Move::None, (piece != 0)));

            if (piece * team < 0) break; // breaking after pushing the position of exactly one enemy piece.
            target_x += x_offsets[i]; target_y += y_offsets[i];
        }
    }
}


void GameState::king_moves(int x, int y, int team) {
    int x_offsets[] = { 1, 1, 1, -1, -1, -1, 0, 0 };
    int y_offsets[] = { -1, 0, 1, -1, 0, 1, 1, -1 };


    // Castling:
    //      a b c d e f g h
    // 
    //  8   r n b q k b n r
    //  7   p p p p p p p p
    //  6   . . . . . . . .
    //  5   . . . . . . . .
    //  4   . . . . . . . .
    //  3   . . . . . . . .
    //  2   P P P P P P P P
    //  1   R N B Q K B N R
    // BQ -> a8, BK -> h8, WQ -> a1, WK -> h1

    for (int i = 0; i < 8; i++) {
        int target_x = x + x_offsets[i], target_y = y + y_offsets[i];
        int piece = board[target_x][target_y];
        if (in_board(target_x, target_y) && piece * team <= 0) {
            if (team == 1)
                white_possible_moves.push_back(Move(x, y, target_x, target_y, Move::None, (piece != 0)));
            else
                black_possible_moves.push_back(Move(x, y, target_x, target_y, Move::None, (piece != 0)));
        }
    }


    if (team == 1) {
        if (!board[7][1] && !board[7][2] && !board[7][3] && canCastle(WQueenSide))
            if (!checked(7, 3, 1) && !checked(x, y, 1))
                white_possible_moves.push_back(Move(x, y, 7, 2, Move::Castling));
        if (!board[7][5] && !board[7][6] && canCastle(WKingSide))
            if (!checked(7, 5, 1) && !checked(x, y, 1))
                white_possible_moves.push_back(Move(x, y, 7, 6, Move::Castling));
    }
    else {
        if (!board[0][1] && !board[0][2] && !board[0][3] && canCastle(BQueenSide)) {
            if (!checked(0, 3, -1) && !checked(x, y, -1))
                black_possible_moves.push_back(Move(x, y, 0, 2, Move::Castling));
        }
        if (!board[0][5] && !board[0][6] && canCastle(BKingSide)) {
            if (!checked(0, 5, -1) && !checked(x, y, -1))
                black_possible_moves.push_back(Move(x, y, 0, 6, Move::Castling));
        }
    }
}


void GameState::knight_moves(int x, int y, int team) {
    int x_offsets[] = { -2, -1, -2, -1, 1, 1, 2, 2 };
    int y_offsets[] = { 1, 2, -1, -2, -2, 2, -1, 1 };

    for (int i = 0; i < 8; i++) {
        int target_x = x + x_offsets[i], target_y = y + y_offsets[i];
        int piece = board[target_x][target_y];
        if (in_board(target_x, target_y) && piece * team <= 0) {
            if (team == 1)
                white_possible_moves.push_back(Move(x, y, target_x, target_y, Move::None, (piece != 0)));
            else
                black_possible_moves.push_back(Move(x, y, target_x, target_y, Move::None, (piece != 0)));
        }
    }

}


void GameState::bishop_moves(int x, int y, int team) {
    int x_offsets[] = { 1, 1, -1, -1 }; // defining an array of offsets like the rook function but
    int y_offsets[] = { 1, -1, 1, -1 }; // in the four diagonal directions.

    for (int i = 0; i < 4; i++) {
        int target_x = x + x_offsets[i], target_y = y + y_offsets[i];
        while (in_board(target_x, target_y)) {
            int piece = board[target_x][target_y];
            if (piece * team > 0) break;

            if (team == 1)
                white_possible_moves.push_back(Move(x, y, target_x, target_y, Move::None, (piece != 0)));
            else
                black_possible_moves.push_back(Move(x, y, target_x, target_y, Move::None, (piece != 0)));

            if (piece * team < 0) break;
            target_x += x_offsets[i]; target_y += y_offsets[i];
        }
    }
}


void GameState::queen_moves(int x, int y, int team) {
    int x_offsets[] = { 1, 1, -1, -1, 0, 0, 1, -1 };
    int y_offsets[] = { 1, -1, 1, -1, 1, -1, 0, 0 };

    for (int i = 0; i < 8; i++) {
        int target_x = x + x_offsets[i], target_y = y + y_offsets[i];
        while (in_board(target_x, target_y)) {
            int piece = board[target_x][target_y];
            if (piece * team > 0) break;

            if (team == 1)
                white_possible_moves.push_back(Move(x, y, target_x, target_y, Move::None, (piece != 0)));
            else
                black_possible_moves.push_back(Move(x, y, target_x, target_y, Move::None, (piece != 0)));

            if (piece * team < 0) break;
            target_x += x_offsets[i]; target_y += y_offsets[i];
        }
    }
}


// Matches the type of a specific piece and calls it's moves generation function.
void GameState::generate_piece_moves(int x, int y, int team, int type) {
    if (type == 1)
        king_moves(x, y, team);
    else if (type == 2)
        queen_moves(x, y, team);
    else if (type == 3)
        rook_moves(x, y, team);
    else if (type == 4)
        knight_moves(x, y, team);
    else if (type == 5)
        bishop_moves(x, y, team);
    else if (type == 6)
        pawn_moves(x, y, team);
}

// Loops over the board to generate all the possible moves for each piece storing it 
// in the object's white_possible_moves or black_possible_moves.
void GameState::generate_all_possible_moves(int team, bool captures) {
    // White -> 1 , Black -> -1
    // If the parameter type == 1 then it will only generate moves for white
    // if it was -1 then it will only generate moves for black.

    if (team == 1) white_possible_moves.clear();
    else black_possible_moves.clear();

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int type = board[i][j];
            if (board[i][j] > 0 && team == 1) {
                generate_piece_moves(i, j, 1, type);
            }
            else if (board[i][j] < 0 && team != 1) {
                generate_piece_moves(i, j, -1, abs(type));
            }
        }
    }
}

// A function used for testing and debugging.
//void GameState::display_possible_moves() {
    //myVector<string> possible;
    //if (player == 1) possible = white_possible_moves;
    //else  possible = black_possible_moves;
    //cout << "Possible moves: " << endl;
    //for (int i = 0; i < possible.size(); i++) {
        //cout << possible[i] << endl;
    //}
//}


// Checks if this position is threatened by an enemy piece.
bool GameState::checked(int kingx, int kingy, int type) {
    // Checking if a knight threatens the king.
    int knight_x_offsets[] = { -2, -1, -2, -1, 1, 1, 2, 2 };
    int knight_y_offsets[] = { 1, 2, -1, -2, -2, 2, -1, 1 };

    for (int i = 0; i < 8; i++) {
        int tx = kingx + knight_x_offsets[i];
        int ty = kingy + knight_y_offsets[i];
        if (in_board(tx, ty) && board[tx][ty] * type == -4) {
            return 1;
        }
    }

    int x_offsets[] = { 0, 0, 1, -1, 1, 1, -1, -1 };
    int y_offsets[] = { 1, -1, 0, 0, 1, -1, 1, -1 };

    // Checking if a sliding piece (Rook, Queen, Bishop) is attacking the king vertically or horizontally.
    for (int i = 0; i < 4; i++) {
        int tx = kingx + x_offsets[i], ty = kingy + y_offsets[i];
        while (in_board(tx, ty)) {
            int piece = board[tx][ty];
            if (piece * type == -3 || piece * type == -2) {
                return 1;
            }
            if (piece != 0) break;
            tx += x_offsets[i]; ty += y_offsets[i];
        }
    }

    // Checking if a sliding piece (Rook, Queen, Bishop) is attacking the king diagonally.
    for (int i = 4; i < 8; i++) {
        int tx = kingx + x_offsets[i], ty = kingy + y_offsets[i];
        while (in_board(tx, ty)) {
            int piece = board[tx][ty];
            if (piece * type == -5 || piece * type == -2) {
                return 1;
            }
            if (piece != 0) break;
            tx += x_offsets[i]; ty += y_offsets[i];
        }
    }

    // Checking for the other king.
    for (int i = 0; i < 8; i++) {
        int tx = kingx + x_offsets[i], ty = kingy + y_offsets[i];
        if (in_board(tx, ty) && board[tx][ty] * type == -1) return 1;
    }

    // Checking for pawns.
    if (type > 0) {
        if (in_board(kingx - 1, kingy - 1) && board[kingx - 1][kingy - 1] == -6) { return 1; }
        if (in_board(kingx - 1, kingy + 1) && board[kingx - 1][kingy + 1] == -6) { return 1; }
    }
    else {
        if (in_board(kingx + 1, kingy - 1) && board[kingx + 1][kingy - 1] == 6) { return 1; }
        if (in_board(kingx + 1, kingy + 1) && board[kingx + 1][kingy + 1] == 6) { return 1; }
    }

    return 0;
}

// Update the internal representation of the board inside the GameState object 
// while handling special moves like en passants, castling and promotions.
void GameState::makeMove(Move& move) {
    gameStateHistory.push_back(currentGameState);

    int fromX = move.FromX(), fromY = move.FromY();
    int toX = move.ToX(), toY = move.ToY();
    int pieceToMove = board[fromX][fromY], targetPiece = board[toX][toY];

    currentGameState &= ~(63U << 4); // Clearing the enPassant bits.

    // Reassigning captured piece.
    currentGameState &= ~(15U << 10);
    currentGameState |= (abs(targetPiece) << 10);
    if (targetPiece > 0) currentGameState |= (1 << 13);

    board[fromX][fromY] = 0;
    board[toX][toY] = pieceToMove;
    
    // Changing the position of the white and black king used for O(1) access to king positions
    // and changing castling rights if king or rook moved.
    if (pieceToMove == 1) {
        currentGameState &= ~(WQueenSide | WKingSide);
        white_king = { toX, toY };
    }
    else if (pieceToMove == -1) {
        currentGameState &= ~(BQueenSide | BKingSide);
        black_king = { toX, toY };
    }
    else if (pieceToMove == -3){
        if (fromY == 0 && fromX == 0) currentGameState &= ~(BQueenSide);
        else if (fromY == 7 && fromX == 0) currentGameState &= ~(BKingSide);
    }
    else if (pieceToMove == 3) {
        if (fromY == 0 && fromX == 7) currentGameState &= ~(WQueenSide);
        else if (fromY == 7 && fromX == 7) currentGameState &= ~(WKingSide);
    }

    if (move.IsPromotion()) {
        board[toX][toY] = 2 * player;
    }
    else if (move.IsCastle()) {
        // Flag the kings and rooks as moved to make them lose castling rights
        if (toX == 0 && toY == 2) {
            swap(board[0][0], board[0][3]);
        }
        else if (toX == 0 && toY == 6) {
            swap(board[0][7], board[0][5]);
        }
        else if (toX == 7 && toY == 2) {
            swap(board[7][0], board[7][3]);
        }
        else if (toX == 7 && toY == 6) {
            swap(board[7][7], board[7][5]);
        }
    }
    else if (move.IsPawnTwoMoves()) {
        // Check if the move is a pawn who moved twice to flag
        // an en passant as an available move.
        if (player == 1) {
            currentGameState |= ((toX + 1U) << 4); // Flagging the sqaure behind the pawn as open for en passant.
            currentGameState |= (toY << 7); 
        }
        else {
            currentGameState |= ((toX - 1U) << 4);
            currentGameState |= (toY << 7);
        }

    }
    else if (move.IsEnPassant()) {
        if (player == 1) {
            board[toX + 1][toY] = 0;
        }
        else {
            board[toX - 1][toY] = 0;
        }
    }

    player *= -1;
}

void GameState::unMakeMove(Move& move) {
    int fromX = move.FromX(), fromY = move.FromY();
    int toX = move.ToX(), toY = move.ToY();
    int pieceToReturn = board[toX][toY], captured = capturedPiece();

    board[fromX][fromY] = pieceToReturn;
    board[toX][toY] = captured;

    player *= -1;

    if (pieceToReturn == 1) {
        white_king = { fromX, fromY };
    }
    else if (pieceToReturn == -1) {
        black_king = { fromX, fromY };
    }

    if (move.IsPromotion()) {
        board[fromX][fromY] = 6 * player;
    }
    else if (move.IsCastle()) {
        if (toX == 0 && toY == 2) {
            swap(board[0][0], board[0][3]);
        }
        else if (toX == 0 && toY == 6) {
            swap(board[0][7], board[0][5]);
        }
        else if (toX == 7 && toY == 2) {
            swap(board[7][0], board[7][3]);
        }
        else if (toX == 7 && toY == 6) {
            swap(board[7][7], board[7][5]);
        }
    }
    else if (move.IsEnPassant()) {
        if (player == 1) {
            board[toX + 1][toY] = -6;
        }
        else {
            board[toX - 1][toY] = 6;
        }
    }

    currentGameState = gameStateHistory[gameStateHistory.size() - 1];
    gameStateHistory.pop_back();
}


// checks if the move is legal by simulating a move and checking for checkMates.
bool GameState::check_legal(Move& move) { 
    int kingx, kingy, current_player = player;

    makeMove(move);

    if (current_player == 1) {
        kingx = white_king.first;
        kingy = white_king.second;
    }
    else {
        kingx = black_king.first;
        kingy = black_king.second;
    }

    bool isLegal = true;
    if (checked(kingx, kingy, current_player)) isLegal = false;

    unMakeMove(move);

    return isLegal;
}

// Checks if the given player has no moves and the king is checked meaning a checkmate.
bool GameState::checkMate(int team) {

    if (team == 1 && player == 1) {
        size_t size_of_vector = white_possible_moves.size();
        for (int i = 0; i < white_possible_moves.size(); i++)
        {
            Move move = white_possible_moves[i];
            if (!check_legal(move))
                size_of_vector--;
        }
        if (size_of_vector == 0 && checked(white_king.first, white_king.second, 1))
            return 1;
    }
    else if (team == -1 && player == -1){
        size_t size_of_vector = black_possible_moves.size();
        for (int i = 0; i < black_possible_moves.size(); i++)
        {
            Move move = black_possible_moves[i];
            if (!check_legal(move))
                size_of_vector--;
        }
        if (size_of_vector == 0 && checked(black_king.first, black_king.second, -1))
            return 1;
    }
    return 0;
}

bool GameState::staleMate(int team) {
    if (team == 1 && player == 1) {
        size_t size_of_vector = white_possible_moves.size();
        for (int i = 0; i < white_possible_moves.size(); i++)
        {
            Move move = white_possible_moves[i];
            if (!check_legal(move))
                size_of_vector--;
        }
        if (size_of_vector == 0 && !checked(white_king.first, white_king.second, 1))
            return 1;
    }
    if (team == -1 && player == -1) {
        size_t size_of_vector = black_possible_moves.size();
        for (int i = 0; i < black_possible_moves.size(); i++)
        {
            Move move = black_possible_moves[i];
            if (!check_legal(move))
                size_of_vector--;
        }
        if (size_of_vector == 0 && !checked(black_king.first, black_king.second, -1))
            return 1;
    }
    return 0;
}


// Used for debugging.
void GameState::show() {
    cout << "   ";
    for (int i = 0; i < 8; i++)
        cout << char('a' + i) << " ";
    cout << "\n\n";
    for (int i = 0, z = 8; i < 8; i++, z--) {
        cout << z << "  ";
        for (int j = 0; j < 8; j++) {
            cout << match_to_char(board[i][j]) << " ";
        }
        cout << "\n";
    }
}

myPair<int, int> GameState::enPassant() {
    int x = (currentGameState >> 4) & 7;
    int y = (currentGameState >> 7) & 7;

    return { x, y };
}

bool GameState::canCastle(uint16_t side) {
    return (currentGameState & side);
}

int GameState::capturedPiece() {
    int type = (currentGameState >> 10) & 7;
    int color = (currentGameState >> 13) & 1;
    if (color == 1) return type;
    else return -type;
}

Move GameState::findMove(int fromX, int fromY, int toX, int toY) {
    for (int i = 0; i < white_possible_moves.size(); i++) {
        Move move = white_possible_moves[i];
        if (move.FromX() == fromX && move.FromY() == fromY && move.ToX() == toX && move.ToY() == toY) 
            return move;
    }
    for (int i = 0; i < black_possible_moves.size(); i++) {
        Move move = black_possible_moves[i];
        if (move.FromX() == fromX && move.FromY() == fromY && move.ToX() == toX && move.ToY() == toY)
            return move;
    }
}

//Used to display the score the ai have given to every possible move.
void Minimax::display_move_scores() {
    for (int i = 0; i < move_scores.size(); i++) {
        Move move = move_scores[i].second;
        cout << to_uci(move.FromX(), move.FromY(), move.ToX(), move.ToY()) << " " << move_scores[i].first << endl;
    }
}

Minimax::Minimax() {};

void Minimax::assign_best_move(GameState& state) {
    if (state.player == 1) {
        best_score = LLONG_MIN;
        for (int i = 0; i < move_scores.size(); i++) {
            int score = move_scores[i].first;
            if (score > best_score) {
                best_score = score;
                best_move = move_scores[i].second;
            }
        }
    }
    else {
        best_score = LLONG_MAX;
        for (int i = 0; i < move_scores.size(); i++) {
            int score = move_scores[i].first;
            if (score < best_score) {
                best_score = score;
                best_move = move_scores[i].second;
            }
        }
    }
}


void Minimax::merge(myVector<myPair<int, Move>>& leftVec, myVector<myPair<int, Move>>& rightVec, myVector<myPair<int, Move>>& vec) {
    int left = 0, right = 0;
    vec.clear();

    while (left < leftVec.size() && right < rightVec.size()) {
        if (leftVec[left].first < rightVec[right].first) {
            vec.push_back(leftVec[left++]);
        }
        else {
            vec.push_back(rightVec[right++]);
        }
    }

    while (left < leftVec.size()) {
        vec.push_back(leftVec[left++]);
    }

    while (right < rightVec.size()) {
        vec.push_back(rightVec[right++]);
    }
}


// Implements mergeSort to sort the moves in increasing order.
// move oredering is important as we explore the best moves from the previous search depth
// first which helps us prune more branches early on.
void Minimax::mergeSort(myVector<myPair<int, Move>>& vec) {
    size_t size = vec.size();
    if (size <= 1) return;

    int mid = size / 2;

    myVector<myPair<int, Move>> leftVec;
    myVector<myPair<int, Move>> rightVec;

    for (int i = 0; i < mid; i++) {
        leftVec.push_back(vec[i]);
    }
    for (int i = mid; i < size; i++) {
        rightVec.push_back(vec[i]);
    }

    mergeSort(leftVec);
    mergeSort(rightVec);

    merge(leftVec, rightVec, vec);
}


void Minimax::sort_moves(GameState& state) {
    mergeSort(move_scores);
    size_t size = move_scores.size();
    state.white_possible_moves.clear();
    state.black_possible_moves.clear();

    if (state.player == 1) {
        for (int i = size - 1; i >= 0; i--) {
            state.white_possible_moves.push_back(move_scores[i].second);
        }
    }
    else {
        for (int i = 0; i < size; i++) {
            state.black_possible_moves.push_back(move_scores[i].second);
        }
    }
}

bool Minimax::timeLimitExceeded(chrono::steady_clock::time_point& start, chrono::milliseconds& duration, int& depth) {
    auto current_time = chrono::steady_clock::now();
    duration = chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time);
    if (duration.count() > time_limit && depth > least_depth) return true;
    return false;
}

int Minimax::get_pcsq_value(int x, int y, int piece, bool endgame) {
    int pos = x * 8 + y;
    switch (abs(piece)) {
    case 6:
        if (endgame) {
            if (piece > 0) return eg_pawn_table[pos];
            else return -eg_pawn_table[flip[pos]];
        }
        else {
            if (piece > 0) return mg_pawn_table[pos];
            else return -mg_pawn_table[flip[pos]];
        }
    case 5:
        if (endgame) {
            if (piece > 0) return eg_bishop_table[pos];
            else return -eg_bishop_table[flip[pos]];
        }
        else {
            if (piece > 0) return mg_bishop_table[pos];
            else return -mg_bishop_table[flip[pos]];
        }
    case 4:
        if (endgame) {
            if (piece > 0) return eg_knight_table[pos];
            else return -eg_knight_table[flip[pos]];
        }
        else {
            if (piece > 0) return mg_knight_table[pos];
            else return -mg_knight_table[flip[pos]];
        }
    case 3:
        if (endgame) {
            if (piece > 0) return eg_rook_table[pos];
            else return -eg_rook_table[flip[pos]];
        }
        else {
            if (piece > 0) return mg_rook_table[pos];
            else return -mg_rook_table[flip[pos]];
        }
    case 2:
        if (endgame) {
            if (piece > 0) return eg_queen_table[pos];
            else return -eg_queen_table[flip[pos]];
        }
        else {
            if (piece > 0) return mg_queen_table[pos];
            else return -mg_queen_table[flip[pos]];
        }
    case 1:
        if (endgame) {
            if (piece > 0) return eg_king_table[pos];
            else return -eg_king_table[flip[pos]];
        }
        else {
            if (piece > 0) return mg_king_table[pos];
            else return -mg_king_table[flip[pos]];
        }
    }
}

int Minimax::evaluate_pawns(int team, int white_pawns_row[], int black_pawns_row[]) {
    int num_isolated = 0;
    int penalty = 0, bonus = 0;

    for (int i = 0; i < 8; i++) {
        int row_white = white_pawns_row[i], row_black = black_pawns_row[i];
        if (team == 1 && row_white != -1) {
            // Isolated pawns.
            if ((i == 0 || white_pawns_row[i - 1] == -1) && (i == 7 || white_pawns_row[i + 1] == -1)) {
                num_isolated++;
            }

            // Passed pawns.
            bool noOpposingPawns = true;
            if (i > 0 && black_pawns_row[i - 1] < row_white && black_pawns_row[i - 1] != -1) noOpposingPawns = false;
            if (i < 7 && black_pawns_row[i + 1] < row_white && black_pawns_row[i + 1] != -1) noOpposingPawns = false;
            if (black_pawns_row[i] < row_white && black_pawns_row[i] != -1) noOpposingPawns = false;

            if (noOpposingPawns) {
                bonus += passedPawnBonuses[row_white];
            }
        }
        else if (team == -1 && black_pawns_row[i] != -1) {
            if ((i == 0 || black_pawns_row[i - 1] == -1) && (i == 7 || black_pawns_row[i + 1] == -1)) {
                num_isolated++;
            }

            bool noOpposingPawns = true;
            if (i > 0 && white_pawns_row[i - 1] > row_black) noOpposingPawns = false;
            if (i < 7 && white_pawns_row[i + 1] > row_black) noOpposingPawns = false;
            if (white_pawns_row[i] > row_black) noOpposingPawns = false;

            if (noOpposingPawns) {
                bonus -= passedPawnBonuses[7 - row_black];
            }
        }
    }

    penalty = isolatedPawnPenaltyByCount[num_isolated];
    penalty = (team == 1) ? penalty : -penalty;

    return penalty + bonus;
}

int Minimax::evaluation(GameState& state) {
    int mgEval = 0;
    int egEval = 0;
    int gamePhase = 0;
    int black_bishops = 0, white_bishops = 0, bishops_bonus = 0;

    int black_pawns_row[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
    int white_pawns_row[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };

    int minor_piece_counter = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int piece = state.board[i][j];
            if (piece != 0) {
                if (piece == 6) white_pawns_row[j] = i;
                else if (piece == -6) black_pawns_row[j] = i;
                else if (piece == 5) white_bishops++;
                else if (piece == -5) black_bishops++;

                int mgPieceVal = mgValue[abs(piece)], egPieceVal = egValue[abs(piece)];

                if (piece < 0) {
                    mgPieceVal *= -1;
                    egPieceVal *= -1;
                }

                mgEval += get_pcsq_value(i, j, piece, false) + mgPieceVal;
                egEval += get_pcsq_value(i, j, piece, true) + egPieceVal;
                gamePhase += gamephaseInc[abs(piece)];
            }
        }
    }

    if (black_bishops == 2) bishops_bonus -= 150;
    if (white_bishops == 2) bishops_bonus += 150;

    int pawnStructure = 0;

    pawnStructure += evaluate_pawns(1, white_pawns_row, black_pawns_row);
    pawnStructure += evaluate_pawns(-1, white_pawns_row, black_pawns_row);

    int mgPhase = min(gamePhase, 24);
    int egPhase = 24 - mgPhase;

    return (mgEval * mgPhase + egEval * egPhase) / 24 + pawnStructure + bishops_bonus;
}


int Minimax::minimax(GameState& state, int depth, int end_depth, int alpha, int beta) {

    if (depth == end_depth) {
        node_counter++;
        if (state.capturedPiece() && depth >= 4) {
            return quiescenceSearch(state, quiescenceMaxDepth, alpha, beta);
        }
        else {
            int eval = evaluation(state);
            return eval;
        }
    }

    if (state.player == 1) {
        state.generate_all_possible_moves(1);
        myVector<Move> Possible = state.white_possible_moves;
        int bestVal = INT_MIN;
        bool noMoves = true;
        for (int i = 0; i < Possible.size(); i++) {
            Move move = Possible[i];
            state.makeMove(move);

            // Skip the move if it's not legal.
            if (state.checked(state.white_king.first, state.white_king.second, 1)) {
                state.unMakeMove(move);
                continue;
            }

            noMoves = false;

            int score = minimax(state, depth + 1, end_depth, alpha, beta);

            bestVal = max(bestVal, score);

            alpha = max(alpha, bestVal);

            // Saves the moves to be sorted and assigned to best move after the search is finished.
            if (depth == 0) {
                move_scores_in_loop.push_back({ score , move });
            }

            state.unMakeMove(move);
 
            if (beta <= alpha) {
                bestVal = score;
                break;
            }

            // Break if the time limit was exceeded.
            if (timeLimitExceeded(start_time, duration, end_depth)) { broke_early = true; return 0; }

        }

        if (noMoves) {
            if (state.checked(state.white_king.first, state.white_king.second, 1)) {
                return INT_MIN + depth * 10000;
            }
            else return 0;
        }

        return bestVal;
    }
    else {
        state.generate_all_possible_moves(-1);
        myVector<Move> Possible = state.black_possible_moves;
        int bestVal = INT_MAX;
        bool noMoves = true;
        for (int i = 0; i < Possible.size(); i++) {
            Move move = Possible[i];
            state.makeMove(move);

            if (state.checked(state.black_king.first, state.black_king.second, -1)) {
                state.unMakeMove(move);
                continue;
            }

            noMoves = false;

            int score = minimax(state, depth + 1, end_depth, alpha, beta);
            
            bestVal = min(bestVal, score);

            beta = min(beta, score);

            if (depth == 0) {
                move_scores_in_loop.push_back({ score , move });
            }

            //if (bestVal == INT_MIN) {
            //    if (state.checked(state.white_king.first, state.white_king.second, 1)) {
            //        state.unMakeMove(move);
            //        return INT_MIN + (10000 * depth);
            //    }
            //    else {
            //        state.unMakeMove(move);
            //        return 0;
            //    }
            //}

            state.unMakeMove(move);

            if (beta <= alpha) {
                bestVal = score;
                break;
            }

            if (timeLimitExceeded(start_time, duration, end_depth)) { broke_early = true; return 0; }

        }
        if (noMoves) {
            if (state.checked(state.black_king.first, state.black_king.second, -1)) {
                return INT_MAX - depth * 10000;
            }
            else return 0;
        }

        return bestVal;
    }
}

void Minimax::iterative_deepening(GameState& state) {
    node_counter = 0, Q_nodes = 0;
    start_time = chrono::steady_clock::now();
    move_scores.clear(); move_scores_in_loop.clear();

    int depth = 1; broke_early = false;
    while (true) {
        minimax(state, 0, depth);

        if (timeLimitExceeded(start_time, duration, depth)) { broke_early = true; }

        if (!broke_early) {
            move_scores = move_scores_in_loop;
            move_scores_in_loop.clear();
            sort_moves(state);
        }
        else {
            sort_moves(state);
            time_in_seconds = duration.count() / 1000.0;
            break;
        }
        depth++;
    }
    reached_depth = depth - broke_early;
    assign_best_move(state);
}


int Minimax::quiescenceSearch(GameState& state, int depth, int alpha, int beta) {
    int staticEval = evaluation(state);
    Q_nodes++;
    if (depth == 0) return staticEval;

    if (state.player == 1) {
        alpha = max(alpha, staticEval);
    }
    else {
        beta = min(beta, staticEval);
    }

    if (beta <= alpha) return staticEval;


    state.generate_all_possible_moves(state.player);
    myVector<Move> moves;
    if (state.player == 1) {
        //bool whiteChecked = state.checked(state.white_king.first, state.white_king.second, 1);
        moves = state.white_possible_moves;
        int bestVal = staticEval;
        bool noMoves = true;
        for (int i = 0; i < moves.size(); i++) {
            Move move = moves[i];
            state.makeMove(move);

            if (state.checked(state.white_king.first, state.white_king.second, 1)) {
                state.unMakeMove(move);
                continue;
            }

            noMoves = false;

            if (!move.IsCapture()) {
                state.unMakeMove(move);
                continue;
            }

            int score = quiescenceSearch(state, depth-1, alpha, beta);

            bestVal = max(bestVal, score);
            alpha = max(alpha, score);

            state.unMakeMove(move);

            if (beta <= alpha) return alpha;
        }
        if (noMoves) {
            if (state.checked(state.white_king.first, state.white_king.second, 1)) {
                return INT_MIN + (quiescenceMaxDepth - depth) * 10000;
            }
            else return staticEval;
        }

        return bestVal;
    }
    else {
        //bool blackChecked = state.checked(state.black_king.first, state.black_king.second, -1);
        moves = state.black_possible_moves;
        int bestVal = staticEval;
        bool noMoves = true;
        for (int i = 0; i < moves.size(); i++) {
            Move move = moves[i];
            state.makeMove(move);

            if (state.checked(state.black_king.first, state.black_king.second, -1)) {
                state.unMakeMove(move);
                continue;
            }

            noMoves = false;

            if (!move.IsCapture()) {
                state.unMakeMove(move);
                continue;
            }

            int score = quiescenceSearch(state, depth-1, alpha, beta);

            bestVal = min(bestVal, score);
            beta = min(beta, score);

            state.unMakeMove(move);

            if (beta <= alpha) return beta;
        }
        if (noMoves) {
            if (state.checked(state.black_king.first, state.black_king.second, -1)) {
                return INT_MAX - (quiescenceMaxDepth - depth) * 10000;
            }
            else return staticEval;
        }

        return bestVal;
    }
}



int node_counter = 0, capture_counter = 0, check_counter = 0, EP_counter = 0, promotion_counter = 0, castle_counter = 0;

Minimax mini;

void perftResults(GameState& state, int depth, int end_depth) {
    if (depth == end_depth) {
        node_counter++;
        return;
    }

    if (state.player == 1) {
        state.generate_all_possible_moves(1);
        myVector<Move> Possible = state.white_possible_moves;
        for (int i = 0; i < Possible.size(); i++) {
            Move move = Possible[i];
            state.makeMove(move);

            if (state.checked(state.white_king.first, state.white_king.second, 1)) {
                state.unMakeMove(move);
                continue;
            }

            if (depth == end_depth - 1) {
                if (state.capturedPiece() != 0) {
                    capture_counter++;
                }

                if (move.IsCastle()) castle_counter++;
                if (move.IsEnPassant()) { EP_counter++; capture_counter++; }
                if (move.IsPromotion()) promotion_counter+=4;


                if (state.checked(state.black_king.first, state.black_king.second, -1)) {
                    check_counter++;
                }
            }

            perftResults(state, depth + 1, end_depth);
            state.unMakeMove(move);
        }
    }
    else {
        state.generate_all_possible_moves(-1);
        myVector<Move> Possible = state.black_possible_moves;
        for (int i = 0; i < Possible.size(); i++) {
            Move move = Possible[i];
            state.makeMove(move);

            if (state.checked(state.black_king.first, state.black_king.second, -1)) {
                state.unMakeMove(move);
                continue;
            }

            if (depth == end_depth - 1) {
                if (state.capturedPiece() != 0) {
                    capture_counter++;
                }

                if (move.IsCastle()) castle_counter++;
                if (move.IsEnPassant()) { EP_counter++; capture_counter++; }
                if (move.IsPromotion()) promotion_counter+=4;


                if (state.checked(state.white_king.first, state.white_king.second, 1)) {
                    check_counter++;
                }
            }

            perftResults(state, depth + 1, end_depth);
            state.unMakeMove(move);;
        }
    }

    if (depth == 0) {
        cout << "Depth: " << end_depth << endl;
        cout << "Nodes: " << node_counter << endl;
        cout << "Captures: " << capture_counter << endl;
        cout << "EP: " << EP_counter << endl;
        cout << "Castles: " << castle_counter << endl;
        cout << "Promotions: " << promotion_counter << endl;
        cout << "Checks: " << check_counter << endl;
        node_counter = 0, capture_counter = 0, check_counter = 0, EP_counter = 0, promotion_counter = 0, castle_counter = 0;
    }
}