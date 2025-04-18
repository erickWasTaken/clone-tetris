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
int pieceMatrix[4][4];
	
int counter;
int fallingSpeed = 35;
int cycle = 0;

int inputRepeatLatency = 5;
int inputRepeatCounter = 0;

int pieceRotation = 0;
int shape = 0;

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
	for(int x = 0; x < 4; x++){
		for(int y = 0; y < 4; y++){
			// grid.grid[pieceY + j][pieceX + i] = pieceMatrix[i][j];
			if(pieceMatrix[x][y] == 0)
				continue;
			else{
				DrawRectangle((pieceX + x) * grid.cellSize + 1, (pieceY + y) * grid.cellSize +1, grid.cellSize -1, grid.cellSize -1, grid.colors[pieceMatrix[x][y]]);
			}
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
		for(x = 0; (x + pX) < 0; x++)
			for(int y = 0; y < 4; y++)
				if(pieceMatrix[x][y] != 0){
					return true;
				}
	}

	// check right boundaries
	if(pX > grid.numCols - 4){
		for(x = pX + 3; x >= grid.numCols; x--)
			for(y = 0; y < 4; y++)
				if(pieceMatrix[x - pX][y] != 0){
					return true;
				}
	}

	// check bottom
	for(x = 0; x < 4; x++)
		for(y = 0; y < 4; y++)
			if(grid.grid[pY + y][pX + x] != 0 && pieceMatrix[x][y] != 0 || ((pY + y) >= grid.numRows && pieceMatrix[x][y] != 0)){
				return true;
			}

	// std::cout << "collision: " << collision << "\npyy: " << (pY + y) << std::endl;
	return false;
}

void GeneratePiece(){
	int color = (rand() % 6) + 1; //start at 1
	shape = rand() % 7;

	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			switch(shape){
				case 0:
					pieceMatrix[j][i] = shapeI[i][j] * color;
					break;
				case 1:
					pieceMatrix[j][i] = shapeL[i][j] * color;
					break;
				case 2:
					pieceMatrix[j][i] = shapeL2[i][j] * color;
					break;
				case 3:
					pieceMatrix[j][i] = shapeO[i][j] * color;
					break;
				case 4:
					pieceMatrix[j][i] = shapeS[i][j] * color;
					break;
				case 5:
					pieceMatrix[j][i] = shapeS2[i][j] * color;
					break;
				case 6:
					pieceMatrix[j][i] = shapeT[i][j] * color;
					break;
			}
			// std::cout << pieceMatrix[j][i] * color << ", ";
		}
		// std::cout << std::endl;
	}

	// std::cout << "color value: " << color << std::endl;
}


void PlacePiece(){
	int x, y;
	int pX = pieceX;
	int pY = pieceY;

	grid.Insert(pieceX, pieceY, pieceMatrix);

	for(x = 0; x < 4; x++)
		for(y = 0; y < 4; y++)
			pieceMatrix[y][x] = 0;

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
	int temp[4][4];
	for(int x = 0; x < 4; x++){
		for(int y = 0; y < 4; y++){
			temp[x][y] = pieceMatrix[x][y];
		}
	}

	switch(shape){
		case SHAPE_O:
			return;
		case SHAPE_L:
		case SHAPE_L2:
		case SHAPE_S:
		case SHAPE_S2:
		case SHAPE_T:
			pieceMatrix[0][0] = temp[2][0];
			pieceMatrix[0][2] = temp[0][0];
			pieceMatrix[2][2] = temp[0][2];
			pieceMatrix[2][0] = temp[2][2];

			pieceMatrix[0][1] = temp[1][0];
			pieceMatrix[1][2] = temp[0][1];
			pieceMatrix[2][1] = temp[1][2];
			pieceMatrix[1][0] = temp[2][1];
			break;

		case SHAPE_I:
			pieceMatrix[0][1] = temp[1][0];
			
			pieceMatrix[1][0] = temp[0][1];
			pieceMatrix[1][2] = temp[2][1];
			pieceMatrix[2][1] = temp[1][2];
			pieceMatrix[1][3] = temp[3][1];
			pieceMatrix[3][1] = temp[1][3];
			break;
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
