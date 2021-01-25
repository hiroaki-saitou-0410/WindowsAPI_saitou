#ifndef DIRECT_INPUT_H_
#define DIRECT_INPUT_H_

#include <dinput.h>

enum InputState
{
	NoHold,		// �����Ă��Ȃ�
	Pushed,		// ������
	Held,		// �����Ă���
	Released,	// ������
};

bool InitInput();
void UpdateInput();
void ReleaseInput();

// �L�[��������Ă��邩�𒲂ׂ�֐�
bool IsKeyHeld(int key);
bool IsKeyPushed(int key);
bool IsKeyReleased(int key);

#endif
