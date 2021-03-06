#include "stdafx.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 19
#define NONE 0
#define WHITE 1
#define BLACK 2
#define MAX_DEPTH 2

typedef struct treeNode {
	int board[BOARD_SIZE][BOARD_SIZE];
	struct treeNode* child[8];
}treeNode;

int x, y;
int board[BOARD_SIZE][BOARD_SIZE] = { { 0 } };
int testBoard1[BOARD_SIZE][BOARD_SIZE];

void boardInit(int board[BOARD_SIZE][BOARD_SIZE]);
void boardPrint(int board[BOARD_SIZE][BOARD_SIZE]);
int checkStone(int x, int y);
int checkWin(int board[BOARD_SIZE][BOARD_SIZE], int x, int y);
int isFinish(int board[BOARD_SIZE][BOARD_SIZE]);

void putStonePlayer(int board[BOARD_SIZE][BOARD_SIZE], int stone_color);

void insertTree(treeNode** root, int board[BOARD_SIZE][BOARD_SIZE]);
void makeChild(treeNode** root, int stone_color, int depth);
int evalBoard(int board[BOARD_SIZE][BOARD_SIZE], int stone_color);
int alphaBetaSearch(int board[BOARD_SIZE][BOARD_SIZE], int stone_color);
int maxValue(treeNode** root, int stone_color);
int minValue(treeNode** root, int stone_color);

void putStoneCPU(int board[BOARD_SIZE][BOARD_SIZE], int stone_color);

void game(int board[BOARD_SIZE][BOARD_SIZE], int player_stone_color);

int main(){
	
	
	game(board, BLACK);
	
	system("pause");

	return 0;
}
void boardInit(int board[BOARD_SIZE][BOARD_SIZE]) {

	int i, j;
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			board[i][j] = NONE;
		}
	}
}

void boardPrint(int board[BOARD_SIZE][BOARD_SIZE]) {

	int i, j;
	 
	printf("   ");

	for (i = 0; i < BOARD_SIZE; i++) {
		if(i < 10)
		printf(" %d ", i);

		else
			printf("%d ", i);
	}
		
	
	printf("\n");

	for (i = 0; i < BOARD_SIZE; i++) {
		printf("%2d", i);
		for (j = 0; j < BOARD_SIZE; j++) {
			switch (board[i][j]) {
			case NONE: printf(" ·"); break;
			case BLACK: printf(" ○"); break;
			case WHITE: printf(" ●"); break;
			}
		}
		printf("\n");
	}
	printf("\n");

}

int checkStone(int x, int y)
{
	return (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE);
}

void putStonePlayer(int board[BOARD_SIZE][BOARD_SIZE], int stone_color) {

	int x, y;

	printf("Player:%s", (stone_color == BLACK) ? "○" : "●");
	printf("'s turn!  ex) (x y)\n> ");

	while (1) {
		scanf_s("%d %d", &x, &y);
		if (checkStone(x, y) && board[y][x] == NONE)
			break;

		printf("illegal input\n> ");
	}

	board[y][x] = stone_color;
}

int checkWin(int board[BOARD_SIZE][BOARD_SIZE], int x, int y)
{
	int i, j, win_flag;
	int dx[] = { 0, 1, 1, -1};
	int dy[] = { 1, 0, 1, 1};

	for (i = 0; i < 4; i++) {
		for (j = 1, win_flag = 1; j <= 4; j++) {
			if (board[y][x] != board[y + j * dy[i]][x + j * dx[i]]) {
				win_flag = 0;
				break;
			}
		}
		if (win_flag == 1) { return 1; }
	}
	return 0;
}

int isFinish(int board[BOARD_SIZE][BOARD_SIZE]) {
	
	int i, j, win_flag;

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == NONE) { continue; }
			if (checkWin(board, j, i)) {
				printf("win!!\n");
				return 1;
			}
		}
	}
	return 0;
}

void insertTree(treeNode** root, int board[BOARD_SIZE][BOARD_SIZE]){

	if ((*root) == NULL) {
		*root = (treeNode*)malloc(sizeof(treeNode));
		
		for(int i = 0; i < BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++) 
				(*root)->board[i][j] = board[i][j];

		for (int i = 0; i < 8; i++)
			(*root)->child[i] = NULL;
	
	}
}

