#pragma once
#include <iostream>
#include <chrono>
#include "pcsq.h"
#include "dataStructures.h"
#include "logic.h"

using namespace std;


int main() {
    GameState current_state;
    Minimax AI;

    string starting_position = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    current_state.initialize_board();
    int from_x, from_y, target_x, target_y;
        
    while (true) {
        current_state.generate_all_possible_moves(current_state.player);
        current_state.show();

        if (current_state.checkmate(1)) {
            cout << "Checkmate!! Black Wins!!" << endl;
        }
        else if (current_state.checkmate(-1)) {
            cout << "Checkmate!! White Wins!!" << endl;
        }
        else if (current_state.stalemate(current_state.player)){
            cout << "Stalemate! A Draw" << endl;
        }
        if (current_state.player == -1) {
            string input;
            cout << "Please input the move in this format (e2e4)..." << endl;
            cin >> input;

            from_y = input[0] - 'a';  from_x = 8 - (input[1] - '0');
            target_y = input[2] - 'a';  target_x = 8 - (input[3] - '0');
        }
        else {
            AI.iterative_deepening(current_state);
            cout << "Best Move: " << AI.best_move << " Score: " << AI.best_score << endl;

            myPair<int, int> from = to_index(AI.best_move[0], AI.best_move[1]);
            myPair<int, int> target = to_index(AI.best_move[2], AI.best_move[3]);
            from_x = from.first; from_y = from.second;
            target_x = target.first; target_y = target.second;
            cout << "Nodes Evaluated: ";
            cout << AI.node_counter << endl;
            cout << "Depth Reached: " << AI.reached_depth << endl;
            cout << "Time Taken: " << AI.time_in_seconds << endl;
            AI.node_counter = 0;
        }

        // Some Notes:
        // 1- after legal moves optimization possible moves are now only pseudo-legal
        // meaning you would have to check for legality yourself.

        bool legal = false;

        myVector<string> possible;
        if (current_state.player == 1) possible = current_state.white_possible_moves;
        else  possible = current_state.black_possible_moves;


        string move_uci = to_uci(from_x, from_y, target_x, target_y);
        for (int i = 0; i < possible.size();i++) {
            if (possible[i] == move_uci) { legal = true; break; }
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