#include <Windows.h>
#include "Graphics.h"
#include "Input.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")


LRESULT CALLBACK WindowProcedure(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam)
{
	switch (message_id)
	{	//
	case WM_CLOSE:PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(window_handle, message_id, wparam, lparam);
		break;
	}
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmpLine,
	INT nCmdShow)
{
	WNDCLASS window__class =
	{
		CS_HREDRAW | CS_VREDRAW,
		WindowProcedure,
		0,
		0,
		hInstance,
		LoadIcon(nullptr,IDI_APPLICATION),
		LoadCursor(nullptr,IDC_ARROW),
		nullptr,
		nullptr,
		TEXT("WindowClass"),
	};

	if (RegisterClass(&window__class) == 0)
	{
		//失敗
		return 0;
	}
	int width = 640;
	int height = 480;

	//ウィンドウ作成
	HWND window_handle = CreateWindow(
		TEXT("WindowClass"),
		TEXT("ウィンドウ生成サンプル"),
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME),
		CW_USEDEFAULT,	// x軸
		CW_USEDEFAULT,	// y軸
		width,
		height,
		NULL,
		NULL,
		hInstance,
		NULL);

	RECT window_rect;
	RECT client_rect;

	if (GetWindowRect(window_handle, &window_rect) == false)
	{
		//失敗
		return 0;
	}
	if (GetClientRect(window_handle, &client_rect) == false)
	{
		return 0;
	}
	//フレームサイズ
	int frame_size_x = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left);
	int frame_size_y = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top);
	//リサイズ
	int resize_width = frame_size_x + width;
	int resize_height = frame_size_y + height;

	SetWindowPos(
		window_handle,
		NULL,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		resize_width,
		resize_height,
		SWP_NOMOVE
	);

	ShowWindow(window_handle, SW_SHOW);
	if (InitGraphics(window_handle)==false)
	{
		return 0;
	}
	if (InitInput()==false)
	{
		return 0;
	}
	if (LoadTexture(TextureID::TexID)==false)
	{
		return 0;
	}
	LoadXFile(TEXT("Witchwait.x"));

	float angle = 0.0f;
	float pos_x = 0.0f;
	float pos_y = 0.0f;
	float pos_z = 0.0f;

	while (true)
	{
		MSG message;

		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//受信したデータを翻訳
				TranslateMessage(&message);
				//ウィンドウプロシージャに送信
				DispatchMessage(&message);
			}
		}
		else
		{
			UpdateInput();

			if (IsKeyHeld(DIK_D))
			{
				angle -= 3.0f;
			}
			else if(IsKeyHeld(DIK_A))
			{
				angle += 3.0f;
			}
			if (IsKeyHeld(DIK_W))
			{
				float vec_z = cosf(D3DXToRadian(angle));
				float vec_x = sinf(D3DXToRadian(angle));
				float speed = 2.0f;

				pos_x += vec_x * speed;
				pos_z += vec_z * speed;
			}

			UpdateTPSCamera(0.0f, 0.0f, 0.0f, 0.0f);
			StartDrawing();
			Draw3DPorigon();
			//UpdateDirectInput();
			//StartDrawing();
			//SetUpView(pos_x, pos_y, pos_z);
			//SetUpProjection();
			//FinishDrawing();
			DrawXFile(pos_x, pos_y, pos_z, angle);
			FinishDrawing();
		}
	}
	//解放
	ReleaseTexture();
	ReleaseGraphics();

	return 0;
}