void makeChild(treeNode** root, int stone_color, int depth) {
	if (depth == MAX_DEPTH)
		return;
	
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			testBoard1[i][j] = (*root)->board[i][j];

	//기준점을 정하다
	for (int i = 0; i < BOARD_SIZE / 2; i++) {
		if ((*root)->board[BOARD_SIZE / 2][BOARD_SIZE / 2 + i] == stone_color) {
			x = BOARD_SIZE / 2;
		    y = BOARD_SIZE / 2 + i;
			break;
		}

		else if ((*root)->board[BOARD_SIZE / 2 + i][BOARD_SIZE / 2] == stone_color) {
			x = BOARD_SIZE / 2 + i;
			y = BOARD_SIZE / 2;
			break;
		}

		else if ((*root)->board[BOARD_SIZE / 2][BOARD_SIZE / 2 - i] == stone_color) {
			x = BOARD_SIZE / 2;
			y = BOARD_SIZE / 2 - i;
			break;
		}

		else if ((*root)->board[BOARD_SIZE / 2 - i][BOARD_SIZE / 2] == stone_color) {
			x = BOARD_SIZE / 2 - i;
			y = BOARD_SIZE / 2;
			break;
		}

		else if ((*root)->board[BOARD_SIZE / 2 + i][BOARD_SIZE / 2 + i] == stone_color) {
			x = BOARD_SIZE / 2 + i;
			y = BOARD_SIZE / 2 + i;
			break;
		}

		else if ((*root)->board[BOARD_SIZE / 2 - i][BOARD_SIZE / 2 - i] == stone_color) {
			x = BOARD_SIZE / 2 - i;
			y = BOARD_SIZE / 2 - i;
			break;
		}

		else if ((*root)->board[BOARD_SIZE / 2 - i][BOARD_SIZE / 2 + i] == stone_color) {
			x = BOARD_SIZE / 2 - i;
			y = BOARD_SIZE / 2 + i;
			break;
		}

		else{
			x = BOARD_SIZE / 2 + i;
			y = BOARD_SIZE / 2 - i;
			break;
		}
	}

	//기준점에서 부터 8방향에 돌을 두고 각각 child에 복사한다, 만약 돌이 있으면 그 옆에 둔다
	for (int i = 0; i < BOARD_SIZE; i++) {
		if ((y + i) < BOARD_SIZE && (*root)->board[x][y + i] == NONE) {
			testBoard1[x][y + i] = stone_color;
			insertTree(&((*root)->child[0]), testBoard1);

			for (int i = 0; i < BOARD_SIZE; i++)
				for (int j = 0; j < BOARD_SIZE; j++)
					testBoard1[i][j] = (*root)->board[i][j];
			break;
		}		
	}

	for (int i = 0; i < BOARD_SIZE; i++) {
		if ((x + i) < BOARD_SIZE && (*root)->board[x + i][y] == NONE) {
			testBoard1[x + i][y] = stone_color;
			insertTree(&((*root)->child[1]), testBoard1);

			for (int i = 0; i < BOARD_SIZE; i++)
				for (int j = 0; j < BOARD_SIZE; j++)
					testBoard1[i][j] = (*root)->board[i][j];
			break;
		}
	}

	for (int i = 0; i < BOARD_SIZE; i++) {
		if ((y - i) < BOARD_SIZE && (*root)->board[x][y - i] == NONE) {
			testBoard1[x][y - i] = stone_color;
			insertTree(&((*root)->child[2]), testBoard1);

			for (int i = 0; i < BOARD_SIZE; i++)
				for (int j = 0; j < BOARD_SIZE; j++)
					testBoard1[i][j] = (*root)->board[i][j];
			break;
		}
	}

	for (int i = 0; i < BOARD_SIZE; i++) {
		if ((x - i) < BOARD_SIZE && (*root)->board[x - i][y] == NONE) {
			testBoard1[x - i][y] = stone_color;
			insertTree(&((*root)->child[3]), testBoard1);

			for (int i = 0; i < BOARD_SIZE; i++)
				for (int j = 0; j < BOARD_SIZE; j++)
					testBoard1[i][j] = (*root)->board[i][j];
			break;
		}
	}

	for (int i = 0; i < BOARD_SIZE; i++) {
		if ((y + i) < BOARD_SIZE && (x + i) < BOARD_SIZE && (*root)->board[x + i][y + i] == NONE) {
			testBoard1[x + i][y + i] = stone_color;
			insertTree(&((*root)->child[4]), testBoard1);

			for (int i = 0; i < BOARD_SIZE; i++)
				for (int j = 0; j < BOARD_SIZE; j++)
					testBoard1[i][j] = (*root)->board[i][j];
			break;
		}
	}

	for (int i = 0; i < BOARD_SIZE; i++) {
		if ((y - i) < BOARD_SIZE && (x - i) < BOARD_SIZE && (*root)->board[x - i][y - i] == NONE) {
			testBoard1[x - i][y - i] = stone_color;
			insertTree(&((*root)->child[5]), testBoard1);

			for (int i = 0; i < BOARD_SIZE; i++)
				for (int j = 0; j < BOARD_SIZE; j++)
					testBoard1[i][j] = (*root)->board[i][j];;
			break;
		}
	}

	for (int i = 0; i < BOARD_SIZE; i++) {
		if ((y + i) < BOARD_SIZE && (x - i) < BOARD_SIZE && (*root)->board[x - i][y + i] == NONE) {
			testBoard1[x - i][y + i] = stone_color;
			insertTree(&((*root)->child[6]), testBoard1);

			for (int i = 0; i < BOARD_SIZE; i++)
				for (int j = 0; j < BOARD_SIZE; j++)
					testBoard1[i][j] = (*root)->board[i][j];
			break;
		}
	}

	for (int i = 0; i < BOARD_SIZE; i++) {
		if ((y - i) < BOARD_SIZE && (x + i) < BOARD_SIZE && (*root)->board[x + i][y - i] == NONE) {
			testBoard1[x + i][y - i] = stone_color;
			insertTree(&((*root)->child[7]), testBoard1);

			for (int i = 0; i < BOARD_SIZE; i++)
				for (int j = 0; j < BOARD_SIZE; j++)
					testBoard1[i][j] = (*root)->board[i][j];
			break;
		}
	}

	if (stone_color == BLACK)
		stone_color = WHITE;

	else
		stone_color = BLACK;

	//각 child들의 child를 생성한다
	for (int i = 0; i < 8; i++)
		makeChild(&((*root)->child[i]), stone_color, depth + 1);
}


