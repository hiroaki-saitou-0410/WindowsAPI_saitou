#include "Graphics.h"

// XFile�f�[�^�ۑ��p�̍\����
/*
	�Ȃ��A�w�b�_�t�@�C���ł͂Ȃ��A�\�[�X�t�@�C���ɍ\���̂�錾�������H
	XFile�\���̂��g�p����t�@�C����DirectGraphics.cpp����������
*/
struct XFile
{
	DWORD MaterialNum;				//!< ���b�V����
	LPD3DXMESH Meshes;				//!< ���b�V��
	LPD3DXBUFFER Materials;			//!< ���b�V���}�e���A��
	LPDIRECT3DTEXTURE9* Textures;	//!< �e�N�X�`���f�[�^
};
XFile g_XFile;

LPDIRECT3D9 g_Interface = nullptr;
LPDIRECT3DDEVICE9 g_Device = nullptr;
// �e�N�X�`���f�[�^
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
		// ���������s
		return false;
	}

	// Device�̏���ݒ肷�邽�߂̍\����
	D3DPRESENT_PARAMETERS parameters;

	// �w�肳�ꂽ�f�[�^���T�C�Y������0�ŏ���������
	ZeroMemory(&parameters, sizeof(D3DPRESENT_PARAMETERS));

	// �o�b�N�o�b�t�@�̐�
	parameters.BackBufferCount = 1;

	parameters.BackBufferFormat = D3DFMT_UNKNOWN;

	// �E�B���h�E���[�h�w��
	parameters.Windowed = true;

	parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

	parameters.EnableAutoDepthStencil = true;
	parameters.AutoDepthStencilFormat = D3DFMT_D24S8;

	HRESULT result = g_Interface->CreateDevice(
		// �f�B�X�v���C�A�_�v�^�̎��
		D3DADAPTER_DEFAULT,
		// �f�o�C�X�̎��
		D3DDEVTYPE_HAL,
		// �f�o�C�X���g�p����E�B���h�E�̃n���h��
		window_handle,
		// �f�o�C�X����̕��@
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		// PRESENT_PARAMETERS
		&parameters,
		// Device�̕ۑ��ϐ��w��
		&g_Device
	);

	if (FAILED(result))
	{
		return false;
	}

	// �v���W�F�N�V�����̐ݒ�
	SetUpProjection();

	return true;
}

void ReleaseGraphics()
{
	if (g_Device != nullptr)
	{
		// Relase => ����֐�
		g_Device->Release();
		// �����̕s���A�N�Z�X�Ή�
		g_Device = nullptr;
	}

	if (g_Interface != nullptr)
	{
		// Release => ����֐�
		g_Interface->Release();
		// �����̕s���A�N�Z�X�Ή�
		g_Interface = nullptr;
	}
}


void StartDrawing()
{
	// �u�����h�̐ݒ�(�ȉ��̐ݒ�ł͒��_�J���[�ƃe�N�X�`���̃��̃u�����h�������Ă���)
	
	g_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);


	// ���l�̔��f��L���ɂ���
	
	g_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//���C�e�B���O�𖳌��ɂ���B
	g_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �o�b�N�o�b�t�@���N���A����
	g_Device->Clear(
		0,						// 0�Œ�
		nullptr,				// nullptr�Œ�
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,		// D3DCLEAR_TARGET�Œ�
		D3DCOLOR_XRGB(0, 255, 255),	// �h��Ԃ��F
		1.0f,					// 1.0f�Œ�
		0						// 0�Œ�
	);

	g_Device->SetRenderState(D3DRS_ZENABLE, true);

	// DirectX�ŕ`����J�n����
	g_Device->BeginScene();
}

void FinishDrawing()
{
	// DirecX�̕`����I������
	g_Device->EndScene();

	// �o�b�N�o�b�t�@���t�����g�o�b�t�@�ɓ]������
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

	// DirectX�ɒ��_�\������ʒm
	g_Device->SetFVF(FVF_2D_VERTEX);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,			// �|���S���̍���
		1,							// �|���S���̐�
		vertices,					// ���_���X�g
		sizeof(CustomVertex)		// 1���_�̃T�C�Y
	);
}

