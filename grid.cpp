#include "grid.hpp"
#include <iostream>

Grid::Grid(){
	numRows = 20;
	numCols = 10;
	cellSize = 30;
	Initialize();
	colors = GetCellColors();
}

void Grid::Initialize(){
	for(int i = 0; i < numRows; i++){
		for(int j = 0; j < numCols; j++){
			grid[i][j] = 0;
		}
	}
}

void Grid::Print(){
	for(int i = 0; i < numRows; i++){
		for(int j = 0; j < numCols; j++){
			std::cout << grid[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

std::vector<Color> Grid::GetCellColors(){
	Color darkGray = {26, 31, 40, 255};
	Color green = {47, 230, 23, 255};
	Color red = {232, 18, 18, 255};
	Color orange = {226, 116, 27, 255};
	Color yellow = {237, 234, 4, 255};
	Color purple = {21, 204, 209, 255};
	Color cyan = {21, 204, 209, 255};
	Color blue = {13, 64, 216, 255};

	return {darkGray, green, red, orange, yellow, purple, cyan, blue};
}

void Grid::Insert(int pieceX, int pieceY, int shape[4][4]){
	int x, y;

	for(x = 0; x < 4; x++){
		for(y = 0; y < 4; y++){
			if(grid[pieceY + y][pieceX + x] != 0)
				continue;
			grid[pieceY + y][pieceX + x] = shape[x][y];
		}
	}
	Draw();
}


void Grid::Draw(){
	for(int i = 0; i < numRows; i++){
		for(int j = 0; j < numCols; j++){
			int cellColor = grid[i][j];
			
			DrawRectangle(j * cellSize +1, i * cellSize +1, cellSize -1, cellSize -1, colors[cellColor]);
		}
	}
}
