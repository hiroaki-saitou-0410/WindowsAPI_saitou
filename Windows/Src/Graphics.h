#ifndef DIRECT_GRAPHICS_H_
#define DIRECT_GRAPHICS_H_

// DirectGraphicsで必要な関数が定義されているヘッダファイル
#include <d3d9.h>
#include <d3dx9.h>

// テクスチャ無しの頂点構造(2D用)
struct CustomVertex
{
	float X;
	float Y;
	float Z;
	float Rhw;
	DWORD Color;
};

// テクスチャ有りの頂点構造(2D用)
struct CustomVertexTex
{
	float X;
	float Y;
	float Z;
	float Rhw;
	DWORD Color;

	float Tu;
	float Tv;
};

// 3D用の頂点構造
struct CustomVertex3D
{
	// 座標情報
	float X;
	float Y;
	float Z;

	// カラー
	DWORD Color;
};

// FVF設定(2D用テクスチャ無し)
#define FVF_2D_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
// FVF設定(2D用テクスチャ有り)
#define FVF_2D_VERTEX_TEXTURE (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// FVF設定(3D用)
#define FVF_3D_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

// テクスチャの種類分のenumを作成
enum TextureID
{
	TexID,
	TexID2,
	TexMax,

};


bool InitGraphics(HWND window_handle);

void ReleaseGraphics();

void StartDrawing();

void FinishDrawing();

void DrawPorigon();

void DrawPorigonWithTriangleList();
void DrawPorigonWithTriangleStrip();
void DrawPorigonWithTriangleFan(TextureID tex_id);

// View座標変換の設定
void SetUpView();
// プロジェクション座標変換の設定
void SetUpProjection();
// 3Dポリゴン描画
void Draw3DPorigon();
void DrawXFile(float pos_x, float pos_y, float pos_z, float angle);

bool LoadTexture(TextureID tex_id);
bool LoadXFile(LPCWSTR file_name);

void ReleaseXFile();
void ReleaseTexture();

void UpdateTPSCamera(float target_pos_x, float target_pos_y, float target_pos_z, float angle);

#endif