void DrawPorigonWithTriangleList()
{
	// �e�N�X�`���L�̒��_�\��
	CustomVertex vertices[] =
	{
		// 1��
		{ 0.0f, 0.0f, 0.0f, 1.0f, 0xff0000 },
		{ 100.0f, 0.0f, 0.0f, 1.0f, 0xff0000 },
		{ 0.0f, 100.0f, 0.0f, 1.0f, 0xff0000 },

		// 2��
		{ 100.0f, 0.0f, 0.0f, 1.0f, 0x00ff00 },
		{ 100.0f, 100.0f, 0.0f, 1.0f, 0x00ff00 },
		{ 0.0f, 100.0f, 0.0f, 1.0f, 0x00ff00 },
	};

	g_Device->SetFVF(FVF_2D_VERTEX);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		2,						// �`�悷��|���S���� => 2
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
		D3DPT_TRIANGLESTRIP,	// D3DPT_TRIANGLESTRIP�Ń|���S���쐬
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

	// D3DXMatrixIdentity => �P�ʍs�񉻂���
	D3DXMatrixIdentity(&matrix);

	// �g�k
	D3DXMatrixScaling(&scale_matrix, scale_x, scale_y, scale_z);

	// ��]
	D3DXMatrixRotationX(&rotate_x_matrix, rad_x);
	D3DXMatrixRotationY(&rotate_y_matrix, rad_y);
	D3DXMatrixRotationZ(&rotate_z_matrix, rad_z);

	// �ړ�
	D3DXMatrixTranslation(&trans_matrix, pos_x, pos_y, pos_z);

	matrix *= scale_matrix;

	matrix *= rotate_x_matrix;
	matrix *= rotate_y_matrix;
	matrix *= rotate_z_matrix;

	matrix *= trans_matrix;


	g_Device->SetTransform(D3DTS_WORLD, &matrix);

	// �`�揈��
	D3DXMATERIAL* materials = (D3DXMATERIAL*)g_XFile.Materials->GetBufferPointer();
	for (DWORD i = 0; i < g_XFile.MaterialNum; i++)
	{
		// �}�e���A���̐ݒ�
		g_Device->SetMaterial(&materials[i].MatD3D);
		// �e�N�X�`���̐ݒ�
		g_Device->SetTexture(0, g_XFile.Textures[i]);
		// ���b�V����`��
		g_XFile.Meshes->DrawSubset(i);
	}
}

