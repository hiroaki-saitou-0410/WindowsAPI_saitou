#include "Graphics.h"

// XFileデータ保存用の構造体
/*
	なぜ、ヘッダファイルではなく、ソースファイルに構造体を宣言したか？
	XFile構造体を使用するファイルがDirectGraphics.cppだけだから
*/
struct XFile
{
	DWORD MaterialNum;				//!< メッシュ数
	LPD3DXMESH Meshes;				//!< メッシュ
	LPD3DXBUFFER Materials;			//!< メッシュマテリアル
	LPDIRECT3DTEXTURE9* Textures;	//!< テクスチャデータ
};
XFile g_XFile;

LPDIRECT3D9 g_Interface = nullptr;
LPDIRECT3DDEVICE9 g_Device = nullptr;
// テクスチャデータ
LPDIRECT3DTEXTURE9 g_Textures[TextureID::TexMax];

LPCWSTR g_TextureNameList[] =
{
	TEXT("bomb.png"),
};

bool InitGraphics(HWND window_handle)
{
	g_Interface = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_Interface == nullptr)
	{
		// 初期化失敗
		return false;
	}

	// Deviceの情報を設定するための構造体
	D3DPRESENT_PARAMETERS parameters;

	// 指定されたデータをサイズ分だけ0で初期化する
	ZeroMemory(&parameters, sizeof(D3DPRESENT_PARAMETERS));

	// バックバッファの数
	parameters.BackBufferCount = 1;

	parameters.BackBufferFormat = D3DFMT_UNKNOWN;

	// ウィンドウモード指定
	parameters.Windowed = true;

	parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

	parameters.EnableAutoDepthStencil = true;
	parameters.AutoDepthStencilFormat = D3DFMT_D24S8;

	HRESULT result = g_Interface->CreateDevice(
		// ディスプレイアダプタの種類
		D3DADAPTER_DEFAULT,
		// デバイスの種類
		D3DDEVTYPE_HAL,
		// デバイスが使用するウィンドウのハンドル
		window_handle,
		// デバイス制御の方法
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		// PRESENT_PARAMETERS
		&parameters,
		// Deviceの保存変数指定
		&g_Device
	);

	if (FAILED(result))
	{
		return false;
	}

	// プロジェクションの設定
	SetUpProjection();

	return true;
}

void ReleaseGraphics()
{
	if (g_Device != nullptr)
	{
		// Relase => 解放関数
		g_Device->Release();
		// 解放後の不正アクセス対応
		g_Device = nullptr;
	}

	if (g_Interface != nullptr)
	{
		// Release => 解放関数
		g_Interface->Release();
		// 解放後の不正アクセス対応
		g_Interface = nullptr;
	}
}


void StartDrawing()
{
	// ブレンドの設定(以下の設定では頂点カラーとテクスチャのαのブレンドを許可している)
	
	g_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);


	// α値の反映を有効にする
	
	g_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//ライティングを無効にする。
	g_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	// バックバッファをクリアする
	g_Device->Clear(
		0,						// 0固定
		nullptr,				// nullptr固定
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,		// D3DCLEAR_TARGET固定
		D3DCOLOR_XRGB(0, 255, 255),	// 塗りつぶす色
		1.0f,					// 1.0f固定
		0						// 0固定
	);

	g_Device->SetRenderState(D3DRS_ZENABLE, true);

	// DirectXで描画を開始する
	g_Device->BeginScene();
}

void FinishDrawing()
{
	// DirecXの描画を終了する
	g_Device->EndScene();

	// バックバッファをフロントバッファに転送する
	g_Device->Present(nullptr, nullptr, nullptr, nullptr);
}

