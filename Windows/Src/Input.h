#ifndef DIRECT_INPUT_H_
#define DIRECT_INPUT_H_

#include <dinput.h>

enum InputState
{
	NoHold,		// ‰Ÿ‚µ‚Ä‚¢‚È‚¢
	Pushed,		// ‰Ÿ‚µ‚½
	Held,		// ‰Ÿ‚µ‚Ä‚¢‚é
	Released,	// —£‚µ‚½
};

bool InitInput();
void UpdateInput();
void ReleaseInput();

// ƒL[‚ª‰Ÿ‚³‚ê‚Ä‚¢‚é‚©‚ğ’²‚×‚éŠÖ”
bool IsKeyHeld(int key);
bool IsKeyPushed(int key);
bool IsKeyReleased(int key);

#endif
