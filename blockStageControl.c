#include <time.h>

#include "common.h"
#include "point.h"
#include "blockInfo.h"
#include "keyCurControl.h"

static int currentBlockModel;
static int curPosX, curPosY;
static int rotateSte;

static int keySensitive = 10000;
static int gameScore = 0;
static int gameLevel = 1;

void InitNewBlockPos(int x, int y)
{
	if (x < 0 || y < 0)
		return;

	curPosX = x;
	curPosY = y;

	SetCurrentCursorPos(x, y);
}

void ChooseBlock(void)
{
	srand((unsigned int)time(NULL));
	currentBlockModel = (rand() % NUM_OF_BLOCK_MODEL) * 4;
}

int GetCurrentBlockIdx(void)
{
	return currentBlockModel + rotateSte;
}

void ShowBlock(char blockInfo[][4])
{
	int x, y;
	point curPos = GetCurrentCursorPos();

	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			SetCurrentCursorPos(curPos.x + (x * 2), curPos.y + y);

			if (blockInfo[y][x] == 1)
				printf("■");
		}
		SetCurrentCursorPos(curPos.x, curPos.y);
	}
}

void DeleteBlock(char blockInfo[][4])
{
	int x, y;
	point curPos = GetCurrentCursorPos();

	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			SetCurrentCursorPos(curPos.x + (x * 2), curPos.y + y);

			if (blockInfo[y][x] == 1)
				printf("  ");
		}
		SetCurrentCursorPos(curPos.x, curPos.y);
	}
}

void BlockDown(void)
{
	DeleteBlock(blockModel[GetCurrentBlockIdx()]);
	curPosY = curPosY + 1;
	SetCurrentCursorPos(curPosX, curPosY);
	ShowBlock(blockModel[GetCurrentBlockIdx()]);
}

void ShiftLeft(void)
{
	DeleteBlock(blockModel[GetCurrentBlockIdx()]);
	curPosX -= 2;
	SetCurrentCursorPos(curPosX, curPosY);
	ShowBlock(blockModel[GetCurrentBlockIdx()]);
}

void ShiftRight(void)
{
	DeleteBlock(blockModel[GetCurrentBlockIdx()]);
	curPosX += 2;
	SetCurrentCursorPos(curPosX, curPosY);
	ShowBlock(blockModel[GetCurrentBlockIdx()]);
}

void ShiftDown(void)
{
	DeleteBlock(blockModel[GetCurrentBlockIdx()]);
	curPosY += 1;
	SetCurrentCursorPos(curPosX, curPosY);
	ShowBlock(blockModel[GetCurrentBlockIdx()]);
}

int GetRightEnd(char block[][4])
{
	int y, x;
	point curPos = GetCurrentCursorPos();

	for (x = 0; x < 4; x++)
	{
		for (y = 0; y < 4; y++)
		{
			if (block[y][3 - x] == 1)
			{
				return (curPos.x + (3 - x) * 2);
			}
		}
	}
}

int GetLeftEnd(char block[][4])
{
	int y, x;
	point curPos = GetCurrentCursorPos();

	for (x = 0; x < 4; x++)
	{
		for (y = 0; y < 4; y++)
		{
			if (block[y][x] == 1)
			{
				return (curPos.x + (x) * 2);
			}
		}
	}
}

void CheckCursorPos(void)
{
	static point prntPnt = {1, 1};
	point prevPnt;

	prevPnt = GetCurrentCursorPos();
	prntPnt.x = GetRightEnd(blockModel[GetCurrentBlockIdx()]);
	SetCurrentCursorPos(70, prntPnt.y);
	printf("%d %d \n", prntPnt.x, (GBOARD_ORIGIN_X + GBOARD_WIDTH * 2));
	SetCurrentCursorPos(prevPnt.x, prevPnt.y);

	prntPnt.y += 1;
}

