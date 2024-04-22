# Basic Chess Engine

This project is a basic chess engine implemented in c++ with a gui made using SFML featuring an AI using the traditional minimax algorithm with alpha-beta pruning and iterative deepening.

## Installation:

Copy the repository to your local machine:
```bash
git clone https://github.com/ISMAILGAMAL/Chess-engine.git
```

## Features:

### Move Generation:

The engine is capable of generating both pseudo-legal and legal moves for all pieces in chess including special moves like en-passants, castling and promotions but for promotions it assumes that pawns only promote to queens for simplicity

### A Chess AI:

*The ai in chess is composed of two parts first the search and then the evaluation as the chess bot works by simulating moves up to a certain search depth. Then,  evaluating the resuling positions. After that, it chooses the most promising moves based on the evaluation heuristic.


### 1- Minimax with alpha-beta pruning (The search algorithm):
![Minimax visualization](https://chrisbutner.github.io/ChessCoach/figure1.png)

Source: https://chrisbutner.github.io/ChessCoach/high-level-explanation.html

**The Minimax algorithm is a graph search algorithm that works by recursively exploring the game tree to a certain depth, evaluating each possible leaf or terminal node using some heuristic. It alternates between maximizing and minimizing players, hence the name "minimax."**

**My implementation uses alpha-beta pruning which is a tree pruning technique that reduces the number of nodes the algorithm needs to explore without affecting the final outcome improving the efficiency of the search.**

**It also uses iterative deepening which allows the bot to stop at any time by doing a search up to a depth of one then to a depth of two and so on until a certain time limit i imposed on the bot returning the best move from the last successful seacrh.**


### 2- The evaluation heuristics:

**The evaluation uses a simplified evaluation function proposed by [Tomasz Michniewski](https://www.chessprogramming.org/Tomasz_Michniewski), which consists of two parts: material piece values and piece square tables. I only used the material values [from here](https://www.chessprogramming.org/Simplified_Evaluation_Function). which works by giving each piece a value ex: white rook -> 500, black queen -900 and then adds up all the material values on the board**

**I then used the piece square tables from [PeSTO's Evaluation Function](https://www.chessprogramming.org/PeSTO%27s_Evaluation_Function).**


### A GUI made using SFML:

(NOT YET IMPLEMENTED)

### Online Multiplayer Gameplay:

(NOT YED IMPLEMENTED)