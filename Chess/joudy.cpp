//#include <iostream>
//#include <cmath>
//#include <vector>
//#include <string>
//using namespace std;
//char chessBoard[8][8] = {
//    {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},
//    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
//    {'.', '.', '.', '.', '.', '.', '.', '.'},
//    {'.', '.', '.', '.', '.', '.', '.', '.'},
//    {'.', '.', '.', '.', '.', '.', '.', '.'},
//    {'.', '.', '.', '.', '.', '.', '.', '.'},
//    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
//    {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}
//};
//bool black = 0; //keeps track of turns
//void movePiece(int fromX, int fromY, int toX, int toY)
//{
//    chessBoard[toX][toY] = chessBoard[fromX][fromY];
//    chessBoard[fromX][fromY] = '.';
//}
//void Display()
//{
//    for (int rank = 0; rank < 8; rank++)
//    {
//        int index = 8 - rank;
//        cout << index << "   ";
//
//        for (int file = 0; file < 8; file++)
//        {
//            cout << chessBoard[rank][file] << " ";
//        }
//        cout << endl;
//    }
//    cout << endl << "    a b c d e f g h" << endl << endl;
//}
////Pieces' Valid moves
//bool pawnMoved[8][8] = {}; //"visited array" for pawns' first move
//bool pawn(int fromX, int fromY, int toX, int toY)
//{
//    //one OR two steps at first move
//    if ((toY == fromY) && (abs(toX - fromX) == 1))
//    {
//        return (chessBoard[toX][toY] == '.');
//    }
//
//    if ((toY == fromY && abs(toX - fromX) == 2) && !pawnMoved[fromX][fromY])
//    {
//        pawnMoved[fromX][fromY] = 1;
//        return chessBoard[toX][toY] == '.' && chessBoard[(fromX + toX) / 2][toY] == '.';
//    }
//
//    //diagonal capture
//    if (abs(toY - fromY) == 1 && abs(toX - fromX) == 1)
//    {
//        //is destination occupied by opponent?
//        return chessBoard[toX][toY] != '.' && (black ? isupper(chessBoard[toX][toY]) : islower(chessBoard[toX][toY]));
//    }
//    return 0;//invalid move
//}
//bool knight(int fromX, int fromY, int toX, int toY)
//{
//    int rowDiff = abs(toX - fromX), colDiff = abs(toY - fromY);
//
//    if ((rowDiff == 1 && colDiff == 2) || (rowDiff == 2 && colDiff == 1)) return 1;
//
//    return 0;
//}
//bool bishop(int fromX, int fromY, int toX, int toY)
//{
//    if (abs(toX - fromX) != abs(toY - fromY)) return 0;
//
//    int rowDir = (toX - fromX > 0) ? 1 : -1;
//    int colDir = (toY - fromY > 0) ? 1 : -1;
//
//    for (int i = fromX + rowDir, j = fromY + colDir; i != toX; i += rowDir, j += colDir)
//        if (chessBoard[i][j] != '.') return 0;
//
//    if (chessBoard[toX][toY] == '.' || (islower(chessBoard[toX][toY]) && isupper(chessBoard[fromX][fromY])))
//        return 1;
//    return 0;
//}
//bool rook(int fromX, int fromY, int toX, int toY)
//{
//    if (fromX != toX && fromY != toY) return 0;
//
//    int rowDir = (toX - fromX > 0) ? 1 : ((toX - fromX < 0) ? -1 : 0);
//    int colDir = (toY - fromY > 0) ? 1 : ((toY - fromY < 0) ? -1 : 0);
//
//    if (fromX != toX)
//    {
//        for (int i = fromX + rowDir; i != toX; i += rowDir)//move on ranks
//        {
//            if (chessBoard[i][fromY] != '.') return 0;
//        }
//    }
//    else
//    {
//        for (int j = fromY + colDir; j != toY; j += colDir) //move on files
//        {
//            if (chessBoard[fromX][j] != '.') return 0;
//        }
//    }
//    if (chessBoard[toX][toY] == '.' || (islower(chessBoard[toX][toY]) && isupper(chessBoard[fromX][fromY])))
//        return 1;
//    return 0;
//}
//bool queen(int fromX, int fromY, int toX, int toY)
//{
//    int rowDir = (toX - fromX > 0) ? 1 : ((toX - fromX < 0) ? -1 : 0);
//    int colDir = (toY - fromY > 0) ? 1 : ((toY - fromY < 0) ? -1 : 0);
//
//    if (fromX != toX && fromY != toY && abs(fromX - toX) != abs(fromY - toY)) return 0;
//
//    if (fromX != toX)
//    {
//        for (int i = fromX + rowDir; i != toX; i += rowDir)
//        {
//            if (chessBoard[i][fromY] != '.') return 0;
//        }
//    }
//    else if (fromY != toY)
//    {
//        for (int j = fromY + colDir; j != toY; j += colDir)
//        {
//            if (chessBoard[fromX][j] != '.') return 0;
//        }
//    }
//    else
//    {
//        for (int i = fromX + rowDir, j = fromY + colDir; i != toX && j != toY; i += rowDir, j += colDir) //move on diagonals
//        {
//            if (chessBoard[i][j] != '.') return 0;
//        }
//    }
//    if (chessBoard[toX][toY] == '.' || (islower(chessBoard[toX][toY]) && isupper(chessBoard[fromX][fromY])))
//        return 1;
//    return 0;
//}
//bool king(int fromX, int fromY, int toX, int toY)
//{
//    if (abs(fromX - toX) > 1 && abs(fromY - toY) <= 1)//adjacent to pos?
//    {
//        if (chessBoard[toX][toY] == '.' || (islower(chessBoard[toX][toY]) && isupper(chessBoard[fromX][fromY])))
//            return 1;
//    }
//    return 0;
//}
////Overall validity of moves
//bool Valid(int fromX, int fromY, int toX, int toY)
//{
//    char piece = chessBoard[fromX][fromY];
//    char targetPiece = chessBoard[toX][toY];
//
//    if (fromX > 7 || toX > 7 || fromY > 7 || toY > 7 || fromX < 0 || toX < 0 || fromY < 0 || toY < 0) return 0; //out of boundaries
//
//    if (piece == '.' || (isupper(piece) && !black) || (islower(piece) && black)) return 0; //invald piece, picking wrong coloured piece
//
//    if ((isupper(targetPiece) && black) || (islower(targetPiece) && !black)) return 0;//attacking yourself (remember lowercase is white)
//
//    switch (tolower(piece))
//    {
//    case 'p':
//        return pawn(fromX, fromY, toX, toY);
//    case 'n':
//        return knight(fromX, fromY, toX, toY);
//    case 'b':
//        return bishop(fromX, fromY, toX, toY);
//    case 'r':
//        return rook(fromX, fromY, toX, toY);
//    case 'q':
//        return queen(fromX, fromY, toX, toY);
//    case 'k':
//        return king(fromX, fromY, toX, toY);
//    default:
//        return 0;
//    }
//    return 1;
//}
//int main()
//{
//    Display();
//    cout << endl;
//    while (true)
//    {
//        if (!black) cout << "White's turn (lowercase) ";
//        else cout << "Black's turn (uppercase) ";
//
//        string move;
//        cout << "Enter move (e.g., a2 a4): ";
//        getline(cin, move);
//
//        if (move.size() != 5 || move[2] != ' ')
//        {
//            cout << "ERROR, Invalid input format." << endl;
//            continue;
//        }
//
//        int fromX = 8 - (move[1] - '0');
//        int fromY = move[0] - 'a';
//        int toX = 8 - (move[4] - '0');
//        int toY = move[3] - 'a';
//
//        if (fromX < 0 || fromX > 8 || fromY < 0 || fromY > 8 || toX < 0 || toX > 8 || toY < 0 || toY > 8)
//        {
//            cout << "ERROR, Coordinates out of range." << endl;
//            continue;
//        }
//
//        if (Valid(fromX, fromY, toX, toY))
//        {
//            movePiece(fromX, fromY, toX, toY);
//            Display();
//            black = !black;
//        }
//        else cout << "ERROR, Enter a valid move." << endl;
//    }
//
//    return 0;
//}