#pragma once

#include <iostream>
#include "IBattleshipGameAlgo.h"

using namespace std;

class BattleshipPlayerNaive : public IBattleshipGameAlgo
{
	int playerNum;
	int cols;
	int rows;
	char** myBoard;
	pair<int, int> currentAttack;

public:
	BattleshipPlayerNaive () : currentAttack{ 1,1 } {}

	BattleshipPlayerNaive(const BattleshipPlayerNaive&) = delete;

	BattleshipPlayerNaive& operator=(const BattleshipPlayerNaive&) = delete;

	~BattleshipPlayerNaive() {
		delete[](myBoard);
	}

	// called once to notify player on his myBoard
	virtual void setBoard(int player, const char** mainBoard, int numRows, int numCols) override;
	
	// called once to allow init from files if needed returns whether the init succeeded or failed
	virtual bool init(const std::string& path) override
	{
		return true;
	}

	//TODO: should be here?
	// This method must be implemented in each player(algorithm) .cpp file
	ALGO_API IBattleshipGameAlgo* GetAlgorithm();
	
	// ask player for his move
	virtual std::pair<int, int> attack() override;

	// notify on last move result
	virtual void notifyOnAttackResult(int player, int row, int col, AttackResult result) override {}
};