bool LoadTexture(TextureID tex_id)
{
	HRESULT hr = D3DXCreateTextureFromFile(
		g_Device,					// DirectX9�̃f�o�C�X
		g_TextureNameList[tex_id],	// �t�@�C����
		&g_Textures[tex_id]			// �ǂݍ��܂ꂽ�e�N�X�`����񂪕ۑ������ϐ�
	);

	// FAILED => HRESULT�^�̕ϐ����w�肵�āA���̒l�����s���Ă�����true
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool LoadXFile(LPCWSTR file_name)
{
	HRESULT hr = D3DXLoadMeshFromX(
		file_name,				// ���t�@�C����
		D3DXMESH_SYSTEMMEM,		// ���b�V���쐬�̃I�v�V����(D3DXMESH_SYSTEMMEM�Œ�)
		g_Device,				// DirectGraphics�̃f�o�C�X
		nullptr,				// �����nullptr�ŗǂ�
		&g_XFile.Materials,		// ���}�e���A���ۑ��p
		nullptr,				// �����nullptr�ŗǂ�
		&g_XFile.MaterialNum,	// ���}�e���A���̐��ۑ��p
		&g_XFile.Meshes			// �����b�V���f�[�^�ۑ��p
	);

	if (FAILED(hr))
	{
		return false;
	}

	// �}�e���A���̐������e�N�X�`����ۑ��ł���悤�ɂ���
	g_XFile.Textures = new LPDIRECT3DTEXTURE9[g_XFile.MaterialNum];

	// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���Ď擾
	D3DXMATERIAL* materials = (D3DXMATERIAL*)g_XFile.Materials->GetBufferPointer();

	for (int i = 0; i < g_XFile.MaterialNum; i++)
	{
		// pTextureFilename��nullptr����Ȃ�������e�N�X�`�����g�p����Ă���
		if (materials[i].pTextureFilename != nullptr)
		{
			LPSTR char_file_name = materials[i].pTextureFilename;

			// LPSTR(char*) => LPWSTR(wchar_t*)�ϊ�
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
	// �e�N�X�`���̉��
	for (int i = 0; i < g_XFile.MaterialNum; i++)
	{
		if (g_XFile.Textures[i] != nullptr)
		{
			g_XFile.Textures[i]->Release();
			g_XFile.Textures[i] = nullptr;
		}
	}

	// �e�N�X�`���z����������
	delete[] g_XFile.Textures;

	// �}�e���A���̉��
	if (g_XFile.Materials != nullptr)
	{
		g_XFile.Materials->Release();
		g_XFile.Materials = nullptr;
	}

	// ���b�V���̉��
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

// View���W�ϊ��̐ݒ�
void SetUpView()
{
	D3DXMATRIX matrix; // �J��������ۑ����邽�߂̍s��
	D3DXVECTOR3 camera_pos = D3DXVECTOR3(0.0f, 0.0f, -200.0f);
	D3DXVECTOR3 eye_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up_vec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// L => Left H => Hand LH => ����n�ŃJ�������W��ݒ肷��
	D3DXMatrixLookAtLH(
		&matrix,		// ���ʕۑ��p�s��̎w��
		&camera_pos,	// �J�������W
		&eye_pos,		// �����_���W
		&up_vec			// ���[���h���W��ɂ�����J�����̏�����̃x�N�g��
	);

	// SetTransform => ���W�ϊ��n�̏���DirectX�ɒʒm����
	g_Device->SetTransform(D3DTS_VIEW, &matrix);
}

// �v���W�F�N�V�������W�ϊ��̐ݒ�
void SetUpProjection()
{
	D3DXMATRIX matrix; // �v���W�F�N�V��������ۑ����邽�߂̍s��
	float aspect = 640.0f / 480.0f;	// �A�X�y�N�g��(�E�B���h�E�T�C�Y�� / �c)

	// Perspective => �������e
	D3DXMatrixPerspectiveFovLH(
		&matrix,				// ���ʕۑ��p�̍s��̎w��
		// D3DXToRadian => �x���@�����W�A���ɕϊ�
		D3DXToRadian(60.0f),	// ��p(���W�A���w��)
		aspect,					// �A�X�y�N�g��
		0.1f,					// �������Near�l
		1000.0f					// �������Far�l
	);

	// SetTransform => ���W�ϊ��n�̏���DirectX�ɒʒm����
	g_Device->SetTransform(D3DTS_PROJECTION, &matrix);
}

// 3D�|���S���`��
void Draw3DPorigon()
{
	// ���|���S���`��
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
	// �^�[�Q�b�g�̌����x�N�g�������߂�
	float vec_x = sinf(D3DXToRadian(0.0f));
	float vec_z = cosf(D3DXToRadian(0.0f));

	// �x�N�g���̔��]
	vec_x *= -1.0f;
	vec_z *= -1.0f;

	// �x�N�g���ɋ������|���Ē�����ω�������
	float distance = 200.0f;
	vec_x *= distance;
	vec_z *= distance;

	// �^�[�Q�b�g�̈ʒu�Ƀx�N�g�������Z�����l��V�����J�����̍��W�Ƃ���
	float new_campos_x = target_pos_x + vec_x;
	float new_campos_y = target_pos_y;
	float new_campos_z = target_pos_z + vec_z;

	D3DXMATRIX matrix; // �J��������ۑ����邽�߂̍s��
	D3DXVECTOR3 camera_pos = D3DXVECTOR3(new_campos_x, new_campos_y, new_campos_z);
	D3DXVECTOR3 eye_pos = D3DXVECTOR3(target_pos_x, target_pos_y, target_pos_z);
	D3DXVECTOR3 up_vec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// L => Left H => Hand LH => ����n�ŃJ�������W��ݒ肷��
	D3DXMatrixLookAtLH(
		&matrix,		// ���ʕۑ��p�s��̎w��
		&camera_pos,	// �J�������W
		&eye_pos,		// �����_���W
		&up_vec			// ���[���h���W��ɂ�����J�����̏�����̃x�N�g��
	);

	// SetTransform => ���W�ϊ��n�̏���DirectX�ɒʒm����
	g_Device->SetTransform(D3DTS_VIEW, &matrix);
}