void DrawPorigon()
{
	CustomVertex vertices[] =
	{
		{ 300.0f, 100.0f, 0.0f, 1.0f, 0x00ffffff },
		{ 500.0f, 300.0f, 0.0f, 1.0f, 0x00ffffff },
		{ 50.0f, 300.0f, 0.0f, 1.0f, 0x00ffffff },
	};

	// DirectXに頂点構造情報を通知
	g_Device->SetFVF(FVF_2D_VERTEX);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,			// ポリゴンの作り方
		1,							// ポリゴンの数
		vertices,					// 頂点リスト
		sizeof(CustomVertex)		// 1頂点のサイズ
	);
}

void DrawPorigonWithTriangleList()
{
	// テクスチャ有の頂点構造
	CustomVertex vertices[] =
	{
		// 1つ目
		{ 0.0f, 0.0f, 0.0f, 1.0f, 0xff0000 },
		{ 100.0f, 0.0f, 0.0f, 1.0f, 0xff0000 },
		{ 0.0f, 100.0f, 0.0f, 1.0f, 0xff0000 },

		// 2つ目
		{ 100.0f, 0.0f, 0.0f, 1.0f, 0x00ff00 },
		{ 100.0f, 100.0f, 0.0f, 1.0f, 0x00ff00 },
		{ 0.0f, 100.0f, 0.0f, 1.0f, 0x00ff00 },
	};

	g_Device->SetFVF(FVF_2D_VERTEX);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		2,						// 描画するポリゴン数 => 2
		vertices,
		sizeof(CustomVertex)
	);
}

void DrawPorigonWithTriangleStrip()
{
	CustomVertex vertices[] =
	{
		{ 300.0f, 0.0f, 0.0f, 1.0f, 0xffffff },
		{ 350.0f, 0.0f, 0.0f, 1.0f, 0x808080 },
		{ 300.0f, 200.0f, 0.0f, 1.0f, 0x808080 },
		{ 350.0f, 200.0f, 0.0f, 1.0f, 0x000000 }
	};

	g_Device->SetFVF(FVF_2D_VERTEX);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,	// D3DPT_TRIANGLESTRIPでポリゴン作成
		2,
		vertices,
		sizeof(CustomVertex)
	);
}

void DrawPorigonWithTriangleFan(TextureID tex_id)
{
	if (g_Textures[tex_id] == nullptr)
	{
		return;
	}

	CustomVertexTex vertices[] = {
		//						ARGB
		{ 0.0f,0.0f, 0.0f, 1.0f, 0xffffff00, 0.0f, 0.0f },
		{ 200.0f, 0.0f, 0.0f, 1.0f, 0xffffff00, 1.0f, 0.0f },
		{ 200.0f, 250.0f, 0.0f, 1.0f, 0xffffff00, 1.0f, 1.0f },
		{ 0.0f, 250.0f, 0.0f, 1.0f, 0xffffff00, 0.0f, 1.0f },
	};

	g_Device->SetFVF(FVF_2D_VERTEX_TEXTURE);

	g_Device->SetTexture(0, g_Textures[TextureID::TexID]);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		vertices,
		sizeof(CustomVertex)
	);
}

void DrawXFile(float pos_x, float pos_y, float pos_z, float angle)
{
	float scale_x = 1.0f;
	float scale_y = 1.0f;
	float scale_z = 1.0f;

	float rad_x = D3DXToRadian(0.0f);
	float rad_y = D3DXToRadian(angle);
	float rad_z = D3DXToRadian(0.0f);

	D3DXMATRIX matrix;
	D3DXMATRIX scale_matrix;
	D3DXMATRIX trans_matrix;
	D3DXMATRIX rotate_x_matrix;
	D3DXMATRIX rotate_y_matrix;
	D3DXMATRIX rotate_z_matrix;

	// D3DXMatrixIdentity => 単位行列化する
	D3DXMatrixIdentity(&matrix);

	// 拡縮
	D3DXMatrixScaling(&scale_matrix, scale_x, scale_y, scale_z);

	// 回転
	D3DXMatrixRotationX(&rotate_x_matrix, rad_x);
	D3DXMatrixRotationY(&rotate_y_matrix, rad_y);
	D3DXMatrixRotationZ(&rotate_z_matrix, rad_z);

	// 移動
	D3DXMatrixTranslation(&trans_matrix, pos_x, pos_y, pos_z);

	matrix *= scale_matrix;

	matrix *= rotate_x_matrix;
	matrix *= rotate_y_matrix;
	matrix *= rotate_z_matrix;

	matrix *= trans_matrix;


	g_Device->SetTransform(D3DTS_WORLD, &matrix);

	// 描画処理
	D3DXMATERIAL* materials = (D3DXMATERIAL*)g_XFile.Materials->GetBufferPointer();
	for (DWORD i = 0; i < g_XFile.MaterialNum; i++)
	{
		// マテリアルの設定
		g_Device->SetMaterial(&materials[i].MatD3D);
		// テクスチャの設定
		g_Device->SetTexture(0, g_XFile.Textures[i]);
		// メッシュを描画
		g_XFile.Meshes->DrawSubset(i);
	}
}

