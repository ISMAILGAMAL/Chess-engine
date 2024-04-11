#pragma once
#include <iostream>
#include <vector>
#include <map>

using namespace std;

// Some helper functions.

// Checks if index is in range of the board.
bool in_board(int x, int y) {
    if (x < 0 || y < 0 || x>7 || y>7)
        return 0;
    return 1;
}

// Checks if two pieces are of the same type.
bool same_type(int type, int num) {
    if (type * num > 0)   //  type 1  (+)  / type 2  (-)
        return 1;
    return 0;
}

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

struct GameState {

    int player = 1;
    int board[8][8];
    pair<int, int> black_king, white_king;

    // Two boolean arrays to check for castling rights.
    bool rook_moved[4], king_moved[2]; 

    // A pair that keeps track of available en passants which can only be one at a time.
    // The first pair stores the x and y coordinates and the bool is like a one turn counter for resetting the move.
    pair<pair<int, int>, bool> en_passant = { {-1, -1}, 0 };

    // Moves are stored in a map whose key is a pair of x and y values denoting the index in the board
    // and whose value is a vector of pair of x and y values denoting the indices of the legal moves that
    // that specific piece can move to.
    map<pair<int, int>, vector<pair<int, int>>> white_possible_moves, black_possible_moves;

    // The pieces are encoded as follows:
    //// 1 -> king
    //// 2 -> queen
    //// 3 -> rook
    //// 4 -> knight
    //// 5 -> bishop
    //// 6 -> pawn 
    //// Negative represents black and positive represents white.

    void initialize_board() {

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

    void initialize_board(string FEN) {
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

        // Same as castling but for en passant
        if (en_passant_fen.size() > 1) {
            int rank = 8 - int(en_passant_fen[1] - '0'), file = 8 - int(en_passant_fen[0] - 'a');
            en_passant = { {rank, file}, 0 };
        }
    }

    // The following functions all Generate pseudo-legal moves for the pieces and push it to the object's maps
    // that stores all the possible moves after validiting that it's legal using the check_legal function.

    // Generate legal moves for the pawn.
    void pawn_moves(int x, int y, int team) {
        vector<pair<int, int>> moves;

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
                if (check_legal(x, y, moves[i].first, moves[i].second, 1))
                    white_possible_moves[{x, y}].push_back(moves[i]);
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
                if (check_legal(x, y, moves[i].first, moves[i].second, -1))
                    black_possible_moves[{x, y}].push_back(moves[i]);
        }
    }

    // Generate legal moves for the rook.
    void rook_moves(int x, int y, int team) {
        int x_offsets[] = { 0, 0, 1, -1 }; // defining an array of offsets to loop over and generate moves  
        int y_offsets[] = { 1, -1, 0, 0 }; // in the four vertical and horizontal directions.
        vector<pair<int, int>> moves;

        for (int i = 0; i < 4; i++) {
            int target_x = x + x_offsets[i], target_y = y + y_offsets[i];
            while (in_board(target_x, target_y)) {
                if (same_type(team, board[target_x][target_y])) break; // breaking before pushing the position of a friendly piece.
                moves.push_back({ target_x, target_y });
                if (!same_type(team, board[target_x][target_y]) && board[target_x][target_y] != 0) break; // breaking after pushing the position 
                target_x += x_offsets[i]; target_y += y_offsets[i];                           // of exactly one enemy piece.
            }
        }

        if (team == 1) {
            for (int i = 0; i < moves.size(); i++)
                if (check_legal(x, y, moves[i].first, moves[i].second, 1))
                    white_possible_moves[{x, y}].push_back(moves[i]);
        }
        else {
            for (int i = 0; i < moves.size(); i++)
                if (check_legal(x, y, moves[i].first, moves[i].second, -1))
                    black_possible_moves[{x, y}].push_back(moves[i]);
        }
    }

