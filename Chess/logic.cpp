#pragma once
#include <iostream>
#include <map>
#include <chrono>
#include "pcsq.h"
#include "dataStructures.h"
#include "logic.h"

using namespace std;

// Some helper functions.

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

// A struct that encapsulates an entire game state which helps us to copy and pass 
// new game states to the searching Alpha-beta pruned minimax algorithm without 
// needing complex logic to handle special moves and also allows us to interface with the gui.

void GameState::initialize_board() {

    //  resetting everything by initializing all the board's and array's values to zero.
    black_king = { 0,4 };
    white_king = { 7,4 };
    memset(board, 0, sizeof(board));
    memset(rook_moved, 0, sizeof(rook_moved));
    memset(king_moved, 0, sizeof(king_moved));

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
    memset(rook_moved, true, sizeof(rook_moved));
    memset(king_moved, true, sizeof(king_moved));

    int i = 0, j = 0;

    // Fills the board according to the parsed FEN.
    // You can read more on FENs from here: https://www.chess.com/terms/fen-chess
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
    else player = 0;

    // Assigning castling rights according to the FEN
    for (int i = 0; i < castling_fen.size(); i++) {
        if (castling_fen[i] == 'q') { rook_moved[0] = false; king_moved[0] = false; }
        else if (castling_fen[i] == 'k') { rook_moved[1] = false; king_moved[0] = false; }
        else if (castling_fen[i] == 'Q') { rook_moved[2] = false; king_moved[1] = false; }
        else if (castling_fen[i] == 'K') { rook_moved[3] = false; king_moved[1] = false; }
    }

    // en passant
    if (en_passant_fen.size() > 1) {
        int rank = 8 - int(en_passant_fen[1] - '0'), file = 8 - int(en_passant_fen[0] - 'a');
        en_passant = { {rank, file}, 0 };
    }
}

// The following functions all Generate pseudo-legal moves for the pieces and push it to the object's move vector.

// Generate pseudo-legal moves for the pawn.
void GameState::pawn_moves(int x, int y, int team) {
    myVector<myPair<int, int>> moves;

    if (team == 1) {

        if (in_board(x - 1, y) && board[x - 1][y] == 0) // Moving once.
            moves.push_back({ x - 1, y });
        if (in_board(x - 2, y) && board[x - 2][y] == 0 && board[x - 1][y] == 0 && x == 6) // Moving twice.
            moves.push_back({ x - 2, y });

        if (in_board(x - 1, y - 1) && board[x - 1][y - 1] < 0)
            moves.push_back({ x - 1, y - 1 });                    // Diagonal piece capturing.
        if (in_board(x - 1, y + 1) && board[x - 1][y + 1] < 0)
            moves.push_back({ x - 1, y + 1 });

        if (en_passant.first.first == x - 1 && en_passant.first.second == y - 1)
            moves.push_back({ x - 1, y - 1 });                                   // En passant
        if (en_passant.first.first == x - 1 && en_passant.first.second == y + 1)
            moves.push_back({ x - 1, y + 1 });

        for (int i = 0; i < moves.size(); i++)
            white_possible_moves.push_back(to_uci(x, y, moves[i].first, moves[i].second));
    }
    else {
        if (in_board(x + 1, y) && board[x + 1][y] == 0)
            moves.push_back({ x + 1, y });
        if (in_board(x + 2, y) && board[x + 2][y] == 0 && board[x + 1][y] == 0 && x == 1)
            moves.push_back({ x + 2, y });

        if (in_board(x + 1, y - 1) && board[x + 1][y - 1] > 0)
            moves.push_back({ x + 1, y - 1 });
        if (in_board(x + 1, y + 1) && board[x + 1][y + 1] > 0)
            moves.push_back({ x + 1, y + 1 });

        if (en_passant.first.first == x + 1 && en_passant.first.second == y - 1)
            moves.push_back({ x + 1, y - 1 });
        if (en_passant.first.first == x + 1 && en_passant.first.second == y + 1)
            moves.push_back({ x + 1, y + 1 });

        for (int i = 0; i < moves.size(); i++)
            black_possible_moves.push_back(to_uci(x, y, moves[i].first, moves[i].second));
    }
}

