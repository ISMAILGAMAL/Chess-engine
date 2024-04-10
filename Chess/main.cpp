#include <iostream>
#include <vector>
#include <map>

using namespace std;

struct GameState {

    int board[8][8];
    pair<int, int> king_black, king_white;

    // Two boolean arrays to check for castling rights.
    bool rook_moved[4], king_moved[2]; 

    // A pair that keeps track of available en passants which can only be one at a time.
    // The first pair stores the x and y coordinates and the bool is like a one turn counter for resetting the move.
    pair<pair<int, int>, bool> en_passant = { {-1, -1}, 0 };

    // Moves are stored in a map whose key is a pair of x and y values denoting the index in the board
    // and whose value is a vector of pair of x and y values denoting the indices of the legal moves that
    // that specific piece can move to.
    map<pair<int, int>, vector<pair<int, int>>> white_possible_moves, black_possible_moves;
}current_state;

// The pieces are encoded as follows:
//// 1 -> king
//// 2 -> queen
//// 3 -> rook
//// 4 -> knight
//// 5 -> bishop
//// 6 -> pawn 
//// Negative represents black and positive represents white.

void new_board() {
    //  resetting everything by initializing all the board's and array's values to zero.
    king_black = { 0,4 };
    king_white = { 7,4 };
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


// Checks if index is in range of the board.
bool in_board(int x, int y) {
    if (x < 0 || y < 0 || x>7 || y>7)    
        return 0;
    return 1;
}

bool check_legal(int x, int y, int team) {
    // checks if the move is legal by checking for checkmates.

    return 1;
}

// Checks if two pieces are of the same type.
bool same_type(int type, int num) {
    if ((type == 1 && num > 0) || (type == 2 && num < 0))   //  type 1  (+)  / type 2  (-)
        return 1;
    return 0;
}

// The following functions all Generate pseudo-legal moves for the pieces and push it to the global maps
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
            if (check_legal(moves[i].first, moves[i].second, 1))
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
            if (check_legal(moves[i].first, moves[i].second, 2))
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
            if (check_legal(moves[i].first, moves[i].second, 1))
                white_possible_moves[{x, y}].push_back(moves[i]);
    }
    else {
        for (int i = 0; i < moves.size(); i++)
            if (check_legal(moves[i].first, moves[i].second, 2))
                black_possible_moves[{x, y}].push_back(moves[i]);
    }
}

// Generate legal moves for the king.
void king_moves(int x, int y, int team) {
    int x_offsets[] = {1, 1, 1, -1, -1, -1, 0, 0};
    int y_offsets[] = {-1, 0, 1, -1, 0, 1, 1, -1};
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


    // An annoying detail not yet implemented is that
    // The king does not pass through or finish on a square that is attacked by an enemy piece.
    // Waiting for the function that checks for a checkmate.


    if (team == 1) {
        if (!board[7][1] && !board[7][2] && !board[7][3] && !rook_moved[2] && !king_moved[1])
            moves.push_back({7, 2});
        if (!board[7][5] && !board[7][6] && !rook_moved[3] && !king_moved[1])
            moves.push_back({ 7, 6 });
    }
    else {
        if (!board[0][1] && !board[0][2] && !board[0][3] && !rook_moved[0] && !king_moved[0])
            moves.push_back({ 0, 2 });
        if (!board[0][5] && !board[0][6] && !rook_moved[1] && !king_moved[0])
            moves.push_back({ 0, 6 });
    }

    for (int i = 0; i < 8; i++) {
        int target_x = x + x_offsets[i], target_y = y + y_offsets[i];
        if (in_board(target_x, target_y) && !same_type(team, board[target_x][target_y]))
            moves.push_back({ target_x, target_y });
    }

    if (team == 1) {
        for (int i = 0; i < moves.size(); i++)
            if (check_legal(moves[i].first, moves[i].second, 1))
                white_possible_moves[{x, y}].push_back(moves[i]);
    }
    else {
        for (int i = 0; i < moves.size(); i++)
            if (check_legal(moves[i].first, moves[i].second, 2))
                black_possible_moves[{x, y}].push_back(moves[i]);
    }
}

// Generate legal moves for the knight.
void knight_moves(int x, int y, int team) {
    int x_offsets[] = {-2, -1, -2, -1, 1, 1, 2, 2};
    int y_offsets[] = {1, 2, -1, -2, -2, 2, -1, 1};
    vector<pair<int, int>> moves;

    for (int i = 0; i < 8; i++) {
        int target_x = x + x_offsets[i], target_y = y + y_offsets[i];
        if (in_board(target_x, target_y) && !same_type(team, board[target_x][target_y]))
            moves.push_back({ target_x, target_y });
    }

    if (team == 1) {
        for (int i = 0; i < moves.size(); i++)
            if (check_legal(moves[i].first, moves[i].second, 1))
                white_possible_moves[{x, y}].push_back(moves[i]);
    }
    else {
        for (int i = 0; i < moves.size(); i++)
            if (check_legal(moves[i].first, moves[i].second, 2))
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
            if (check_legal(moves[i].first, moves[i].second, 1))
                white_possible_moves[{x, y}].push_back(moves[i]);
    }
    else {
        for (int i = 0; i < moves.size(); i++)
            if (check_legal(moves[i].first, moves[i].second, 2))
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
            if (check_legal(moves[i].first, moves[i].second, 1))
                white_possible_moves[{x, y}].push_back(moves[i]);
    }
    else {
        for (int i = 0; i < moves.size(); i++)
            if (check_legal(moves[i].first, moves[i].second, 2))
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
// in the global white_possible_moves and black_possible_moves.
void generate_all_possible_moves() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int type = board[i][j];
            if (board[i][j] > 0) {
                generate_piece_moves(i, j, 1, type);
            }
            else if (board[i][j] < 0) {
                generate_piece_moves(i, j, 2, abs(type));
            }
        }
    }
}

