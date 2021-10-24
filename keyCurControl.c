#include <conio.h>
#include <Windows.h>

#include "common.h"
#include "point.h"
#include "blockStageControl.h"

// KEY_SENSITIVE ***
#define SYS_DELAY		20

#define LEFT			75
#define RIGHT			77
#define UP				72
#define DOWN			80

#define	ESC				27
#define SPACEBAR		32

static int keyDelayRate;

void RemoveCursor(void)
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

point GetCurrentCursorPos(void)
{
	point curPoint;
	CONSOLE_SCREEN_BUFFER_INFO curInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curPoint.x = curInfo.dwCursorPosition.X;
	curPoint.y = curInfo.dwCursorPosition.Y;

	return curPoint;
}

void SetCurrentCursorPos(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void InitKeyDelayRate(int rate)
{
	if (rate < 1)
		return;
	keyDelayRate = rate;
}

void DrawGameBoard(void)
{
	int x, y;

	for (y = 0; y <= GBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);

		if (y == GBOARD_HEIGHT)
			printf("¦±");
		else
			printf("¦­");
	}

	for (y = 0; y <= GBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y);

		if (y == GBOARD_HEIGHT)
			printf("¦°");
		else
			printf("¦­");
	}

	for (x = 1; x < GBOARD_WIDTH + 1; x++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
		printf("¦¬");
	}

	SetCurrentCursorPos(35, 5);
	puts("Level : ");
	SetCurrentCursorPos(49, 5);
	printf("%d\n", 1);

	SetCurrentCursorPos(35, 7);
	puts("Score : ");
	SetCurrentCursorPos(45, 7);
	printf("%5d\n", 0);

	SetCurrentCursorPos(35, 13);
	puts("How to play tetris");

	SetCurrentCursorPos(37, 15);
	puts("¡è   : Rotate");
	SetCurrentCursorPos(35, 16);
	puts("¡ç");
	SetCurrentCursorPos(37, 16);
	puts("¡é");
	SetCurrentCursorPos(39, 16);
	puts("¡æ");

	SetCurrentCursorPos(42, 16);
	puts(": Move Left, Down, Right");

	SetCurrentCursorPos(35, 18);
	puts("SPACE BAR  : SPEED LANDING");

	SetCurrentCursorPos(35, 20);
	puts("ESC        : EXIT");

	SetCurrentCursorPos(0, 0);
}

void InitBoardState(void)
{
	int i, j;

	for (i = 0; i < GBOARD_HEIGHT + 1; i++)
	{
		for (j = 0; j < GBOARD_WIDTH + 2; j++)
		{
			if (j == 0)
				boardState[i][j] = 1;
			else if (j == GBOARD_WIDTH + 1)
				boardState[i][j] = 1;
			else if (i == GBOARD_HEIGHT)
				boardState[i][j] = 1;
			else
				boardState[i][j] = 0;
		}
	}
}

int FindRightEnd(char block[][4])
{
	int y, x;
	int curState;
	point curPos = GetCurrentCursorPos();
	point nextPos;

	for (x = 0; x < 4; x++)
	{
		for (y = 0; y < 4; y++)
		{
			if (block[y][x] == 1)
			{
				nextPos.x = (x + (curPos.x - 2) / 2 - 1) + 1;
				nextPos.y = (y - 2) + curPos.y;

				curState = boardState[nextPos.y][nextPos.x];

				if (curState == 1)
					return 0;
			}
		}
	}

	return 1;
}

int FindLeftEnd(char block[][4])
{
	int y, x;
	int curState;
	point curPos = GetCurrentCursorPos();
	point nextPos;

	for (x = 0; x < 4; x++)
	{
		for (y = 0; y < 4; y++)
		{
			if (block[y][x] == 1)
			{
				nextPos.x = (x + (curPos.x - 2) / 2 - 1) - 1;
				nextPos.y = (y - 2) + curPos.y;

				curState = boardState[nextPos.y][nextPos.x];

				if (curState == 1)
					return 0;
			}
		}
	}

	return 1;
}

int FindBottomEnd(char block[][4])
{
	int y, x;
	int curState;
	point curPos = GetCurrentCursorPos();
	point nextPos;

	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			if (block[y][x] == 1)
			{
				nextPos.x = x + (curPos.x - 2) / 2 - 1;
				nextPos.y = (y - 1) + curPos.y;

				curState = boardState[nextPos.y][nextPos.x];

				if (curState == 1)
					return 0;
			}
		}
	}

	return 1;
}

int ProcessKeyInput(char blockInfo[][4], int keySensitive)
{
	int i;
	int	key;

	int boolLeft;
	int boolRight;
	int boolBottom;

	for (i = 0; i < keySensitive; i++)
	{
		if (_kbhit() != 0)
		{
			key = _getch();

			boolLeft = FindLeftEnd(blockInfo);
			boolRight = FindRightEnd(blockInfo);
			boolBottom = FindBottomEnd(blockInfo);
			
			switch (key)
			{
			case LEFT:
				if (boolLeft)
					ShiftLeft();

				break;
			case RIGHT:
				if (boolRight)
					ShiftRight();

				break;
			case DOWN:
				if (boolBottom)
					ShiftDown();

				break;
			case SPACEBAR:
				if (boolBottom)
				{
					do
					{
						BlockDown();
					} while (FindBottomEnd(blockInfo));
				}
				break;
			case UP:
				if (boolBottom)
					RotateBlock();
								
				break;
			case ESC:
				SetCurrentCursorPos(0, 25);
				exit(0);
			}

			if (i % keyDelayRate == 0)
				Sleep(SYS_DELAY);
		}
	}
}
/* end of file */