int evalBoard(int board[BOARD_SIZE][BOARD_SIZE], int stone_color) {

	int count1 = 0;
	int count2 = 0;
	int count3 = 0;
	int count4 = 0;
	int count5 = 0;
	int count6 = 0;
	int count7 = 0;
	int count8 = 0;

	int utility = 0;

	int other_stone_color;

	if (stone_color == BLACK)
		other_stone_color = WHITE;

	else
		other_stone_color = BLACK;
	
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++) {

			if (board[j][i] == stone_color) {

				for (int k = 1; k <= 4; k++) {
					if (board[j][i + k] == stone_color)
						count1++;

					if (board[j + k][i] == stone_color)
						count2++;

					if (board[j + k][i + k] == stone_color)
						count3++;
						
					if (count1 == 4 || count2 == 4 || count3 == 4)
						utility += 10000;	
				}

				count1 = 0;
				count2 = 0;
				count3 = 0;

				for (int k = 1; k <= 4; k++) {
					if (board[j][i + k] == other_stone_color)
						count1++;

					if (board[j + k][i] == other_stone_color)
						count2++;

					if (board[j + k][i + k] == other_stone_color)
						count3++;

					if (i >= 5 && board[j][i - k] == other_stone_color)
						count4++;

					if (j >= 5 && board[j - k][i] == other_stone_color)
						count5++;

					if (i >= 5 && j >= 5 && board[j - k][i - k] == other_stone_color)
						count6++;

					if (i >= 5 && board[j + k][i - k] == other_stone_color)
						count7++;

					if (j >= 5 && board[j - k][i + k] == other_stone_color)
						count8++;
				}
					if (board[j][i + 5] == stone_color)
						count1++;

					if (board[j + 5][i] == stone_color)
						count2++;

					if (board[j + 5][i + 5] == stone_color)
						count3++;

					if (i >= 5 && board[j][i - 5] == stone_color)
						count4++;

					if (j >= 5 && board[j - 5][i] == stone_color)
						count5++;

					if (i >= 5 && j >= 5 && board[j - 5][i - 5] == stone_color)
						count6++;

					if (i >= 5 && board[j + 5][i - 5] == stone_color)
						count7++;

					if (j >= 5 && board[j - 5][i + 5] == stone_color)
						count8++;

					if(count1 == 5 || count2 == 5 || count3 == 5 || count4 == 5 || count5 == 5 || count6 == 5 || count7 == 5 || count8 == 5)
						utility += 5000;

				count1 = 0;
				count2 = 0;
				count3 = 0;
				count4 = 0;
				count5 = 0;
				count6 = 0;
				count7 = 0;
				count8 = 0;
					

				for (int k = 1; k <= 3; k++) {
					if (board[j][i + k] == stone_color)
							count1++;

					if (board[j + k][i] == stone_color)
							count2++;

					if (board[j + k][i + k] == stone_color)
							count3++;

					if (count1 == 3 || count2 == 3 || count3 == 3) 
						utility += 3000;
					
				}

				count1 = 0;
				count2 = 0;
				count3 = 0;

				for (int k = 1; k <= 3; k++) {
					if (board[j][i + k] == other_stone_color)
						count1++;

					if (board[j + k][i] == other_stone_color)
						count2++;

					if (board[j + k][i + k] == other_stone_color)
						count3++;

					if (i >= 3 && board[j][i - k] == other_stone_color)
						count4++;

					if (j >= 3 && board[j - k][i] == other_stone_color)
						count5++;

					if (i >= 3 && j >= 3 && board[j - k][i - k] == other_stone_color)
						count6++;

					if (i >= 3 && board[j + k][i - k] == other_stone_color)
						count7++;

					if (j >= 3 && board[j - k][i + k] == other_stone_color)
						count8++;

					if (count1 == 3 || count2 == 3 || count3 == 3 || count4 == 3 || count5 == 3 || count6 == 3 || count7 == 3 || count8 == 3) 
						utility += 1000;
					
				}

				count1 = 0;
				count2 = 0;
				count3 = 0;
				count4 = 0;
				count5 = 0;
				count6 = 0;
				count7 = 0;
				count8 = 0;

				for (int k = 1; k <= 2; k++) {
					if (board[j][i + k] == stone_color)
						count1++;

					if (board[j + k][i] == stone_color)
						count2++;

					if (board[j + k][i + k] == stone_color)
						count3++;

					if (count1 == 2 || count2 == 2 || count3 == 2)
						utility += 100;
				}

				count1 = 0;
				count2 = 0;
				count3 = 0;

				for (int k = 1; k <= 2; k++) {
					if (board[j][i + k] == other_stone_color)
						count1++;

					if (board[j + k][i] == other_stone_color)
						count2++;

					if (board[j + k][i + k] == other_stone_color)
						count3++;

					if (i >= 2 && board[j][i - k] == other_stone_color)
						count4++;

					if (j >= 2 && board[j - k][i] == other_stone_color)
						count5++;

					if (i >= 2 && j >= 2 && board[j - k][i - k] == other_stone_color)
						count6++;

					if (i >= 2 && board[j + k][i - k] == other_stone_color)
						count7++;

					if (j >= 2 && board[j - k][i + k] == other_stone_color)
						count8++;

					if (count1 == 2 || count2 == 2 || count3 == 2 || count4 == 2 || count5 == 2 || count6 == 2 || count7 == 2 || count8 == 2)
						utility += 50;

				}

				count1 = 0;
				count2 = 0;
				count3 = 0;
				count4 = 0;
				count5 = 0;
				count6 = 0;
				count7 = 0;
				count8 = 0;

				if (board[j][i + 1] == stone_color)
					count1++;

				if (board[j + 1][i] == stone_color)
					count2++;

				if (board[j + 1][i + 1] == stone_color)
					count3++;

				if (i >= 1 && board[j][i - 1] == stone_color)
					count4++;

				if (j >= 1 && board[j - 1][i] == stone_color)
					count5++;

				if (i >= 1 && j >= 1 && board[j - 1][i - 1] == stone_color)
					count6++;

				if (i >= 1 && board[j + 1][i - 1] == stone_color)
					count7++;

				if (j >= 1 && board[j - 1][i + 1] == stone_color)
					count8++;

				if (count1 == 1 || count2 == 1 || count3 == 1 || count4 == 1 || count5 == 1 || count6 == 1 || count7 == 1 || count8 == 1)
						utility += 1;
				

				count1 = 0;
				count2 = 0;
				count3 = 0;
				count4 = 0;
				count5 = 0;
				count6 = 0;
				count7 = 0;
				count8 = 0;

				if (board[j][i + 1] == other_stone_color)
					count1++;

				if (board[j + 1][i] == other_stone_color)
					count2++;

				if (board[j + 1][i + 1] == other_stone_color)
					count3++;

				if (i >= 1 && board[j][i - 1] == other_stone_color)
					count4++;

				if (j >= 1 && board[j - 1][i] == other_stone_color)
					count5++;

				if (i >= 1 && j >= 1 && board[j - 1][i - 1] == other_stone_color)
					count6++;

				if (i >= 1 && board[j + 1][i - 1] == other_stone_color)
					count7++;

				if (j >= 1 && board[j - 1][i + 1] == other_stone_color)
					count8++;

				if (count1 == 1 || count2 == 1 || count3 == 1 || count4 == 1 || count5 == 1 || count6 == 1 || count7 == 1 || count8 == 1)
						utility += 1;

			}

		}
	
		return utility;
}

