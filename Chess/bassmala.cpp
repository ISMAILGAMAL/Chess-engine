//#include <iostream>
//using namespace std;
//#define el "\n"
//
////--------------------------------global variables----------------------------------//
//int chessboard[8][8] = {
//                       -5,-4,-3,-2,-1,-3,-4,-5,
//                       -6,-6,-6,-6,-6,-6,-6,-6,
//                         0,0,0,0,0,0,0,0,
//                         0,0,0,0,0,0,0,0,
//                         0,0,0,0,0,0,0,0,
//                         0,0,0,0,0,0,0,0,
//                         6,6,6,6,6,6,6,6,
//                         5,4,3,2,1,3,4,5 };
//
//// 1 -> king
//// 2 -> queen
//// 3 -> bishop
//// 4 -> knight
//// 5 -> rook
//// 6 -> pawn 
//
//// White is positive and black is negative.
//
//bool endgame = 0;
//bool turn = 1;
////---------------------------------functions----------------------------------------//
//
//// A function to display the board in the terminal.
//void display_board() {
//    for (int firstrow = 0; firstrow < 8; ++firstrow) {
//        if (!firstrow)cout << "  | ";
//        else {
//            char character = '`' + firstrow;
//            cout << character << "  ";
//        }
//    }
//    cout << el << "---------------------------" << el;
//    for (int row = 1; row < 8; ++row) {
//        for (int colomn = 0; colomn < 8; ++colomn) {
//            cout << chessboard[row][colomn] << " ";
//            if (colomn < 7 && chessboard[row][colomn + 1] >= 0)cout << " ";
//        }
//        cout << el;
//    }
//    cout << el;
//}
//
//
//// The following function are all used to check if the move is valid.
//
//bool king(int row1, int row2, int colomn1, int colomn2) {
//    if (abs(row1 - row2) > 1 || abs(colomn1 - colomn2) > 1)
//        return 0;
//    return 1;
//}
//
//bool queen(int row1, int row2, int colomn1, int colomn2) {
//    if (row1 == row2) {
//        if (colomn1 > colomn2)
//            swap(colomn1, colomn2);
//        for (int i = colomn1 | +1; i < colomn2; i++)
//            if (chessboard[row1][i] != 0)
//                return 0;
//    }
//    else if (colomn1 == colomn2) {
//        if (row1 > row2)
//            swap(row1, row2);
//        for (int i = row1 + 1; i < row2; i++)
//            if (chessboard[i][colomn1] != 0)
//                return 0;
//    }
//    else if (abs(row1 - row2) == abs(colomn1 - colomn2)) {
//        if (colomn1 > colomn2) {
//            swap(colomn1, colomn2);
//            swap(row1, row2);
//        }
//        if (row2 > row1) {
//            for (int i = row1 + 1, j = colomn1 + 1; i < row2 && j < colomn2; i++, j++)
//                if (chessboard[i][j] != 0)
//                    return 0;
//        }
//        else {
//            for (int i = row1 - 1, j = colomn1 + 1; i > row2 && j < colomn2; i--, j++)
//                if (chessboard[i][j] != 0)
//                    return 0;
//        }
//    }
//    else
//        return 0;
//    return 1;
//}
//
//bool bishop(int row1, int row2, int colomn1, int colomn2) {
//    if (abs(row1 - row2) == abs(colomn1 - colomn2)) {
//        if (colomn1 > colomn2) {
//            swap(colomn1, colomn2);
//            swap(row1, row2);
//        }
//        if (row2 > row1) {
//            for (int i = row1 + 1, j = colomn1 + 1; i < row2 && j < colomn2; i++, j++)
//                if (chessboard[i][j] != 0)
//                    return 0;
//        }
//        else {
//            for (int i = row1 - 1, j = colomn1 + 1; i > row2 && j < colomn2; i--, j++)
//                if (chessboard[i][j] != 0)
//                    return 0;
//        }
//    }
//    else
//        return 0;
//    return 1;
//}
//
//bool knight(int row1, int row2, int colomn1, int colomn2) {
//    if (abs(row1 - row2) == 2 && abs(colomn1 - colomn2) == 1)
//        return 1;
//    if (abs(row1 - row2) == 1 && abs(colomn1 - colomn2) == 2)
//        return 1;
//    return 0;
//}
//
//bool rook(int row1, int row2, int colomn1, int colomn2) {
//    if (row1 == row2) {
//        if (colomn1 > colomn2)
//            swap(colomn1, colomn2);
//        for (int i = colomn1 | +1; i < colomn2; i++)
//            if (chessboard[row1][i] != 0)
//                return 0;
//    }
//    else if (colomn1 == colomn2) {
//        if (row1 > row2)
//            swap(row1, row2);
//        for (int i = row1 + 1; i < row2; i++)
//            if (chessboard[i][colomn1] != 0)
//                return 0;
//    }
//    else
//        return 0;
//    return 1;
//}
//
//bool pawn(int row1, int row2, int colomn1, int colomn2) {
//    if (!turn) {
//        swap(colomn1, colomn2);
//        swap(row1, row2);
//    }
//    if (row1 - row2 == 1 && abs(colomn1 - colomn2) == 1 && chessboard[row2][colomn2] < 0)
//        return 1;
//    else if (row1 - row2 == 1 && colomn1 == colomn2)
//        return 1;
//    return 0;
//}
//
//// Function that takes the rows and columns of the two squares (the one we want to move the piece from
////  and the one we want to move the piece to) and checks if that move is valid.
//bool valid_move(int row1, int row2, int colomn1, int colomn2) {
//
//    int from = chessboard[row1][colomn1];
//    int to = chessboard[row2][colomn2];
//    //out of range
//    if (row1 > 8 || row2 > 8 || colomn1 > 8 || colomn2 > 8 || row1 < 1 || row2 < 1 || colomn1 < 1 || colomn2 < 1)
//        return 0;
//    //wrong color
//    if (turn && from <= 0)
//        return 0;
//    if (!turn && from >= 0)
//        return 0;
//    //attack same color
//    if (from * to > 0)
//        return 0;
//    //check king moves
//    if (chessboard[row1][colomn1] == 1 || chessboard[row1][colomn1] == -1) {
//        if (!king(row1, row2, colomn1, colomn2))
//            return 0;
//    }
//    //check queen moves
//    else if (chessboard[row1][colomn1] == 2 || chessboard[row1][colomn1] == -2) {
//        if (!queen(row1, row2, colomn1, colomn2))
//            return 0;
//    }
//    //check bishop moves
//    else if (chessboard[row1][colomn1] == 3 || chessboard[row1][colomn1] == -3) {
//        if (!bishop(row1, row2, colomn1, colomn2))
//            return 0;
//    }
//    //check knight moves
//    else if (chessboard[row1][colomn1] == 4 || chessboard[row1][colomn1] == -4) {
//        if (!knight(row1, row2, colomn1, colomn2))
//            return 0;
//    }
//    //check rook moves
//    else if (chessboard[row1][colomn1] == 5 || chessboard[row1][colomn1] == -5) {
//        if (!rook(row1, row2, colomn1, colomn2))
//            return 0;
//    }
//    //check pawm moves
//    else {
//        if (!pawn(row1, row2, colomn1, colomn2))
//            return 0;
//    }
//
//    return 1;
//}
//
//
//
////-----------------------------------code-------------------------------------------//
//
//int main()
//{
//    display_board();
//    while (!endgame) {
//        if (turn)cout << "player 1 " << el;
//        else cout << "player 2 " << el;
//
//        cout << "Move From ";
//        string move_from;
//        cin >> move_from;
//        cout << " To ";
//        string move_to;
//        cin >> move_to;
//        int row1 = move_from[1] - '0';
//        int row2 = move_to[1] - '0';
//        int colomn1 = move_from[0] - '`';
//        int colomn2 = move_to[0] - '`';
//        cout << row1 << " " << row2 << " " << colomn1 << " " << colomn2 << endl;
//        if (!valid_move(row1, row2, colomn1, colomn2))
//            cout << el << "OOPS! Invalid move" << el << el;
//        else {
//            chessboard[row2][colomn2] = chessboard[row1][colomn1];
//            chessboard[row1][colomn1] = 0;
//            display_board();
//            turn = !turn;
//        }
//
//    }
//}