// A function used for testing and debugging.
void display_possible_moves(int team) {
    map<pair<int, int>, vector<pair<int, int>>> possible;
    if (team == 1) possible = white_possible_moves;
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
void promotion(int x, int y){
    int piece = board[x][y];
    if (piece == 6 && x == 0) board[x][y] = 2;
    else if (piece == -6 && x == 7) board[x][y] = -2;
}

bool checked(int kingx, int kingy, int type) {

    // Checking if a knight threatens the king.
    int knight_x_offsets[] = { -2, -1, -2, -1, 1, 1, 2, 2 };
    int knight_y_offsets[] = { 1, 2, -1, -2, -2, 2, -1, 1 };

    for (int k = 0; k < 8; k++) {
        int tx = kingx + knight_x_offsets[k];
        int ty = kingy + knight_y_offsets[k];
        if (in_board(tx, ty) && board[tx][ty] * type == -1)
            return 1;
    }

    int x_offsets[] = { 0, 0, 1, -1, 1, 1, -1, -1 };
    int y_offsets[] = { 1, -1, 0, 0, 1, -1, 1, -1 };

    // Checking if a sliding piece (Rook, Queen, Bishop) is attacking the king vertically or horizontally.
    for (int i = 0; i < 4; i++) {
        int tx = kingx + x_offsets[i], ty = kingy + y_offsets[i];
        while (in_board(tx, ty)) {
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
            if (board[tx][ty] * type == -5 || board[tx][ty] * type == -2) {
                return 1;
            }
            tx += x_offsets[i]; ty += y_offsets[i];
        }
    }

    // Checking for pawns.
    if (type > 0) {
        if (in_board(kingx - 1, kingy - 1) && board[kingx - 1][kingy - 1] == -6) return 1;
        if (in_board(kingx - 1, kingy + 1) && board[kingx - 1][kingy + 1] == -6) return 1;
    }
    else {
        if (in_board(kingx + 1, kingy - 1) && board[kingx + 1][kingy - 1] == -6) return 1;
        if (in_board(kingx + 1, kingy + 1) && board[kingx + 1][kingy + 1] == -6) return 1;
    }
}

void update_board(int from_x, int from_y, int target_x, int target_y, int team) {
    int piece = board[from_x][from_y];

    move_piece(from_x, from_y, target_x, target_y, piece); 

    promotion(target_x, target_y); // Promoting the pawn if possible.

    if (piece == 1) king_moved[1] = true; // Flag the kings as moved to make them lose castling rights.
    else if (piece == -1) king_moved[0] = true;

    if (board[0][0] != -3) rook_moved[0] = true; // Flagging the rooks as moved for castling.
    if (board[0][7] != -3) rook_moved[1] = true;
    if (board[7][0] != 3) rook_moved[2] = true;
    if (board[7][7] != 3) rook_moved[3] = true;

    // Check if the move is a pawn who moved twice to flag
    // an en passant as an available move.
    if (abs(piece) == 6 && abs(target_x - from_x) == 2) {
        if (team == 1) {
            en_passant = { { target_x + 1, target_y }, 1 }; // Flagging the sqaure behind the pawn as open for en passant.
        }
        else {
            en_passant = { { target_x - 1, target_y }, 1 }; 
        }
    }

    // Check if the move is an en passant.
    else if (target_x == en_passant.first.first && target_y == en_passant.first.second) { 
        if (team == 1) {
            board[target_x + 1][target_y] = 0;
        }
        else {
            board[target_x - 1][target_y] = 0;
        }
    }

    // Check if the move is a castling move.
    else if (abs(piece) == 1 && abs(target_y - from_y) == 2) {
        if (team == 1) {
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

    if (en_passant.second) en_passant.second = 0; // Keeps the en passant open for one turn then restarts it.
    else en_passant = { {-1, -1}, 0 };
    
}


////////// WHAT REMAINS:
// A way to test all the functions and to play a couple of games to test the logic.
// A way to simulate a move and then undo it.
// Checkmates to generate legal moves.
// AI.



int main() {

    new_board();



    int player=1;
    int from_x, from_y, target_x, target_y;
    while(true){
        show();
        generate_all_possible_moves();
        display_possible_moves(player);

        string input;
        cout << "Please input the move in this format (e2e4)..." << endl;
        cin >> input;

        from_y = input[0] - 'a';  from_x = 8 - (input[1] - '0');
        target_y = input[2] - 'a';  target_x = 8 - (input[3] - '0');
        
        bool legal = false;

        map<pair<int, int>, vector<pair<int, int>>> possible;
        if (player == 1) possible = white_possible_moves;
        else  possible = black_possible_moves;

        for (auto& move : possible[{from_x, from_y}]) {
            if (move.first == target_x && move.second == target_y) legal = true;
        }

        white_possible_moves.clear(); black_possible_moves.clear();

        if (!legal) {
            cout << "Sorry, this move is not possible!" << endl;
            continue;
        }

        update_board(from_x, from_y, target_x, target_y, player);

        player^=1;

    }


    return 0;
}