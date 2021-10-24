#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>

// Game Board
#define GBOARD_WIDTH 10
#define GBOARD_HEIGHT 20

#define GBOARD_ORIGIN_X 4
#define GBOARD_ORIGIN_Y 2

int boardState[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];

#endif

/*
point prevPnt;

prevPnt = GetCurrentCursorPos();

for (i = 0; i < GBOARD_HEIGHT + 1; i++)
{
	for (j = 0; j < GBOARD_WIDTH + 2; j++)
	{
		SetCurrentCursorPos(70 + 2*j, 2 + i);
		printf("%d ", boardState[i][j]);
	}
	printf("\n");
}
SetCurrentCursorPos(prevPnt.x, prevPnt.y);
*/