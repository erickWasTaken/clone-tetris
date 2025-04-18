#pragma once
#include <raylib.h>
#include <vector>

class Grid{
public:
	Grid();
	void Initialize();
	void Print();
	void Draw();
	void Insert(int pieceX, int pieceY, int shape[16]);
	int grid[20][10];
	int numRows;
	int numCols;
	int cellSize;
	std::vector<Color> colors;

private:
	std::vector<Color> GetCellColors();
};
