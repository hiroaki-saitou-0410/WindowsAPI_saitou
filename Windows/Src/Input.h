#ifndef DIRECT_INPUT_H_
#define DIRECT_INPUT_H_

#include <dinput.h>

enum InputState
{
	NoHold,		// 押していない
	Pushed,		// 押した
	Held,		// 押している
	Released,	// 離した
};

bool InitInput();
void UpdateInput();
void ReleaseInput();

// キーが押されているかを調べる関数
bool IsKeyHeld(int key);
bool IsKeyPushed(int key);
bool IsKeyReleased(int key);

#endif