bool LoadTexture(TextureID tex_id)
{
	HRESULT hr = D3DXCreateTextureFromFile(
		g_Device,					// DirectX9のデバイス
		g_TextureNameList[tex_id],	// ファイル名
		&g_Textures[tex_id]			// 読み込まれたテクスチャ情報が保存される変数
	);

	// FAILED => HRESULT型の変数を指定して、その値が失敗していたらtrue
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool LoadXFile(LPCWSTR file_name)
{
	HRESULT hr = D3DXLoadMeshFromX(
		file_name,				// ●ファイル名
		D3DXMESH_SYSTEMMEM,		// メッシュ作成のオプション(D3DXMESH_SYSTEMMEM固定)
		g_Device,				// DirectGraphicsのデバイス
		nullptr,				// 現状はnullptrで良し
		&g_XFile.Materials,		// ●マテリアル保存用
		nullptr,				// 現状はnullptrで良し
		&g_XFile.MaterialNum,	// ●マテリアルの数保存用
		&g_XFile.Meshes			// ●メッシュデータ保存用
	);

	if (FAILED(hr))
	{
		return false;
	}

	// マテリアルの数だけテクスチャを保存できるようにする
	g_XFile.Textures = new LPDIRECT3DTEXTURE9[g_XFile.MaterialNum];

	// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
	D3DXMATERIAL* materials = (D3DXMATERIAL*)g_XFile.Materials->GetBufferPointer();

	for (int i = 0; i < g_XFile.MaterialNum; i++)
	{
		// pTextureFilenameがnullptrじゃなかったらテクスチャが使用されている
		if (materials[i].pTextureFilename != nullptr)
		{
			LPSTR char_file_name = materials[i].pTextureFilename;

			// LPSTR(char*) => LPWSTR(wchar_t*)変換
			wchar_t wtext[256];
			size_t ret;
			mbstowcs_s(&ret, wtext, char_file_name, strlen(char_file_name) + 1);
			LPWSTR file_name = wtext;

			D3DXCreateTextureFromFile(
				g_Device,
				file_name,
				&g_XFile.Textures[i]);
		}
		else
		{
			g_XFile.Textures[i] = nullptr;
		}
	}

	return true;
}

void ReleaseXFile()
{
	// テクスチャの解放
	for (int i = 0; i < g_XFile.MaterialNum; i++)
	{
		if (g_XFile.Textures[i] != nullptr)
		{
			g_XFile.Textures[i]->Release();
			g_XFile.Textures[i] = nullptr;
		}
	}

	// テクスチャ配列も解放する
	delete[] g_XFile.Textures;

	// マテリアルの解放
	if (g_XFile.Materials != nullptr)
	{
		g_XFile.Materials->Release();
		g_XFile.Materials = nullptr;
	}

	// メッシュの解放
	if (g_XFile.Meshes != nullptr)
	{
		g_XFile.Meshes->Release();
		g_XFile.Meshes = nullptr;
	}
}

void ReleaseTexture()
{
	for (int i = 0; i < TextureID::TexMax; i++)
	{
		if (g_Textures[i] != nullptr)
		{
			g_Textures[i]->Release();
			g_Textures[i] = nullptr;
		}
	}
}

// View座標変換の設定
void SetUpView()
{
	D3DXMATRIX matrix; // カメラ情報を保存するための行列
	D3DXVECTOR3 camera_pos = D3DXVECTOR3(0.0f, 0.0f, -200.0f);
	D3DXVECTOR3 eye_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up_vec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// L => Left H => Hand LH => 左手系でカメラ座標を設定する
	D3DXMatrixLookAtLH(
		&matrix,		// 結果保存用行列の指定
		&camera_pos,	// カメラ座標
		&eye_pos,		// 注視点座標
		&up_vec			// ワールド座標上におけるカメラの上向きのベクトル
	);

	// SetTransform => 座標変換系の情報をDirectXに通知する
	g_Device->SetTransform(D3DTS_VIEW, &matrix);
}

// プロジェクション座標変換の設定
void SetUpProjection()
{
	D3DXMATRIX matrix; // プロジェクション情報を保存するための行列
	float aspect = 640.0f / 480.0f;	// アスペクト比(ウィンドウサイズ横 / 縦)

	// Perspective => 透視投影
	D3DXMatrixPerspectiveFovLH(
		&matrix,				// 結果保存用の行列の指定
		// D3DXToRadian => 度数法をラジアンに変換
		D3DXToRadian(60.0f),	// 画角(ラジアン指定)
		aspect,					// アスペクト比
		0.1f,					// 視錐台のNear値
		1000.0f					// 視錐台のFar値
	);

	// SetTransform => 座標変換系の情報をDirectXに通知する
	g_Device->SetTransform(D3DTS_PROJECTION, &matrix);
}

// 3Dポリゴン描画
void Draw3DPorigon()
{
	// △ポリゴン描画
	CustomVertex3D vertices[]
	{
		{ -1.0f, -1.0f, 0.0f, 0xffff0000 },
		{ 0.0f, 1.0f, 0.0f, 0xffff0000 },
		{ 1.0f, -1.0f, 0.0f, 0xffff0000 }
	};

	g_Device->SetFVF(FVF_3D_VERTEX);

	g_Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 1, vertices, sizeof(CustomVertex3D));
}