    // Generate legal moves for the king.
    void king_moves(int x, int y, int team) {
        int x_offsets[] = { 1, 1, 1, -1, -1, -1, 0, 0 };
        int y_offsets[] = { -1, 0, 1, -1, 0, 1, 1, -1 };
        vector<pair<int, int>> moves;

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
            if (!board[0][1] && !board[0][2] && !board[0][3] && !rook_moved[0] && !king_moved[0])
                if (!checked(0, 3, -1) && !checked(x, y, -1))
                    moves.push_back({ 0, 2 });
            if (!board[0][5] && !board[0][6] && !rook_moved[1] && !king_moved[0])
                if (!checked(0, 5, -1) && !checked(x, y, -1))
                    moves.push_back({ 0, 6 });
        }

        for (int i = 0; i < 8; i++) {
            int target_x = x + x_offsets[i], target_y = y + y_offsets[i];
            if (in_board(target_x, target_y) && !same_type(team, board[target_x][target_y]))
                moves.push_back({ target_x, target_y });
        }

        if (team == 1) {
            for (int i = 0; i < moves.size(); i++)
                if (check_legal(x, y, moves[i].first, moves[i].second, 1))
                    white_possible_moves[{x, y}].push_back(moves[i]);
        }
        else {
            for (int i = 0; i < moves.size(); i++)
                if (check_legal(x, y, moves[i].first, moves[i].second, -1))
                    black_possible_moves[{x, y}].push_back(moves[i]);
        }
    }

    // Generate legal moves for the knight.
    void knight_moves(int x, int y, int team) {
        int x_offsets[] = { -2, -1, -2, -1, 1, 1, 2, 2 };
        int y_offsets[] = { 1, 2, -1, -2, -2, 2, -1, 1 };
        vector<pair<int, int>> moves;

        for (int i = 0; i < 8; i++) {
            int target_x = x + x_offsets[i], target_y = y + y_offsets[i];
            if (in_board(target_x, target_y) && !same_type(team, board[target_x][target_y]))
                moves.push_back({ target_x, target_y });
        }

        if (team == 1) {
            for (int i = 0; i < moves.size(); i++)
                if (check_legal(x, y, moves[i].first, moves[i].second, 1))
                    white_possible_moves[{x, y}].push_back(moves[i]);
        }
        else {
            for (int i = 0; i < moves.size(); i++)
                if (check_legal(x, y, moves[i].first, moves[i].second, -1))
                    black_possible_moves[{x, y}].push_back(moves[i]);
        }
    }

    // Generate legal moves for the bishop.
    void bishop_moves(int x, int y, int team) {
        int x_offsets[] = { 1, 1, -1, -1 }; // defining an array of offsets like the rook function but
        int y_offsets[] = { 1, -1, 1, -1 }; // in the four diagonal directions.
        vector<pair<int, int>> moves;

        for (int i = 0; i < 4; i++) {
            int target_x = x + x_offsets[i], target_y = y + y_offsets[i];
            while (in_board(target_x, target_y)) {
                if (same_type(team, board[target_x][target_y])) break;
                moves.push_back({ target_x, target_y });
                if (!same_type(team, board[target_x][target_y]) && board[target_x][target_y] != 0) break;
                target_x += x_offsets[i]; target_y += y_offsets[i];
            }
        }
        if (team == 1) {
            for (int i = 0; i < moves.size(); i++)
                if (check_legal(x, y, moves[i].first, moves[i].second, 1))
                    white_possible_moves[{x, y}].push_back(moves[i]);
        }
        else {
            for (int i = 0; i < moves.size(); i++)
                if (check_legal(x, y, moves[i].first, moves[i].second, -1))
                    black_possible_moves[{x, y}].push_back(moves[i]);
        }
    }

    // Generate legal moves for the queen.
    void queen_moves(int x, int y, int team) {
        int x_offsets[] = { 1, 1, -1, -1, 0, 0, 1, -1 }; // defining an array of offsets like in the bishop and rook functions
        int y_offsets[] = { 1, -1, 1, -1, 1, -1, 0, 0 }; // but combining both their offsets to move in all 8 directions.
        vector<pair<int, int>> moves;

        for (int i = 0; i < 8; i++) {
            int target_x = x + x_offsets[i], target_y = y + y_offsets[i];
            while (in_board(target_x, target_y)) {
                if (same_type(team, board[target_x][target_y])) break;
                moves.push_back({ target_x, target_y });
                if (!same_type(team, board[target_x][target_y]) && board[target_x][target_y] != 0) break;
                target_x += x_offsets[i]; target_y += y_offsets[i];
            }
        }

        if (team == 1) {
            for (int i = 0; i < moves.size(); i++)
                if (check_legal(x, y, moves[i].first, moves[i].second, 1))
                    white_possible_moves[{x, y}].push_back(moves[i]);
        }
        else {
            for (int i = 0; i < moves.size(); i++)
                if (check_legal(x, y, moves[i].first, moves[i].second, -1))
                    black_possible_moves[{x, y}].push_back(moves[i]);
        }
    }


    // Matches the type of a specific piece and calls it's moves generation function.
    void generate_piece_moves(int x, int y, int team, int type) {
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
    // in the object's white_possible_moves and black_possible_moves.
    void generate_all_possible_moves() {
        white_possible_moves.clear(); black_possible_moves.clear();
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                int type = board[i][j];
                if (board[i][j] > 0) {
                    generate_piece_moves(i, j, 1, type);
                }
                else if (board[i][j] < 0) {
                    generate_piece_moves(i, j, -1, abs(type));
                }
            }
        }
    }

    // A function used for testing and debugging.
    void display_possible_moves() {
        map<pair<int, int>, vector<pair<int, int>>> possible;
        if (player == 1) possible = white_possible_moves;
        else  possible = black_possible_moves;
        for (auto item : possible) {
            cout << "Piece: " << char(item.first.second + 'a') << 8 - item.first.first << "  ";
            cout << match_to_char(board[item.first.first][item.first.second]) << "  " << " Can move to: " << endl;
            for (auto pos : item.second) {
                cout << char(pos.second + 'a') << 8 - pos.first << "  " << match_to_char(board[pos.first][pos.second]) << endl;
            }
        }
    }

    // Takes the piece from the old position and puts it in the new position.
    void move_piece(int from_x, int from_y, int target_x, int target_y, int piece) {
        board[target_x][target_y] = piece;
        board[from_x][from_y] = 0;
    }

    // Promoting the pawn to a queen if it reached the other side of the board.
    void promotion(int x, int y) {
        int piece = board[x][y];
        if (piece == 6 && x == 0) board[x][y] = 2;
        else if (piece == -6 && x == 7) board[x][y] = -2;
    }

    // Checks if this position is threatened by an enemy piece.
    bool checked(int kingx, int kingy, int type) {
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
                if (board[tx][ty] * type > 0) break;
                if (board[tx][ty] * type == -3 || board[tx][ty] * type == -2) {
                    return 1;
                }
                tx += x_offsets[i]; ty += y_offsets[i];
            }
        }

        // Checking if a sliding piece (Rook, Queen, Bishop) is attacking the king diagonally.
        for (int i = 4; i < 8; i++) {
            int tx = kingx + x_offsets[i], ty = kingy + y_offsets[i];
            while (in_board(tx, ty)) {
                if (board[tx][ty] * type > 0) break;
                if (board[tx][ty] * type == -5 || board[tx][ty] * type == -2) {
                    return 1;
                }
                tx += x_offsets[i]; ty += y_offsets[i];
            }
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
    void update_board(int from_x, int from_y, int target_x, int target_y) {
        int piece = board[from_x][from_y];

        move_piece(from_x, from_y, target_x, target_y, piece);

        promotion(target_x, target_y); // Promoting the pawn if possible.

        // Flag the kings as moved to make them lose castling rights
        if (piece == 1) { king_moved[1] = true; white_king = { target_x, target_y}; }
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
            if (player == 1) {
                board[target_x + 1][target_y] = 0;
            }
            else {
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

        player ^= 1;

        if (en_passant.second) en_passant.second = 0; // Keeps the en passant open for one turn then restarts it.
        else en_passant = { {-1, -1}, 0 };

    }

    // Creates and returns a copy of the current game state after simulating a specific move.
    // (Used in checking for checkmates and the minimax recursion).
    GameState simulate_move(int from_x, int from_y, int target_x, int target_y) {
        GameState new_state = *this;
        new_state.update_board(from_x, from_y, target_x, target_y);
        return new_state;
    }


    // checks if the move is legal by simulating a move and checking for checkmates.
    bool check_legal(int from_x, int from_y, int target_x, int target_y, int team) {
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


        if (new_state.checked(kingx, kingy, team)) {  return false; }
        return true;
    }

    // Checks if the given player has no moves meaning a checkmate.
    bool checkmate(int color) {
        if ((color == 1 && white_possible_moves.empty()) || (color == 0 && black_possible_moves.empty())) return 1;
        return 0;
    }

    void show() {
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


} current_state;










int evaluation(GameState& state) {

    return int(state.white_possible_moves.size()) - int(state.black_possible_moves.size());
}

pair<int, int> best_move_from, best_move_to;

int minimax(GameState& state, int depth = 0) {

    if (depth == 3) {
        int eval = evaluation(state);
        //cout << eval << endl;
        return eval;
    }

    if (state.player) {
        int maximum = INT_MIN;
        for (auto& piece : state.white_possible_moves) {
            int from_x = piece.first.first, from_y = piece.first.second;
            for (auto& move : piece.second) {
                int target_x = move.first, target_y = move.second;
                GameState new_state = state.simulate_move(from_x, from_y, target_x, target_y);
                new_state.black_possible_moves.clear(); new_state.white_possible_moves.clear();
                new_state.generate_all_possible_moves();
                int score = minimax(new_state, depth + 1);
                if (score > maximum) {
                    maximum = score;
                    if (depth == 0) {
                        best_move_from = piece.first;
                        best_move_to = move;
                    }
                }
            }
        }
        return maximum;
    }
    else {
        int minimum = INT_MAX;

        for (auto& piece : state.black_possible_moves) {
            int from_x = piece.first.first, from_y = piece.first.second;
            for (auto& move : piece.second) {
                int target_x = move.first, target_y = move.second;
                GameState new_state = state.simulate_move(from_x, from_y, target_x, target_y);
                new_state.black_possible_moves.clear(); new_state.white_possible_moves.clear();
                new_state.generate_all_possible_moves();
                int score = minimax(new_state, depth + 1);
                if (score < minimum) {
                    minimum = score;
                    if (depth == 0) {
                        best_move_from = piece.first;
                        best_move_to = move;
                    }
                }
            }
        }
        return minimum;
    }
}



int main() {
    current_state.initialize_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    int player=1;
    int from_x, from_y, target_x, target_y;
    while(true){
        current_state.generate_all_possible_moves();
        current_state.display_possible_moves();
        current_state.show();

        if (current_state.checkmate(1)) {
            cout << "Checkmate!! Black Wins!!" << endl;
        }
        else if (current_state.checkmate(0)) {
            cout << "Checkmate!! White Wins!!" << endl;
        }

        if (current_state.player){
            string input;
            cout << "Please input the move in this format (e2e4)..." << endl;
            cin >> input;

            from_y = input[0] - 'a';  from_x = 8 - (input[1] - '0');
            target_y = input[2] - 'a';  target_x = 8 - (input[3] - '0');
        }
        else {
            minimax(current_state, 0);
            tie(from_x, from_y) = best_move_from;
            tie(target_x, target_y) = best_move_to;
            cout << from_x << " " << from_y << " " << target_x << " " << target_y << endl;
        }
        


        bool legal = false;


        map<pair<int, int>, vector<pair<int, int>>> possible;
        if (current_state.player == 1) possible = current_state.white_possible_moves;
        else  possible = current_state.black_possible_moves;

        for (auto& move : possible[{from_x, from_y}]) {
            if (move.first == target_x && move.second == target_y) legal = true;
        }

        current_state.white_possible_moves.clear(); current_state.black_possible_moves.clear();

        if (!legal) {
            cout << "Sorry, this move is not possible!" << endl;
            continue;
        }

        current_state.update_board(from_x, from_y, target_x, target_y);

    }


    return 0;
}