int alphaBetaSearch(int board[BOARD_SIZE][BOARD_SIZE], int stone_color) {
	
	int other_stone_color;

	if (stone_color == BLACK)
		other_stone_color = WHITE;
	else
		other_stone_color = BLACK;


	treeNode* root = NULL;
	insertTree(&(root), board);
	makeChild(&(root), other_stone_color, 0);

	return maxValue(&(root), stone_color);

}

int maxValue(treeNode** root, int stone_color) {
	if ((*root)->child[0] == NULL)
		return evalBoard((*root)->board, stone_color);

	int alpha = -1; // 가장 작은 값

	for (int i = 0; i < 8; i++)
		if (alpha < minValue(&((*root)->child[i]), stone_color))
				alpha = minValue(&((*root)->child[i]), stone_color);
	
	return alpha;
}

int minValue(treeNode** root, int stone_color) {
	if ((*root)->child[0] == NULL)
		return evalBoard((*root)->board, stone_color);

	int beta = 1000000; // 가장 큰 값

	for (int i = 0; i < 8; i++)
		if (beta > minValue(&((*root)->child[i]), stone_color))
			beta = minValue(&((*root)->child[i]), stone_color);

	return beta;
}

void putStoneCPU(int board[BOARD_SIZE][BOARD_SIZE], int stone_color) {
	
	int count = 0;
	int utility = 0;
	int max = -1;
	int x, y;

	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			if (board[i][j] == BLACK || board[i][j] == WHITE)
				count++;

	if (count == 0){
		board[BOARD_SIZE / 2][BOARD_SIZE / 2] = stone_color;
		printf("CPU:%s", (stone_color == BLACK) ? "○" : "●");
		printf(" x = %d, y = %d \n", BOARD_SIZE / 2, BOARD_SIZE / 2);
	}

	else {
		for (int i = 0; i < BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++){
				if (board[j][i] == NONE) {
					board[j][i] = stone_color;
					utility = alphaBetaSearch(board, stone_color);

					if (utility > max) {
						x = j;
						y = i;
						max = utility;
					}

					board[j][i] = NONE;
				}
			}

		board[x][y] = stone_color;
		boardPrint(board);
		printf("CPU:%s", (stone_color == BLACK) ? "○" : "●");
		printf(" x = %d, y = %d \n", y, x);

	}

}

void game(int board[BOARD_SIZE][BOARD_SIZE], int player_stone_color) {
	
	boardInit(board);

	int finish = 0;
	int CPU_stone_color = 0;

	if (player_stone_color == BLACK)
		CPU_stone_color = WHITE;

	else
		CPU_stone_color = BLACK;
	
	if(player_stone_color == BLACK){
		boardPrint(board);
		for (int i = 0; i < BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++) {

				putStonePlayer(board, player_stone_color);
				boardPrint(board);

				finish = isFinish(board);
				if (finish == 1) {
					
					system("pause");
					exit(1);
				}

				putStoneCPU(board, CPU_stone_color);
				boardPrint(board);

				finish = isFinish(board);
				if (finish == 1) {

					system("pause");
					exit(1);
				}
			}
	}

	else
		for (int i = 0; i < BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++) {

				putStoneCPU(board, CPU_stone_color);
				boardPrint(board);

				finish = isFinish(board);
				if (finish == 1) {

					system("pause");
					exit(1);
				}

				putStonePlayer(board, player_stone_color);
				boardPrint(board);

				finish = isFinish(board);
				if (finish == 1) {

					system("pause");
					exit(1);
				}		
			}
	printf("draw!\n");
}