void GameState::rook_moves(int x, int y, int team) {
    int x_offsets[] = { 0, 0, 1, -1 }; // defining an array of offsets to loop over and generate moves  
    int y_offsets[] = { 1, -1, 0, 0 }; // in the four vertical and horizontal directions.
    myVector<myPair<int, int>> moves;

    for (int i = 0; i < 4; i++) {
        int target_x = x + x_offsets[i], target_y = y + y_offsets[i];
        while (in_board(target_x, target_y)) {
            int piece = board[target_x][target_y];
            if (piece * team > 0) break; // breaking before pushing the position of a friendly piece.
            moves.push_back({ target_x, target_y });
            if (piece * team < 0) break; // breaking after pushing the position of exactly one enemy piece.
            target_x += x_offsets[i]; target_y += y_offsets[i];
        }
    }

    if (team == 1) {
        for (int i = 0; i < moves.size(); i++)
            white_possible_moves.push_back(to_uci(x, y, moves[i].first, moves[i].second));
    }
    else {
        for (int i = 0; i < moves.size(); i++)
            black_possible_moves.push_back(to_uci(x, y, moves[i].first, moves[i].second));
    }
}


void GameState::king_moves(int x, int y, int team) {
    int x_offsets[] = { 1, 1, 1, -1, -1, -1, 0, 0 };
    int y_offsets[] = { -1, 0, 1, -1, 0, 1, 1, -1 };
    myVector<myPair<int, int>> moves;

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

    // In the rook_moved boolean array to check for castling rights the rook's indices are as follows:
    // a8 -> 0, h8 -> 1, a1 -> 2, h1 -> 3

    // And the king's in the king_moved array are: Black -> 0, White -> 1

    if (team == 1) {
        if (!board[7][1] && !board[7][2] && !board[7][3] && !rook_moved[2] && !king_moved[1])
            if (!checked(7, 3, 1) && !checked(x, y, 1))
                moves.push_back({ 7, 2 });
        if (!board[7][5] && !board[7][6] && !rook_moved[3] && !king_moved[1])
            if (!checked(7, 5, 1) && !checked(x, y, 1))
                moves.push_back({ 7, 6 });
    }
    else {
        if (!board[0][1] && !board[0][2] && !board[0][3] && !rook_moved[0] && !king_moved[0]) {
            if (!checked(0, 3, -1) && !checked(x, y, -1))
                moves.push_back({ 0, 2 });
        }
        if (!board[0][5] && !board[0][6] && !rook_moved[1] && !king_moved[0]) {
            if (!checked(0, 5, -1) && !checked(x, y, -1))
                moves.push_back({ 0, 6 });
        }
    }

    for (int i = 0; i < 8; i++) {
        int target_x = x + x_offsets[i], target_y = y + y_offsets[i];
        if (in_board(target_x, target_y) && board[target_x][target_y] * team <= 0)
            moves.push_back({ target_x, target_y });
    }

    if (team == 1) {
        for (int i = 0; i < moves.size(); i++)
            white_possible_moves.push_back(to_uci(x, y, moves[i].first, moves[i].second));
    }
    else {
        for (int i = 0; i < moves.size(); i++)
            black_possible_moves.push_back(to_uci(x, y, moves[i].first, moves[i].second));
    }
}


void GameState::knight_moves(int x, int y, int team) {
    int x_offsets[] = { -2, -1, -2, -1, 1, 1, 2, 2 };
    int y_offsets[] = { 1, 2, -1, -2, -2, 2, -1, 1 };
    myVector<myPair<int, int>> moves;

    for (int i = 0; i < 8; i++) {
        int target_x = x + x_offsets[i], target_y = y + y_offsets[i];
        if (in_board(target_x, target_y) && board[target_x][target_y] * team <= 0)
            moves.push_back({ target_x, target_y });
    }

    if (team == 1) {
        for (int i = 0; i < moves.size(); i++)
            white_possible_moves.push_back(to_uci(x, y, moves[i].first, moves[i].second));
    }
    else {
        for (int i = 0; i < moves.size(); i++)
            black_possible_moves.push_back(to_uci(x, y, moves[i].first, moves[i].second));
    }
}


