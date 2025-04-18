#include <raylib.h>
#include "grid.hpp"
#include <iostream>
#include <ctime>

#define RIGHT 201
#define LEFT 160
#define TOP 30
#define DOWN 203
#define REFRESH 0

#define SHAPE_I 0
#define SHAPE_L 1
#define SHAPE_L2 2
#define SHAPE_O 3
#define SHAPE_S 4
#define SHAPE_S2 5
#define SHAPE_T 6

int MovePiece(int direction);
bool DetectCollision(int direction);
void DrawFallingPiece();
void GeneratePiece();
void PlacePiece();
void ProcessInput();
void SnapDown();
void RotatePiece();

Color bgColor = {44, 44, 127, 255}; // dark blue

Grid grid;
int pieceX;
int pieceY;
int pieceMatrix[16];
	
int counter;
int fallingSpeed = 35;
int cycle = 0;

int inputRepeatLatency = 5;
int inputRepeatCounter = 0;

int pieceRotation = 0;
int shape = 0;

int shapeI[16] = {
	0,1,0,0,
	0,1,0,0,
	0,1,0,0,
	0,1,0,0
};

int shapeT[16] = {
	0,1,0,0,
	0,1,1,0,
	0,1,0,0,
	0,0,0,0
};

int shapeL[16] = {
	0,0,1,0,
	1,1,1,0,
	0,0,0,0,
	0,0,0,0
};

int shapeL2[16] = {
	1,0,0,0,
	1,1,1,0,
	0,0,0,0,
	0,0,0,0
};

int shapeS[16] = {
	0,1,1,0,
	1,1,0,0,
	0,0,0,0,
	0,0,0,0
};

int shapeS2[16] = {
	1,1,0,0,
	0,1,1,0,
	0,0,0,0,
	0,0,0,0
};

int shapeO[16] = {
	1,1,0,0,
	1,1,0,0,
	0,0,0,0,
	0,0,0,0
};

int MovePiece(int direction){
	switch(direction){
		case RIGHT:
			if(DetectCollision(RIGHT))
				return 0;
			pieceX++;
			break;
		case LEFT:
			if(DetectCollision(LEFT))
				return 0;
			pieceX--;
			break;
		case DOWN:
			if(DetectCollision(DOWN))
				return 0;
			pieceY++;
			break;
		case REFRESH:
			if(DetectCollision(REFRESH))
				return 0;
			break;
	}
	return 1;
}

void DrawFallingPiece(){
	for(int i = 0; i < 16; i++){
		if(pieceMatrix[i] == 0)
			continue;
		else{
			int x = i % 4;
			int y = (int)(i / 4);

			DrawRectangle((pieceX + x) * grid.cellSize + 1, (pieceY + y) * grid.cellSize +1, grid.cellSize -1, grid.cellSize -1, grid.colors[pieceMatrix[i]]);
		}
	}
}

bool DetectCollision(int direction){
	int x, y;
	int pX = pieceX;
	int pY = pieceY;

	switch(direction){
		case RIGHT:
			++pX;
			break;
		case LEFT:
			--pX;
			break;
		case DOWN:
			pY++;
			break;
		default:
			break;
	}

	// check left boundaries
	if(pX < 0){
		for(int i = (0 - pX) -1; i < 16; i += 4){
			if(pieceMatrix[i] != 0)
				return true;
		}
	}

	// check right boundaries
	if(pX > grid.numCols - 4){
		for(int i = 16 - (pX - (grid.numCols - 4)); i >= 0; i -= 4)
			if(pieceMatrix[i] != 0){
				return true;
			}
	}

	// check bottom
	for(int i = 0; i < 16; i++){
		x = i % 4;
		y = (int)(i / 4);

		if(grid.grid[pY + y][pX + x] != 0 && pieceMatrix[i] != 0 || ((pY + y) >= grid.numRows && pieceMatrix[i] != 0)){
			return true;
		}
	}

	// std::cout << "collision: " << collision << "\npyy: " << (pY + y) << std::endl;
	return false;
}

void GeneratePiece(){
	int color = (rand() % 6) + 1; //start at 1
	shape = rand() % 7;

	for(int i = 0; i < 16; i++){
		switch(shape){
			case 0:
				pieceMatrix[i] = shapeI[i] * color;
				break;
			case 1:
				pieceMatrix[i] = shapeL[i] * color;
				break;
			case 2:
				pieceMatrix[i] = shapeL2[i] * color;
				break;
			case 3:
				pieceMatrix[i] = shapeO[i] * color;
				break;
			case 4:
				pieceMatrix[i] = shapeS[i] * color;
				break;
			case 5:
				pieceMatrix[i] = shapeS2[i] * color;
				break;
			case 6:
				pieceMatrix[i] = shapeT[i] * color;
				break;
		}
	}
}


void PlacePiece(){
	int i, y;
	int pX = pieceX;
	int pY = pieceY;

	grid.Insert(pieceX, pieceY, pieceMatrix);

	for(i = 0; i < 16; i++)
		pieceMatrix[i] = 0;

	pieceX = (int)(grid.numCols / 2);
	pieceY = 0;

	GeneratePiece();
}

void ProcessInput(){
	int keycode = GetKeyPressed();
	if(keycode == 0)
		return;

	switch(keycode){
		case KEY_H:
			MovePiece(LEFT);
			break;
		case KEY_L:
			MovePiece(RIGHT);
			break;
		case KEY_J:
			SnapDown();
			break;
		case KEY_E:
			pieceRotation += 3;
			RotatePiece();
			break;
		case KEY_F:
			pieceRotation++;
			RotatePiece();
			break;
	}

	// inputRepeatCounter++;
	// if(inputRepeatCounter >= inputRepeatLatency){
	// 	switch(keycode){
	// 		case KEY_H:
	// 			MovePiece(LEFT);
	// 			break;
	// 		case KEY_L:
	// 			MovePiece(RIGHT);
	// 			break;
	// 		case KEY_J:
	// 			SnapDown();
	// 			break;
	// 	}	
	// 	inputRepeatCounter = 0;
	// }
}

void SnapDown(){
	while(!DetectCollision(DOWN)){
		pieceY++;
	}
	PlacePiece();
}

void RotatePiece(){
	int temp[16];
	for(int i = 0; i < 16; i++){
		temp[i] = pieceMatrix[i];
	}

	switch(shape){

	}
}

int main(){
	InitWindow(300, 600, "Tetris clone");
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
	SetTargetFPS(60);

	grid = Grid();
	pieceX = (int)(grid.numCols / 2);
	pieceY = 0;

	srand(time(0));
	
	GeneratePiece();

	while(WindowShouldClose() == false){
		if(++counter >= fallingSpeed){
			counter = 0;
			if(!MovePiece(DOWN)){
				if(pieceY <= 0)
					break;
				PlacePiece();
			}
		}
		// std::cout << "counter: " << counter << "\nspeed: " << speed << std::endl;

		ProcessInput();

		BeginDrawing();
		ClearBackground(bgColor);
		grid.Draw();
		DrawFallingPiece();
		// DrawRectangle(0, 0, 300, 600, RED);

		EndDrawing();
	
	}
	// std::cout << "last cycle: " << cycle << std::endl;
	CloseWindow();
}
