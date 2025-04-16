#include <raylib.h>
#include "grid.hpp"
#include <iostream>
#include <ctime>

#define RIGHT 201
#define LEFT 160
#define TOP 30
#define DOWN 203
#define REFRESH 0

int MovePiece(int direction);
bool DetectCollision(int direction);
void DrawFallingPiece();
void GeneratePiece();

Color bgColor = {44, 44, 127, 255}; // dark blue

Grid grid;
int pieceX;
int pieceY;
int pieceMatrix[4][4];
	
int counter;
int speed = 35;

int shapeI[4][4] = {
	0,1,0,0,
	0,1,0,0,
	0,1,0,0,
	0,1,0,0
};

int shapeT[4][4] = {
	0,1,0,0,
	0,1,1,0,
	0,1,0,0,
	0,0,0,0
};

int shapeL[4][4] = {
	0,0,1,0,
	1,1,1,0,
	0,0,0,0,
	0,0,0,0
};

int shapeL2[4][4] = {
	1,0,0,0,
	1,1,1,0,
	0,0,0,0,
	0,0,0,0
};

int shapeS[4][4] = {
	0,1,1,0,
	1,1,0,0,
	0,0,0,0,
	0,0,0,0
};

int shapeS2[4][4] = {
	1,1,0,0,
	0,1,1,0,
	0,0,0,0,
	0,0,0,0
};

int shapeO[4][4] = {
	1,1,0,0,
	1,1,0,0,
	0,0,0,0,
	0,0,0,0
};

int MovePiece(int direction){
	switch(direction){
		case RIGHT:
			if(DetectCollision(RIGHT))
				return 1;
			pieceX++;
			break;
		case LEFT:
			if(DetectCollision(LEFT))
				return 1;
			pieceX--;
			break;
		case DOWN:
			if(DetectCollision(DOWN))
				return 1;
			pieceY++;
			break;
		case REFRESH:
			if(DetectCollision(REFRESH))
				return 1;
			break;
	}
	return 0;
}

void DrawFallingPiece(){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			// grid.grid[pieceY + j][pieceX + i] = pieceMatrix[i][j];
			if(pieceMatrix[i][j] == 0)
				continue;
			else{
				DrawRectangle((pieceX + i) * grid.cellSize + 1, (pieceY + j) * grid.cellSize +1, grid.cellSize -1, grid.cellSize -1, grid.colors[pieceMatrix[i][j]]);
			}
		}
	}
}

bool DetectCollision(int direction){
	int x, y;
	int pX = pieceX;
	int pY = pieceY;
	bool collision = false;

	switch(direction){
		case RIGHT:
			pX++;
			break;
		case LEFT:
			pX--;
			break;
		case DOWN:
			pY++;
			break;
		default:
			break;
	}

	// check left boundaries
	if(pX < 0){
		for(x = 0; x < 4; x++)
			for(y = 0; y < 4; y++)
				if(pieceMatrix[y][x] != 0)
					collision = true;
	}

	// check right boundaries
	if(pX > grid.numCols - 4){
		for(x = pX + 3; x >= grid.numCols; x--)
			for(y = 0; y < 4; y++)
				if(pieceMatrix[y][x - pX] != 0)
					collision = true;
	}

	// check bottom
	for(x = 0; x < 4; x++)
		for(y = 0; y < 4; y++)
			if(grid.grid[pY + y][pX + x] != 0 && pieceMatrix[x][y] != 0 || ((pY + y) >= grid.numRows && pieceMatrix[x][y] != 0))
				collision = true;

	// std::cout << "collision: " << collision << "\npyy: " << (pY + y) << std::endl;
	return collision;
}

void GeneratePiece(){
	srand(time(0));

	int color = (rand() % 6) + 1; //start at 1
	int shape = rand() % 6;

	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			switch(shape){
				case 0:
					pieceMatrix[j][i] = shapeI[j][i] * color;
					break;
				case 1:
					pieceMatrix[j][i] = shapeL[j][i] * color;
					break;
				case 2:
					pieceMatrix[j][i] = shapeL2[j][i] * color;
					break;
				case 3:
					pieceMatrix[j][i] = shapeO[j][i] * color;
					break;
				case 4:
					pieceMatrix[j][i] = shapeS[j][i] * color;
					break;
				case 5:
					pieceMatrix[j][i] = shapeS2[j][i] * color;
					break;
				case 6:
					pieceMatrix[j][i] = shapeT[j][i] * color;
					break;
			}
			std::cout << pieceMatrix[j][i] * color << ", ";
		}
		std::cout << std::endl;
	}

	// std::cout << "color value: " << color << std::endl;
}

int main(){
	InitWindow(300, 600, "Tetris clone");
	SetTargetFPS(60);

	grid = Grid();
	pieceX = (int)(grid.numCols / 2);
	pieceY = 0;

	GeneratePiece();

	while(WindowShouldClose() == false){
		if(++counter >= speed){
			counter = 0;
			MovePiece(DOWN);
		}
		// std::cout << "counter: " << counter << "\nspeed: " << speed << std::endl;

		BeginDrawing();
		ClearBackground(bgColor);
		grid.Draw();
		DrawFallingPiece();

		EndDrawing();
	
	}
	CloseWindow();
}