void GameState::bishop_moves(int x, int y, int team) {
    int x_offsets[] = { 1, 1, -1, -1 }; // defining an array of offsets like the rook function but
    int y_offsets[] = { 1, -1, 1, -1 }; // in the four diagonal directions.
    myVector<myPair<int, int>> moves;

    for (int i = 0; i < 4; i++) {
        int target_x = x + x_offsets[i], target_y = y + y_offsets[i];
        while (in_board(target_x, target_y)) {
            int piece = board[target_x][target_y];
            if (piece * team > 0) break;
            moves.push_back({ target_x, target_y });
            if (piece * team < 0) break;
            target_x += x_offsets[i]; target_y += y_offsets[i];
        }
    }
    if (team == 1) {
        for (int i = 0; i < moves.size(); i++)
            white_possible_moves.push_back(to_uci(x, y, moves[i].first, moves[i].second));
    }
    else {
        for (int i = 0; i < moves.size(); i++)
            black_possible_moves.push_back(to_uci(x, y, moves[i].first, moves[i].second));
    }
}


void GameState::queen_moves(int x, int y, int team) {
    int x_offsets[] = { 1, 1, -1, -1, 0, 0, 1, -1 };
    int y_offsets[] = { 1, -1, 1, -1, 1, -1, 0, 0 };
    myVector<myPair<int, int>> moves;

    for (int i = 0; i < 8; i++) {
        int target_x = x + x_offsets[i], target_y = y + y_offsets[i];
        while (in_board(target_x, target_y)) {
            int piece = board[target_x][target_y];
            if (piece * team > 0) break;
            moves.push_back({ target_x, target_y });
            if (piece * team < 0) break;
            target_x += x_offsets[i]; target_y += y_offsets[i];
        }
    }

    if (team == 1) {
        for (int i = 0; i < moves.size(); i++)
            white_possible_moves.push_back(to_uci(x, y, moves[i].first, moves[i].second));
    }
    else {
        for (int i = 0; i < moves.size(); i++)
            black_possible_moves.push_back(to_uci(x, y, moves[i].first, moves[i].second));
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
void GameState::generate_all_possible_moves(int team) {
    // White -> 1 , Black -> -1
    // If the parameter type == 1 then it will only generate moves for white
    // if it was -1 then it will only generate moves for black.
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
void GameState::display_possible_moves() {
    myVector<string> possible;
    if (player == 1) possible = white_possible_moves;
    else  possible = black_possible_moves;
    cout << "Possible moves: " << endl;
    for (int i = 0; i < possible.size(); i++) {
        cout << possible[i] << endl;
    }
}

// Takes the piece from the old position and puts it in the new position.
void GameState::move_piece(int from_x, int from_y, int target_x, int target_y, int piece) {
    board[target_x][target_y] = piece;
    board[from_x][from_y] = 0;
}

// Promoting the pawn to a queen if it reached the other side of the board.
void GameState::promotion(int x, int y) {
    int piece = board[x][y];
    if (piece == 6 && x == 0) board[x][y] = 2;
    else if (piece == -6 && x == 7) board[x][y] = -2;
}

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
void GameState::update_board(int from_x, int from_y, int target_x, int target_y) {
    int piece = board[from_x][from_y];

    move_piece(from_x, from_y, target_x, target_y, piece);

    promotion(target_x, target_y); // Promoting the pawn if possible.

    // Flag the kings as moved to make them lose castling rights
    if (piece == 1) { king_moved[1] = true; white_king = { target_x, target_y }; }
    else if (piece == -1) { king_moved[0] = true; black_king = { target_x, target_y }; }

    if (board[0][0] != -3) rook_moved[0] = true; // Flagging the rooks as moved for castling.
    if (board[0][7] != -3) rook_moved[1] = true;
    if (board[7][0] != 3) rook_moved[2] = true;
    if (board[7][7] != 3) rook_moved[3] = true;

    // Check if the move is a pawn who moved twice to flag
    // an en passant as an available move.
    if (abs(piece) == 6 && abs(target_x - from_x) == 2) {
        if (player == 1) {
            en_passant = { { target_x + 1, target_y }, 1 }; // Flagging the sqaure behind the pawn as open for en passant.
        }
        else {
            en_passant = { { target_x - 1, target_y }, 1 };
        }
    }

    // Check if the move is an en passant.
    else if (target_x == en_passant.first.first && target_y == en_passant.first.second) {
        if (player == 1 && piece == 6) {
            board[target_x + 1][target_y] = 0;
        }
        else if (player == -1 && piece == -6) {
            board[target_x - 1][target_y] = 0;
        }
    }

    // Check if the move is a castling move.
    else if (abs(piece) == 1 && abs(target_y - from_y) == 2) {
        if (player == 1) {
            if (target_y == 6) {
                move_piece(from_x, 7, target_x, 5, 3); // move the rook to the right of thw white king.
            }
            else {
                move_piece(from_x, 0, target_x, 3, 3); // move the rook to the left of the white king.
            }
        }
        else {
            if (target_y == 6) {
                move_piece(from_x, 7, target_x, 5, -3); // move the rook to the right of the black king.
            }
            else {
                move_piece(from_x, 0, target_x, 3, -3); // move the rook to the left of the black king.
            }
        }
    }

    player *= -1;

    if (en_passant.second) en_passant.second = 0; // Keeps the en passant open for one turn then restarts it.
    else en_passant = { {-1, -1}, 0 };

}

// Creates and returns a copy of the current game state after simulating a specific move.
// (Used in checking for checkmates and the minimax recursion).
GameState GameState::simulate_move(int from_x, int from_y, int target_x, int target_y) {
    GameState new_state;

    // Iam implementing the copying myself because if i just use the 
    // default copy constructor or assignment operator it copies
    // the move vectors which wastes a lot of time as they will 
    // be cleared and generated again after every move in the board.

    // Copy relevant member variables
    new_state.player = player;
    new_state.black_king = black_king;
    new_state.white_king = white_king;
    for (int i = 0; i < 4; i++)
        new_state.rook_moved[i] = rook_moved[i];
    for (int i = 0; i < 2; i++)
        new_state.king_moved[i] = king_moved[i];
    new_state.en_passant = en_passant;

    // Copy the board
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            new_state.board[i][j] = board[i][j];
        }
    }

    // Don't copy the white and black possible moves vectors.

    new_state.update_board(from_x, from_y, target_x, target_y);
    return new_state;
}


// checks if the move is legal by simulating a move and checking for checkmates.
bool GameState::check_legal(int from_x, int from_y, int target_x, int target_y, int team) {
    GameState new_state = simulate_move(from_x, from_y, target_x, target_y);

    int kingx, kingy;
    if (team == 1) {
        kingx = new_state.white_king.first;
        kingy = new_state.white_king.second;
    }
    else {
        kingx = new_state.black_king.first;
        kingy = new_state.black_king.second;
    }


    if (new_state.checked(kingx, kingy, team)) { return false; }
    return true;
}

// Checks if the given player has no moves and the king is checked meaning a checkmate.
bool GameState::checkmate(int team) {
    if (team == 1 && white_possible_moves.empty() && checked(white_king.first, white_king.second, 1)) return 1;
    else if (team == -1 && black_possible_moves.empty() && checked(black_king.first, black_king.second, -1)) return 1;
    return 0;
}

bool GameState::stalemate(int team) {
    if ((team == 1 && white_possible_moves.empty()) || (team == -1 && black_possible_moves.empty())) return 1;
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


void Minimax::assign_best_move(GameState& state) {
    //Used to display the score the ai have given to every possible move.
    for (int i = 0; i < move_scores.size(); i++) {
        cout << move_scores[i].second << " " << move_scores[i].first << endl;
    }

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
            int left = black_pawns_row[i - 1], right = black_pawns_row[i + 1];
            if ((i == 0 || left >= row_white || left == -1) && (i == 7 || right >= row_white || right == -1) && row_black == -1) {
                bonus += passedPawnBonuses[row_white];
            }
        }
        else if (team == -1 && row_black != -1) {
            if ((i == 0 || black_pawns_row[i - 1] == -1) && (i == 7 || black_pawns_row[i + 1] == -1)) {
                num_isolated++;
            }

            int left = white_pawns_row[i - 1], right = white_pawns_row[i + 1];
            if ((i == 0 || left <= row_black || left == -1) && (i == 7 || right <= row_black || right == -1) && row_white == -1) {
                bonus -= passedPawnBonuses[7 - row_black];
            }
        }
    }

    penalty = isolatedPawnPenaltyByCount[num_isolated];
    penalty = (team == 1) ? penalty : -penalty;

    return penalty + bonus;
}

