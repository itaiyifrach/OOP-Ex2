#pragma once
#include "BattleshipPlayerFromFile.h"
#include <stdlib.h>
#include <direct.h>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream> 
#include <vector>
#include <windows.h>

using namespace std;

#define ROWS_WIDTH 10
#define COLS_WIDTH 10
#define SIZE_SHIP_B 1
#define SIZE_SHIP_P 2
#define SIZE_SHIP_M 3
#define SIZE_SHIP_D 4
#define SHIPS_NUM 5
#define BOARD_SUFFIX "\\*.sboard"
#define ATTACK_SUFFIX "\\*.attack"
#define WRONG_PATH "Wrong path: "
#define MISSING_BOARD "Missing board file (*.sboard) looking in path: "
#define INIT_FAILED "Algorithm initialization failed for dll: "
#define BOARD_MISTAKE_0 "Wrong size or shape for ship "
#define FOR_PLAYER " for player "
#define BOARD_MISTAKE_2 "Too many ships for player A"
#define BOARD_MISTAKE_3 "Too few ships for player A"
#define BOARD_MISTAKE_4 "Too many ships for player B"
#define BOARD_MISTAKE_5 "Too few ships for player B"
#define BOARD_MISTAKE_6 "Adjacent Ships on Board"


/*
function that extracts a .file_suffix file from given path.
return 0 upon success, -1 o/w.
*/
int parsePath(const string& basePath, string& file_path);
//Parse rows*cols matrix from .sboard file to board
void parseBoard(const string& boardPath, char**& board, int rows, int cols);

//Check if input args are valid. If so parse the board and check its validity. Also assign right paths to the attack files of each players
int initialize(int argc, char** argv, char** board, int numRows, int numCols, string& basePath, bool* useAnimation, int* delay);

//Check if the board is valid. if yes returns true, Otherwise returns false and prints to console the relevant mistakes
bool checkBoard(char** board, int* mistakes);
bool checkShape(char** board, char** markedBoard, int posI, int posJ, char shipType, int shipSize, int* mistakes, int player);
bool checkBound(char** board, char shipType, int i, int j, int* mistakes, int player, int* possibleAdj);

//Prints the board to console
void printBoard(char** board);
