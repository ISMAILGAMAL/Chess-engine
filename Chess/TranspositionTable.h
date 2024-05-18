#pragma once
#include <random>
#include "dataStructures.h"

using namespace std;

struct RandomGenerator {
	random_device rd;
	mt19937_64 gen;
	uniform_int_distribution<uint64_t> dist;

	RandomGenerator();
	uint64_t generate64Bits();
};

struct Transposition {
	static constexpr uint8_t Exact = 0;
	static constexpr int Alpha = 1;
	static constexpr int Beta = 2;

	uint64_t key = 0; // Zobrist key of the board
	// The flag contains whether the position has been evaluated or there was a cut-off due to alpha-beta.
	uint8_t flag; // Node type flag: exact, lower bound (Beta), upper bound (alpha)
	uint16_t depth; // Depth of the search
	int value; // Evaluation score
};

struct TranspositionTable {
private:
	RandomGenerator randomGenerator;
public:
	uint64_t pieceKeys[2][7][8][8];
	uint64_t blackToMove;
	myVector<Transposition> table;
	int tableSize;
	int entriesCount = 0, overwrites = 0, collisions;

	TranspositionTable(int sizeMB);
	void initializePieceKeys();
	void storeTransposition(uint64_t key, uint8_t flag, uint16_t depth, int value);
	bool probeTransposition(uint64_t key, Transposition& trans);
	void displayFillPercentage();
	uint64_t generateZobristKey(int board[8][8]);
};