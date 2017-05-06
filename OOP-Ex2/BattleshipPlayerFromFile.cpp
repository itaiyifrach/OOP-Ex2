#include "BattleshipPlayerFromFile.h"

void BattleshipPlayerFromFile::setBoard(int player, const char** playerBoard, int numRows, int numCols) {
	playerNum = player;
	cols = numCols;
	rows = numRows;	
	this->myBoard = new char*[numRows];
	for (int i = 0; i < numRows; ++i)
	{
		this->myBoard[i] = new char[numCols];
		for (int j = 0; j < numCols; ++j)
			this->myBoard[i][j] = playerBoard[i][j];
	}
}


bool BattleshipPlayerFromFile::init(const std::string& path)
{	
	string attackPath;
	HANDLE dir;
	WIN32_FIND_DATAA fileData; //data struct for file
	vector<string> fileNames; //vactor of name files to sort lexicography
	string file_suffix = ATTACK_SUFFIX; //only *.attack files
	dir = FindFirstFileA((path + file_suffix).c_str(), &fileData); // Notice: Unicode compatible version of FindFirstFile
	if (dir != INVALID_HANDLE_VALUE) //check if the dir opened successfully and found a matching file
	{
		do
		{
			fileNames.push_back(fileData.cFileName);
		} while (FindNextFileA(dir, &fileData));
	}	
	else
	{
		return false;
	}
	//sort the relevant file names lexicography
	sort(fileNames.begin(), fileNames.end());
	// if player == 0 --> assign first *.attack to path
	// else if player == 1 --> assign second *.attack to path
	if(this->playerNum==0)
		attackPath = path + "\\" + fileNames[0];
	else
		if(fileNames.size()>1)
			attackPath = path + "\\" + fileNames[1];
		else
			attackPath = path + "\\" + fileNames[0];
	
	//check if attack-file opening and parsing was successfull (without throwing exceptions from constructor)
	if (parseAttackMoves(attackPath) < 0)
	{
		return false;
	}
	currentAttack = attackMoves.begin();
	return true;
}


int BattleshipPlayerFromFile::parseAttackMoves (string attackPath) {

	this->attackMoves.clear();
	ifstream fin(attackPath);
	if (!fin.is_open())
	{
		std::cout << "Error while open internal file" << std::endl;
		return -1;
	}
	string line;
	int x, y;
	while (getline(fin, line))
	{
		vector<string> tokens = split(line, ',');
		if (tokens.size() != 2)
			continue;
		istringstream firstStream(tokens[0]);
		istringstream secondStream(tokens[1]);
		firstStream >> x;
		secondStream >> y;
		if (firstStream.fail() || secondStream.fail() || x<1 || x>rows || y<1 || y>cols)
			continue;
		pair<int, int> aPair(x, y);
		attackMoves.push_back(aPair);
	}
	fin.close();
	return 0;
}

vector<string> BattleshipPlayerFromFile::split(string str, char delimiter) {
	vector<string> internal;
	stringstream strSt(str); // Turn the string into a stream.
	string item;
	while (getline(strSt, item, delimiter)) {
		internal.push_back(item);
	}
	return internal;
}

std::pair<int, int> BattleshipPlayerFromFile::attack() {
	pair<int, int> result{-1,-1};
	if (currentAttack != attackMoves.end())
	{
		result = *currentAttack;
		++currentAttack;
	}
	return result;
}


void BattleshipPlayerFromFile::notifyOnAttackResult(int player, int row, int col, AttackResult result) {
	//check if it's a self-attack
	bool IwasHit = false;
	if (hasUnHitShipPart(row,col))
		IwasHit = true;
	
	if (IwasHit) {
		if (result == AttackResult::Sink)
		{
			BoardUtils::markSinkedShip(myBoard, rows, cols, row, col, 32);
		}
		else if (result == AttackResult::Hit)
		{
			myBoard[row][col] = 'X';
		}
	}
	
	//for next exsr:
	/*else if (player == playerNum)
	{
		if (result == AttackResult::Miss)
			opponentBoard[row][col] = '%';
		else if (result == AttackResult::Sink) {
			opponentBoard[row][col] = 'X';
			markSinkedShip(opponentBoard, rows, cols, row, col, 'S');
		}
		else
			opponentBoard[row][col] = 'X';
	}*/
}

void BattleshipPlayerFromFile::printPlayerDiscription() const {
	cout << "Player " << playerNum << " board:" << endl;
	printBoard(myBoard);
	printf("===================================");
	cout << endl;
	cout << "Attack moves of player " << playerNum << ":" << endl;
	for (pair<int, int> att : attackMoves) {
		cout << att.first << "," << att.second << endl;
	}
	cout << endl;
}

IBattleshipGameAlgo* GetAlgorithm()
{
	return new BattleshipPlayerFromFile();

}

