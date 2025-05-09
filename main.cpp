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
void HorizontalSnap();
void PaintInactive();
bool IsRowEmpty(int col);
bool IsColumnEmpty(int row);
void RepositionShape();
void RemoveLine(int line);

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
int color;

int prevColor = 0;

int score = 0;

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
	// color = 1;
	do{
		color =  (rand() % 6) + 1;
	}while(color == prevColor);
	prevColor = color;

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
	// PaintInactive();
	RepositionShape();
}

void PaintInactive(){
	for(int i = 0; i < 16; i++)
		if(pieceMatrix[i] == 0)
			pieceMatrix[i] = 8;

}


void PlacePiece(){
	int i, y;
	int pX = pieceX;
	int pY = pieceY;

	// score += grid.Insert(pieceX, pieceY, pieceMatrix);
	for(i = 0; i < 16; i++){
		if(grid.grid[pieceY + (int)(i / 4)][pieceX + (i % 4)] != 0)
			continue;
		grid.grid[pieceY + (int)(i / 4)][pieceX + (i % 4)] = pieceMatrix[i];
	}

	bool remove;
	for(int y = 0; y < grid.numRows; y++){
		remove = true;
		for(int x = 0; x < grid.numCols; x++){
			if(grid.grid[y][x] == 0)
				remove = false;
		}

		if(remove){
			RemoveLine(y);
			score = i * grid.numCols;
			fallingSpeed --;
		}
	}

	for(i = 0; i < 16; i++)
		pieceMatrix[i] = 0;

	pieceX = (int)(grid.numCols / 2);
	pieceY = 0;
	pieceRotation = 0;

	// std::cout << "score: " << score << std::endl;
	GeneratePiece();
	// grid.Print();
}

void RemoveLine(int line){
	int temp[10];
	int i;
	for(i = 0; i < grid.numCols; i++){
		temp[i] = grid.grid[line][i];
	}

	for(i = line; i > 0; i--){
		for(int j = 0; j < grid.numCols; j++){
			grid.grid[i][j] = grid.grid[i -1][j];
		}
	}
}

void ProcessInput(){
	if(IsKeyDown(KEY_LEFT_SHIFT)){
		HorizontalSnap();
		return;
	}
	
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
			PlacePiece();
			break;
		case KEY_E:
			pieceRotation += 3;
			RotatePiece();
			break;
		case KEY_F:
			pieceRotation++;
			RotatePiece();
			break;
		// case KEY_LEFT_CONTROL:
		// 	GeneratePiece();
		// 	break;
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
}

void HorizontalSnap(){
	int snapDirection = 0;
	if(IsKeyPressed(KEY_H))
		snapDirection--;
	if(IsKeyPressed(KEY_L))
		snapDirection++;

	switch(snapDirection){
		case -1:
			while(!DetectCollision(LEFT)){
				pieceX--;
			}
			break;
		case 1:
			while(!DetectCollision(RIGHT)){
				pieceX++;
			}
			break;
		case 0:
			return;
	}
}

void RotatePiece(){
	int temp[16];
	int current[16];
	for(int i = 0; i < 16; i++){
		current[i] = pieceMatrix[i];

		switch(shape){
			case SHAPE_I:
				temp[i] = shapeI[i];
				break;
			case SHAPE_L:
				temp[i] = shapeL[i];
				break;
			case SHAPE_L2:
				temp[i] = shapeL2[i];
				break;
			case SHAPE_O:
				temp[i] = shapeO[i];
				break;
			case SHAPE_S:
				temp[i] = shapeS[i];
				break;
			case SHAPE_S2:
				temp[i] = shapeS2[i];
				break;
			case SHAPE_T:
				temp[i] = shapeT[i];
				break;
		}
	}

	for(int i = 0; i < 16; i++){
		int pi = 0;
		int x = i % 4;
		int y = (int)(i / 4);

		switch(pieceRotation % 4){
			case 0:
				pi = y * 4 + x;
				break;
			case 1:
				pi = 12 + y - (x * 4);
				break;
			case 2:
				pi = 15 - (y * 4) - x;
				break;
			case 3:
				pi = 3 - y + (x * 4);
				break;
		}

		pieceMatrix[i] = temp[pi] * color;
		// pieceMatrix[i] = temp[pi];
	}

	// for(int i = 0; i < 4; i++){
	// 	for(int j = 0; j < 4; j++){
	// 		std::cout << pieceMatrix[i * 4 + j] << ", ";
	// 	}
	// 	std::cout << std::endl;
	// }
	
	RepositionShape();

	if(DetectCollision(REFRESH)){
		for(int i = 0; i < 16; i++)
			pieceMatrix[i] = current[i];
		pieceRotation += 4;
	}
	// std::cout << "piece rotation: " << pieceRotation % 4 << std::endl;
	// PaintInactive();
}

bool IsRowEmpty(int row){
	bool empty = true;
	for(int i = 0; i < 4; i++){
		int j = row * 4 + i;
		if(pieceMatrix[j] != 0 && pieceMatrix[j] != 8){
			empty = false;
			break;
		}
	}

	return empty;
}

bool IsColumnEmpty(int col){
	bool empty = true;
	for(int i = 0; i < 4; i++){
		int k = i * 4 + col;
		if(pieceMatrix[k] != 0 && pieceMatrix[k] != 8){
			empty = false;
			break;
		}
	}

	return empty;
}

void RepositionShape(){
	while(IsRowEmpty(0)){
		for(int i = 0; i < 3; i++){
			// std::cout << i << std::endl;
			if(IsRowEmpty(i)){
				for(int j = 0; j < 4; j++){
					// int k = i * 4 + j;
					pieceMatrix[i * 4 + j] = pieceMatrix[(i + 1) * 4 + j];
					pieceMatrix[(i + 1) * 4 + j] = 0;
				}
				continue;
			}
			break;
		}
	}

	while(IsColumnEmpty(0)){
		for(int i = 0; i < 3; i++){
			// std::cout << i << std::endl;
			if(IsColumnEmpty(i)){
				for(int j = 0; j < 4; j++){
					// int k = j * 4 + i;
					pieceMatrix[j * 4 + i] = pieceMatrix[j * 4 + i + 1];
					pieceMatrix[j * 4 + i + 1] = 0;
				}
				continue;
			}
			break;
		}
	}
}

int main(){
	InitWindow(300, 600, "Tetris clone");
	// SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
	SetTargetFPS(60);

	Font font = LoadFontEx("x10y12pxDonguriDuel.ttf", 16, NULL, 0);

	grid = Grid();
	srand(time(0));
	pieceX = (int)(grid.numCols / 2);
	// pieceY = (int)(grid.numRows / 2);
	pieceY = 0;

	GeneratePiece();

	while(WindowShouldClose() == false){
		ProcessInput();

		if(++counter >= fallingSpeed){
			counter = 0;
			if(!MovePiece(DOWN)){
				if(pieceY <= 0)
					break;
				PlacePiece();
			}
		}
		// std::cout << "counter: " << counter << "\nspeed: " << speed << std::endl;

		BeginDrawing();
		ClearBackground(bgColor);
		grid.Draw();
		DrawFallingPiece();
		DrawTextEx(font, "Score: ", {180, 20}, 16, 2, WHITE);
		
		char scoreText[10];
		sprintf(scoreText, "%d", score);
		
		DrawTextEx(font, scoreText, {240, 20}, 16, 2, WHITE);
		// DrawRectangle(0, 0, 300, 600, RED);

		EndDrawing();
	
	}
	// std::cout << "last cycle: " << cycle << std::endl;
	CloseWindow();
}
