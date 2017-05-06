#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "IBattleshipGameAlgo.h"
#include "BoardUtils.h"
#include "initialize.h"

using namespace std;

class BattleshipPlayerFromFile : public IBattleshipGameAlgo
{
	int playerNum;
	int cols;
	int rows;
	char** myBoard;
	vector<pair<int, int>> attackMoves;
	vector<pair<int, int>>::iterator currentAttack;

public:
	BattleshipPlayerFromFile () {}

	BattleshipPlayerFromFile(const BattleshipPlayerFromFile&) = delete;

	BattleshipPlayerFromFile& operator=(const BattleshipPlayerFromFile&) = delete;

	~BattleshipPlayerFromFile() {
		delete[](myBoard);
	}

	// called once to notify player on his myBoard
	virtual void setBoard(int player, const char** mainBoard, int numRows, int numCols) override;

	// called once to allow init from files if needed returns whether the init succeeded or failed
	virtual bool init(const std::string& path) override;

	//TODO: should be here?
	// This method must be implemented in each player(algorithm) .cpp file
	IBattleshipGameAlgo* GetAlgorithm();

	// ask player for his move
	virtual std::pair<int, int> attack() override;

	// notify on last move result
	virtual void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;

	//for debug purposes
	void printPlayerDiscription() const;

private:
	static vector<string> split(string str, char delimiter);
	int parseAttackMoves(string attackPath);

	//returns true if there is an unhit ship part in (i,j)
	bool hasUnHitShipPart(int i, int j) const
	{
		return ((myBoard[i][j] != 32) && (myBoard[i][j] != 'X'));
	}
};