#include <Windows.h>
#include <conio.h>

#include "common.h"
#include "keyCurControl.h"
#include "blockStageControl.h"

#define START_CURPOS_X		12
#define START_CURPOS_Y		0

int main(void)
{
	int i, j;

	puts("ver. 2019");
	DrawGameBoard();

	InitKeyDelayRate(100);
	RemoveCursor();
	InitBoardState();
	
	while(1)
	{
		InitNewBlockPos(START_CURPOS_X, START_CURPOS_Y);
		ChooseBlock();
		ControlBlock();

		BoardState();
		
		GameOver();
	}

	return 0;
}
/* end of file */