void UpdateTPSCamera(float target_pos_x, float target_pos_y, float target_pos_z, float angle)
{
	// ターゲットの向きベクトルを求める
	float vec_x = sinf(D3DXToRadian(0.0f));
	float vec_z = cosf(D3DXToRadian(0.0f));

	// ベクトルの反転
	vec_x *= -1.0f;
	vec_z *= -1.0f;

	// ベクトルに距離を掛けて長さを変化させる
	float distance = 200.0f;
	vec_x *= distance;
	vec_z *= distance;

	// ターゲットの位置にベクトルを加算した値を新しいカメラの座標とする
	float new_campos_x = target_pos_x + vec_x;
	float new_campos_y = target_pos_y;
	float new_campos_z = target_pos_z + vec_z;

	D3DXMATRIX matrix; // カメラ情報を保存するための行列
	D3DXVECTOR3 camera_pos = D3DXVECTOR3(new_campos_x, new_campos_y, new_campos_z);
	D3DXVECTOR3 eye_pos = D3DXVECTOR3(target_pos_x, target_pos_y, target_pos_z);
	D3DXVECTOR3 up_vec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// L => Left H => Hand LH => 左手系でカメラ座標を設定する
	D3DXMatrixLookAtLH(
		&matrix,		// 結果保存用行列の指定
		&camera_pos,	// カメラ座標
		&eye_pos,		// 注視点座標
		&up_vec			// ワールド座標上におけるカメラの上向きのベクトル
	);

	// SetTransform => 座標変換系の情報をDirectXに通知する
	g_Device->SetTransform(D3DTS_VIEW, &matrix);
}
