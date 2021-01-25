#ifndef DIRECT_GRAPHICS_H_
#define DIRECT_GRAPHICS_H_

// DirectGraphics�ŕK�v�Ȋ֐�����`����Ă���w�b�_�t�@�C��
#include <d3d9.h>
#include <d3dx9.h>

// �e�N�X�`�������̒��_�\��(2D�p)
struct CustomVertex
{
	float X;
	float Y;
	float Z;
	float Rhw;
	DWORD Color;
};

// �e�N�X�`���L��̒��_�\��(2D�p)
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

// 3D�p�̒��_�\��
struct CustomVertex3D
{
	// ���W���
	float X;
	float Y;
	float Z;

	// �J���[
	DWORD Color;
};

// FVF�ݒ�(2D�p�e�N�X�`������)
#define FVF_2D_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
// FVF�ݒ�(2D�p�e�N�X�`���L��)
#define FVF_2D_VERTEX_TEXTURE (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// FVF�ݒ�(3D�p)
#define FVF_3D_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

// �e�N�X�`���̎�ޕ���enum���쐬
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

// View���W�ϊ��̐ݒ�
void SetUpView();
// �v���W�F�N�V�������W�ϊ��̐ݒ�
void SetUpProjection();
// 3D�|���S���`��
void Draw3DPorigon();
void DrawXFile(float pos_x, float pos_y, float pos_z, float angle);

bool LoadTexture(TextureID tex_id);
bool LoadXFile(LPCWSTR file_name);

void ReleaseXFile();
void ReleaseTexture();

void UpdateTPSCamera(float target_pos_x, float target_pos_y, float target_pos_z, float angle);

#endif
