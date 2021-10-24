#ifndef __KEY_CUR_CONTROL_H__
#define __KEY_CUR_CONTROL_H__

#include <conio.h>
#include <Windows.h>

#include "point.h"

void RemoveCursor(void);
point GetCurrentCursorPos(void);
void SetCurrentCursorPos(int x, int y);

void InitKeyDelayRate(int rate);

void DrawGameBoard(void);
void InitBoardState(void);

int FindBottomEnd(char block[][4]);
int FindRightEnd(char block[][4]);
int FindLeftEnd(char block[][4]);

int ProcessKeyInput(char blockInfo[][4], int keySensitive);
#endif