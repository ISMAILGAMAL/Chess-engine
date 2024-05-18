#pragma once
#include "TranspositionTable.h"
#include <iostream>

using namespace std;

RandomGenerator::RandomGenerator() : gen(rd()), dist(0, numeric_limits<uint64_t>::max()) {}

uint64_t RandomGenerator::generate64Bits() {
	return dist(gen);
}

// Initializes the transposition table with the specified size.
TranspositionTable::TranspositionTable(int sizeMB) {
	initializePieceKeys();
	tableSize = (sizeMB * 1024 * 1024) / sizeof(Transposition);
	table.resize(tableSize);
}

// Initialize the zobrist keys used in hashing the transpositions.
void TranspositionTable::initializePieceKeys() {
	blackToMove = randomGenerator.generate64Bits();
	for (int i = 0; i < 2; i++) 
		for (int j = 0; j < 7; j++) 
			for (int k = 0; k < 8; k++) 
				for (int l = 0;l < 8;l++)
					pieceKeys[i][j][k][l] = randomGenerator.generate64Bits();
}

void TranspositionTable::storeTransposition(uint64_t key, uint8_t flag, uint16_t depth, int value) {
	int hash = key % tableSize;

	// First time for this hash.
	if (table[hash].key == 0) {
		entriesCount++;
		table[hash] = { key, flag, depth, value };
	}
	else { // The entry exists in the table.
		int originalHash = hash;

		// Search linearly for the key.
		while (table[hash].key != 0 && table[hash].key != key) {
			hash = (hash + 1) % tableSize;

			// Table is full.
			if (hash == originalHash) return;
		}

		if (hash != originalHash) collisions++;

		if (table[hash].key == 0) {
			entriesCount++;
			table[hash] = { key, flag, depth, value };
		}
		else {
			if (table[hash].depth < depth || (depth == table[hash].depth && flag == Transposition::Exact)) {
				overwrites++;
				table[hash] = { key, flag, depth, value };
			}
		}

	}

	
}

// Checks if the transposition exists in the table.
bool TranspositionTable::probeTransposition(uint64_t key, Transposition& trans) {
	int hash = key % tableSize;
	int originalHash = hash;
	while (table[hash].key != 0) {
		if (table[hash].key == key) {
			trans = table[hash];
			return true;
		}
		
		hash = (hash + 1) % tableSize;

		if (hash == originalHash) return false;
		
	}
	return false;
}

uint64_t TranspositionTable::generateZobristKey(int board[8][8]) {
	uint64_t key = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			int piece = board[i][j];
			if (piece > 0) {
				key ^= pieceKeys[0][piece][i][j];
			}
			else if (piece < 0) {
				key ^= pieceKeys[1][abs(piece)][i][j];
			}
		}
	}

	return key;
}

void TranspositionTable::displayFillPercentage() {
	cout << "Table Occupancy: " << entriesCount << " : " << (double(entriesCount) / double(tableSize)) * 100 << " %" << endl;
	cout << "Table Overwrites:  " << overwrites << endl;
	cout << "Table Collisions:  " << collisions << endl;
}