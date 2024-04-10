//#include<iostream>
//#include <vector>
//using namespace std;
//int board[8][8];
//
//bool in_board(int x, int y) {
//	if (x < 0 || y < 0 || x>7 || y>7)
//		return 0;
//	return 1;
//}
//
//bool checked(int kingx, int kingy, int type){
//
//	// Checking if a knight threatens the king.
//	int knight_x_offsets[] = { -2, -1, -2, -1, 1, 1, 2, 2 };
//	int knight_y_offsets[] = { 1, 2, -1, -2, -2, 2, -1, 1 };
//
//	for (int k = 0; k < 8; k++){
//		int tx = kingx + knight_x_offsets[k];
//		int ty = kingy + knight_y_offsets[k];
//		if (in_board(tx, ty) && board[tx][ty] * type == -1)
//			return 1;
//	}
//
//	int x_offsets[] = { 0, 0, 1, -1, 1, 1, -1, -1 };
//	int y_offsets[] = { 1, -1, 0, 0, 1, -1, 1, -1 };
//
//	// Checking if a sliding piece (Rook, Queen, Bishop) is attacking the king vertically or horizontally.
//	for (int i = 0; i < 4; i++) {
//		int tx = kingx + x_offsets[i], ty = kingy + y_offsets[i];
//		while (in_board(tx, ty)) {
//			if (board[tx][ty] * type == -3 || board[tx][ty] * type == -2) {
//				return 1;
//			}
//			tx += x_offsets[i]; ty += y_offsets[i];
//		}
//	}
//
//	// Checking if a sliding piece (Rook, Queen, Bishop) is attacking the king diagonally.
//	for (int i = 4; i < 8; i++) {
//		int tx = kingx + x_offsets[i], ty = kingy + y_offsets[i];
//		while (in_board(tx, ty)) {
//			if (board[tx][ty] * type == -5 || board[tx][ty] * type == -2) {
//				return 1;
//			}
//			tx += x_offsets[i]; ty += y_offsets[i];
//		}
//	}
//
//	// Checking for pawns.
//	if (type > 0) {
//		if (in_board(kingx - 1, kingy - 1) && board[kingx - 1][kingy - 1] == -6) return 1;
//		if (in_board(kingx - 1, kingy + 1) && board[kingx - 1][kingy + 1] == -6) return 1;
//	}
//	else {
//		if (in_board(kingx + 1, kingy - 1) && board[kingx + 1][kingy - 1] == -6) return 1;
//		if (in_board(kingx + 1, kingy + 1) && board[kingx + 1][kingy + 1] == -6) return 1;
//	}
//}
//
////would a given move for a piece (for player 1) put the king (of player 1) in check?
//bool can_out_check(int fromx, int fromy, int tox, int toy, int kingx, int kingy) //desired move, king's coordinates 
//{
//	//simulate the move "do it" then get it tested 
//	if (fromx == kingx && fromy == kingy)
//	{
//		kingx = tox;
//		kingy = toy;
//	}
//
//	int kingType = board[kingx][kingy]; //white or black king?
//	int to = board[tox][toy]; //destination square
//	board[tox][toy] = board[fromx][fromy];
//	board[fromx][fromy] = 0;
//
//	bool ret = 1;
//
//	int x_offsets[] = { -2, -1, -2, -1, 1, 1, 2, 2 };
//	int y_offsets[] = { 1, 2, -1, -2, -2, 2, -1, 1 };
//
//	for (int k = 0; k < 8; k++)
//	{
//		int nx = kingx + x_offsets[k];
//		int ny = kingy + y_offsets[k];
//		if (in_board(nx, ny) && board[nx][ny] * kingType == -1)
//		{
//			ret = 0;// knight threatens the king
//			break;
//		}
//	}
//
//	//scanning all eight directions
//	for (int i = kingx + 1; i < 8; i++)
//	{
//		if (board[kingx][i] * kingType == -3 || board[kingx][i] * kingType == -2)
//		{
//			ret = 0;
//			break;
//		}
//		else if (board[kingx][i]) break; //occupied square
//	}
//	if (ret)
//	{
//		for (int i = kingx - 1; i >= 0; i--)
//		{
//			if (board[kingx][i] * kingType == -3 || board[kingx][i] * kingType == -2)
//			{
//				ret = 0;
//				break;
//			}
//			else if (board[kingx][i]) break;
//		}
//	}
//	if (ret)
//	{
//		for (int i = kingy + 1; i < 8; i++)
//		{
//			if (board[i][kingy] * kingType == -3 || board[i][kingy] * kingType == -2)
//			{
//				ret = 0;
//				break;
//			}
//			else if (board[i][kingy]) break;
//		}
//	}
//	if (ret)
//	{
//		for (int i = kingy - 1; i >= 0; i--)
//		{
//			if (board[i][kingy] * kingType == -3 || board[i][kingy] * kingType == -2)
//			{
//				ret = 0;
//				break;
//			}
//			else if (board[i][kingy]) break;
//		}
//	}
//
//	//diagonals -let's handle pawn attacks-
//	if (ret)
//	{
//		for (int i = kingx + 1, j = kingy + 1; i < 8 && j < 8; i++, j++)
//		{
//			//for white pawns on lower right side of black king
//			if ((i == kingx + 1 && kingType == -1 && board[i][j] == 6) || (board[i][j] * kingType == -2 || board[i][j] * kingType == -5))
//			{
//				ret = 0;
//				break;
//			}
//			else if (board[i][j]) break;
//		}
//	}
//
//	if (ret)
//	{
//		for (int i = kingx - 1, j = kingy - 1; i >= 0 && j >= 0; i--, j--)
//		{
//			//for black pawns on upper left side of white king
//			if ((i == kingx - 1 && kingType == 1 && board[i][j] == -6) || (board[i][j] * kingType == -2 || board[i][j] * kingType == -5))
//			{
//				ret = 0;
//				break;
//			}
//			else if (board[i][j]) break;
//		}
//	}
//
//	if (ret)
//	{
//		for (int i = kingx - 1, j = kingy + 1; i >= 0 && j < 8; i--, j++)
//		{
//			//for black pawns on upper right side of white king
//			if ((i == kingx - 1 && kingType == 1 && board[i][j] == -6) || (board[i][j] * kingType == -2 || board[i][j] * kingType == -5))
//			{
//				ret = 0;
//				break;
//			}
//			else if (board[i][j]) break;
//		}
//	}
//
//	if (ret)
//	{
//		for (int i = kingx + 1, j = kingy - 1; i < 8 && j >= 0; i++, j--)
//		{
//			//for white pawns on lower left side of black king
//			if ((i == kingx + 1 && kingType == -1 && board[i][j] == 6) || (board[i][j] * kingType == -2 || board[i][j] * kingType == -5))
//			{
//				ret = 0;
//				break;
//			}
//			else if (board[i][j]) break;
//		}
//	}
//
//	//undo the simulated move
//	if (kingx == tox && kingy == toy)
//	{
//		kingx = fromx;
//		kingy = fromy;
//	}
//	board[fromx][fromy] = board[tox][toy];
//	board[tox][toy] = to;
//
//	return ret; //safe move if true
//}
//
////would a king's move put itself under attack?
//bool not_in_check(int fromx, int fromy, int tox, int toy, int kingx, int kingy)
//{
//
//	int kingType = board[kingx][kingy]; //get side
//	int to = board[tox][toy];//destination square
//	//simulate move
//	board[tox][toy] = board[fromx][fromy];
//	board[fromx][fromy] = 0;
//	bool ret = 1;
//
//	if (fromx == kingx && fromy == kingy) //does the move involve the king itself?
//		ret = !checked(kingx, kingy);//scan all 8 directions for a threat
//
//	else
//	{
//		//check for threats to the king along rank
//		if (fromx == kingx) //is the piece moved on the same rank as the king?  
//		{
//			//iterate over sides of king searching for an attacking piece
//			for (int i = kingx + 1; i < 8; i++)
//				if (board[kingx][i] * kingType == -3 || board[kingx][i] * kingType == -2)
//				{
//					ret = 0;
//					break;
//				}
//				else if (board[kingx][i]) break;
//			if (ret) {
//				for (int i = kingx - 1; i >= 0; i--)
//					if (board[kingx][i] * kingType == -3 || board[kingx][i] * kingType == -2)
//					{
//						ret = 0;
//						break;
//					}
//					else if (board[kingx][i]) break;
//			}
//		}
//		//check for threats to the king along file
//		else if (fromy == kingy) //is the piece moved on the same file as the king?
//		{
//			for (int i = kingy + 1; i < 8; i++)
//				if (board[i][kingy] * kingType == -3 || board[i][kingy] * kingType == -2)
//				{
//					ret = 0;
//					break;
//				}
//				else if (board[kingx][i]) break;
//			if (ret)
//			{
//				for (int i = kingy - 1; i >= 0; i--)
//					if (board[i][kingy] * kingType == -3 || board[i][kingy] * kingType == -2)
//					{
//						ret = 0;
//						break;
//					}
//					else if (board[kingx][i]) break;
//			}
//		}
//		//check for threatsalong the main diagonal 
//		else if (fromx - fromy == kingx - kingy) //on same diagonal (difference is constant)
//		{
//			//scan section of diagonal from king to lower-right corner
//			for (int i = kingx + 1, j = kingy + 1; i < 8 && j < 8; i++, j++)
//				if (board[i][j] * kingType == -2 || board[i][j] * kingType == -5)
//				{
//					ret = 0;
//					break;
//				}
//				else if (board[kingx][i]) break;
//			if (ret)
//			{
//				//scan section of diagonal from king to upper-left corner
//				for (int i = kingx - 1, j = kingy - 1; i >= 0 && j >= 0; i--, j--)
//					if (board[i][j] * kingType == -2 || board[i][j] * kingType == -5)
//					{
//						ret = 0;
//						break;
//					}
//					else if (board[kingx][i]) break;
//			}
//		}
//		//check for threats along the counter-diagonal
//		else if (fromx + fromy == kingx + kingy) //(sum is constant)
//		{
//			//scan section of diagonal from king to upper-right corner
//			for (int i = kingx - 1, j = kingy + 1; i >= 0 && j < 8; i--, j++)
//				if (board[i][j] * kingType == -2 || board[i][j] * kingType == -5)
//				{
//					ret = 0;
//					break;
//				}
//				else if (board[kingx][i]) break;
//			if (ret)
//			{
//				//scan section of diagonal from king to lower-left corner
//				for (int i = kingx + 1, j = kingy - 1; i < 8 && j >= 0; i++, j--)
//					if (board[i][j] * kingType == -2 || board[i][j] * kingType == -5)
//					{
//						ret = 0;
//						break;
//					}
//					else if (board[kingx][i]) break;
//			}
//		}
//	}
//	//restore board to original state
//	board[fromx][fromy] = board[tox][toy];
//	board[tox][toy] = to;
//
//	return ret;//true if move would not leave king in check
//}
//
//
//int main()
//{
//	return 0;
//}
