#include "Input.h"

LPDIRECTINPUT8 g_InputInterface;			// DIRECTINPUT8�̃|�C���^
LPDIRECTINPUTDEVICE8 g_InputKeyboardDevice;	// Keyboard�pDevice�̃|�C���^

InputState g_KeyStates[256];	// �L�[���̕ۑ��p

InputState UpdateInputState(bool is_push, InputState state);

InputState UpdateInputState(bool is_push, InputState state)
{
	if (is_push == true)
	{
		// �O�񉟂���Ă��Ȃ�����
		if (state == InputState::NoHold)
		{
			return InputState::Pushed;
		}
		// �O�񉟂��ꂽ or �O�񉟂���Ă���
		else if (state == InputState::Pushed ||
			state == InputState::Held)
		{
			return InputState::Held;
		}
	}
	else
	{
		// �O�񉟂���Ă��� or �O�񉟂��ꂽ
		if (state == InputState::Pushed ||
			state == InputState::Held)
		{
			return InputState::Released;
		}
	}

	return InputState::NoHold;
}

bool InitInput()
{
	HRESULT hr = DirectInput8Create(
		GetModuleHandle(nullptr),	// �C���X�^���X�n���h��
		DIRECTINPUT_VERSION,		// DirectInput�̃o�[�W����(�Œ�l)
		IID_IDirectInput8,			// �g�p����@�\(�Œ�l)
		(void**)&g_InputInterface,	// �쐬���ꂽIDirectInput8�ۑ��p�ϐ��w��
		nullptr						// nullptr�Œ�
	);

	if (FAILED(hr))
	{
		return false;
	}

	hr = g_InputInterface->CreateDevice(
		GUID_SysKeyboard,			// �g�p����f�o�C�X
		&g_InputKeyboardDevice,		// �쐬���ꂽIDirectInputDevice8�ۑ��p�ϐ��w��
		nullptr);					// nullptr�Œ�
	if (FAILED(hr))
	{
		// ���s
		return false;
	}

	/*
		�쐬���ꂽ�f�o�C�X���t�H�[�}�b�g(������)����
	*/
	hr = g_InputKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		return false;
	}

	hr = g_InputKeyboardDevice->SetCooperativeLevel(
		FindWindow(TEXT("WindowClass"), nullptr),
		DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
	{
		// ���s
		return false;
	}

	// ����J�n
	hr = g_InputKeyboardDevice->Acquire();
	if (FAILED(hr))
	{
		// ���s
		return false;
	}

	// ����
	return true;
}

void UpdateInput()
{
	// �L�[�{�[�h���i�[�p
	BYTE key_states[256];
	HRESULT hr;

	hr = g_InputKeyboardDevice->GetDeviceState(
		256,			// �f�o�C�X���̃T�C�Y 
		key_states);	// �f�o�C�X���i�[�p�ϐ��w��

	if (SUCCEEDED(hr))
	{
		for (int i = 0; i < 256; i++)
		{
			bool is_push = false;

			if (key_states[i] & 0x80)
			{
				is_push = true;
			}

			// �L�[�̏�Ԃ��X�V����
			g_KeyStates[i] = UpdateInputState(is_push, g_KeyStates[i]);
		}
	}
}

void ReleaseInput()
{
	if (g_InputKeyboardDevice != nullptr)
	{
		// �����~ <= Input�n�̃f�o�C�X�ł͕K�v
		g_InputKeyboardDevice->Unacquire();

		// �f�o�C�X�����
		g_InputKeyboardDevice->Release();
		g_InputKeyboardDevice = nullptr;
	}

	if (g_InputInterface != nullptr)
	{
		// �C���^�[�t�F�[�X�����
		g_InputInterface->Release();
		g_InputInterface = nullptr;
	}
}

bool IsKeyHeld(int key)
{
	// �v�f�ԍ��ȊO���ݒ肳��Ă��Ȃ����`�F�b�N
	if (key < 0 || key >= 256)
	{
		return false;
	}

	return g_KeyStates[key] == InputState::Held ? true : false;
}

bool IsKeyPushed(int key)
{
	// �v�f�ԍ��ȊO���ݒ肳��Ă��Ȃ����`�F�b�N
	if (key < 0 || key >= 256)
	{
		return false;
	}

	if (g_KeyStates[key] == InputState::Pushed)
	{
		return true;
	}

	return false;
}

bool IsKeyReleased(int key)
{
	// �v�f�ԍ��ȊO���ݒ肳��Ă��Ȃ����`�F�b�N
	if (key < 0 || key >= 256)
	{
		return false;
	}

	return g_KeyStates[key] == InputState::Released ? true : false;
}