int Minimax::evaluation(GameState& state, int depth) {
    int mgEval = 0;
    int egEval = 0;
    int gamePhase = 0;

    int black_pawns_row[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
    int white_pawns_row[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };

    int minor_piece_counter = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int piece = state.board[i][j];
            if (piece != 0) {
                if (piece == 6) white_pawns_row[j] = i;
                else if (piece == -6) black_pawns_row[j] = i;
                mgEval += get_pcsq_value(i, j, piece, false) + piece_values[piece];
                egEval += get_pcsq_value(i, j, piece, true) + piece_values[piece];
                gamePhase += gamephaseInc[abs(piece)];
            }
        }
    }

    int pawnStructure = 0;

    pawnStructure += evaluate_pawns(1, white_pawns_row, black_pawns_row);
    pawnStructure += evaluate_pawns(-1, white_pawns_row, black_pawns_row);

    int mgPhase = min(gamePhase, 24);
    int egPhase = 24 - mgPhase;

    return (mgEval * mgPhase + egEval * egPhase) / 24 + pawnStructure;
}



int Minimax::minimax(GameState& state, int depth, int end_depth, int alpha, int beta) {

    if (depth == end_depth) {
        node_counter++;
        int eval = evaluation(state, depth);
        return eval;
    }

    //if (state.stalemate(state.player)) return 0;

    if (state.player == 1) {
        short Size = state.white_possible_moves.size();
        for (short i = 0; i < Size; i++) {
            string move = state.white_possible_moves[i];

            myPair<int, int> from = to_index(move[0], move[1]);
            myPair<int, int> target = to_index(move[2], move[3]);
            int from_x = from.first, from_y = from.second;
            int target_x = target.first, target_y = target.second;

            GameState new_state = state.simulate_move(from_x, from_y, target_x, target_y);
            if (new_state.checked(new_state.white_king.first, new_state.white_king.second, 1)) continue;
            new_state.black_possible_moves.clear();
            new_state.generate_all_possible_moves(-1);

            int score = minimax(new_state, depth + 1, end_depth, alpha, beta);

            alpha = max(alpha, score);

            current_time = chrono::steady_clock::now();
            duration = chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time);
            if (duration.count() > time_limit && end_depth > least_depth) { broke_early = true; return 0; }

            if (depth == 0) {
                move_scores_in_loop.push_back({ score , move });
            }

            if (score >= beta) return score;


        }
        if (alpha == INT_MIN) {
            if (state.checked(state.white_king.first, state.white_king.second, 1)) return INT_MIN + 10000 * depth;
            else return 0;
        }

        return alpha;
    }
    else {
        short Size = state.black_possible_moves.size();
        for (short i = 0; i < Size; i++) {
            string move = state.black_possible_moves[i];

            myPair<int, int> from = to_index(move[0], move[1]);
            myPair<int, int> target = to_index(move[2], move[3]);
            int from_x = from.first, from_y = from.second;
            int target_x = target.first, target_y = target.second;

            GameState new_state = state.simulate_move(from_x, from_y, target_x, target_y);
            if (new_state.checked(new_state.black_king.first, new_state.black_king.second, -1)) continue;
            new_state.white_possible_moves.clear();
            new_state.generate_all_possible_moves(1);

            int score = minimax(new_state, depth + 1, end_depth, alpha, beta);

            beta = min(beta, score);

            current_time = chrono::steady_clock::now();
            duration = chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time);
            if (duration.count() > time_limit && end_depth > least_depth) { broke_early = true; return 0; }

            if (depth == 0) {
                move_scores_in_loop.push_back({ score , move });
            }

            if (score <= alpha) return score;


        }
        if (beta == INT_MAX) {
            if (state.checked(state.black_king.first, state.black_king.second, -1)) return INT_MAX - 10000 * depth;
            else return 0;
        }

        return beta;
    }
}

void Minimax::iterative_deepening(GameState& state) {
    start_time = chrono::steady_clock::now();

    int depth = 3; broke_early = false;
    while (true) {
        minimax(state, 0, depth);

        current_time = chrono::steady_clock::now();
        duration = chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time);
        if (duration.count() > time_limit && depth > least_depth) { broke_early = true; }

        if (!broke_early) {
            move_scores = move_scores_in_loop;
            move_scores_in_loop.clear();
        }
        else {
            time_in_seconds = duration.count() / 1000.0;
            break;
        }
        depth++;
    }
    reached_depth = depth - broke_early;
    assign_best_move(state);
    move_scores.clear();
}

