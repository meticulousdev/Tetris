#ifndef __BLOCK_STAGE_CONTROL_H__
#define __BLOCK_STAGE_CONTROL_H__

void InitNewBlockPos(int x, int y);
void ChooseBlock(void);
void ShowBlock(char blockInfo[][4]);
void DeleteBlock(char blockInfo[][4]);
void BlockDown(void);

void ShiftLeft(void);
void ShiftRight(void);
void ShiftDown(void);

int GetRightEnd(char block[][4]);
int GetLeftEnd(char block[][4]);
void CheckCursorPos(void);
void RotateBlock(void);

void BoardState(void);
int DestroyBlock(void);

void GameScore(void);
void GameLevel(void);
void GameOver(void);

void ControlBlock(void);
#endif