void RotateBlock(void)
{
	int nexRotSte;

	DeleteBlock(blockModel[GetCurrentBlockIdx()]);

	nexRotSte = rotateSte + 1;
	nexRotSte %= 4;
	rotateSte = nexRotSte;

	if (GetRightEnd(blockModel[GetCurrentBlockIdx()]) > (GBOARD_ORIGIN_X + GBOARD_WIDTH * 2))
	{
		curPosX = curPosX - (GetRightEnd(blockModel[GetCurrentBlockIdx()]) - (GBOARD_ORIGIN_X + GBOARD_WIDTH * 2));
		SetCurrentCursorPos(curPosX, curPosY);
	}
	else if (GetLeftEnd(blockModel[GetCurrentBlockIdx()]) < (GBOARD_ORIGIN_X + 2))
	{
		curPosX = curPosX + (GBOARD_ORIGIN_X + 2) - GetLeftEnd(blockModel[GetCurrentBlockIdx()]);
		SetCurrentCursorPos(curPosX, curPosY);
	}
	else
		;

	ShowBlock(blockModel[GetCurrentBlockIdx()]);
}

// 좌표 공간과 동일하게 수정해야 보드의 일부가 지워지는 문제 해결가능할 듯
void BoardState(void)
{
	int i, j;
	int n, m;

	int num;

	point tempPnt = GetCurrentCursorPos();
	char (*tempBlock)[4] = blockModel[GetCurrentBlockIdx()];
	
	for (i = 1; i < GBOARD_HEIGHT + 1; i++)
	{
		for (j = 1; j < GBOARD_WIDTH + 2; j++)
		{
			if ((i == (tempPnt.y - 1)) && (j == (tempPnt.x / 2) - 1))
			{
				for (n = 0; n < 4; n++)
				{
					for (m = 0; m < 4; m++)
					{
						if (tempBlock[n][m] == 1)
						{
							boardState[i + n - 1][j + m - 1] = 1;
						}
					}
				}
			}
		}
	}
}

int DestroyBlock(void)
{
	int i, j, k, l, m;
	int n = 0;
	int sum;
	int destroyCheck = 0;

	for (i = GBOARD_HEIGHT - 1; i > 0; i--)
	{
		sum = 0;
		for (j = 1; j < GBOARD_WIDTH + 1; j++)
			sum = sum + boardState[i][j];

		if (sum == 10)
		{
			destroyCheck += 1;
			for (k = i; k > 0; k--)
			{
				for (l = 1; l < GBOARD_WIDTH + 1; l++)
					boardState[k][l] = boardState[k - 1][l];
			}
			i += 1;
		}
	}

	if (destroyCheck)
	{
		k = 0;
		for (i = 5; i < GBOARD_HEIGHT + 1; i++)
		{
			l = 0;
			for (j = 1; j < GBOARD_WIDTH + 1; j++)
			{
				l += 1;

				SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 + 2 * (j - 1), GBOARD_ORIGIN_Y + (i - 1));
				if (boardState[k + 4][l] == 0)
					printf("  ");

				else if (boardState[k + 4][l] == 1)
					printf("■");

//				Sleep(50);
			}
			k += 1;
		}
	}

	return destroyCheck;
}

void GameScore(void)
{
	point prevPnt;

	gameScore += 100;

	prevPnt = GetCurrentCursorPos();
	SetCurrentCursorPos(45, 7);
	printf("%5d\n", gameScore);
	SetCurrentCursorPos(prevPnt.x, prevPnt.y);
}

void GameLevel(void)
{
	point prevPnt;

	keySensitive -= 2000;
	gameLevel += 1;

	prevPnt = GetCurrentCursorPos();
	SetCurrentCursorPos(49, 5);
	printf("%d\n", gameLevel);
	SetCurrentCursorPos(prevPnt.x, prevPnt.y);
}

// BoardState에 대해서 동일한 이름 확인
void GameOver(void)
{
	int i;

	for (i = 1; i < (GBOARD_WIDTH + 1); i++)
	{
		if (boardState[2][i] == 1)
		{
			SetCurrentCursorPos(GBOARD_WIDTH+1, GBOARD_HEIGHT/2);
			printf("GAME OVER!!\n");
			SetCurrentCursorPos(0, 25);
			exit(0);
		}
	}
}

void ControlBlock(void)
{
	int destroyCheck;
	int i;

	do
	{
		destroyCheck = DestroyBlock();
		for (i = 0; i < destroyCheck; i++)
		{
			GameScore();

			if (gameScore % 1000 == 0)
				GameLevel();
		}
		BlockDown();
		ProcessKeyInput(blockModel[GetCurrentBlockIdx()], keySensitive);
	} while (FindBottomEnd(blockModel[GetCurrentBlockIdx()]));
}
/* end of file */