#pragma once

typedef struct tagInfo
{
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vSize;		// ���� ������Ʈ ũ��.
	D3DXVECTOR3		vDir;
	D3DXVECTOR3		vLook;
	
	D3DXVECTOR3		vScale;		// ������Ʈ ũ�� ����.

	D3DXMATRIX		matWorld;
}INFO;

typedef	struct tagFrame
{
	float		fFrame; // �������� ���� ���� ����
	float		fMax;	// �ִ� �̹����� ������ ��
}FRAME;

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9		pTexture;   // �� ��ü

	D3DXIMAGE_INFO			tImgInfo;	// ����ü

}TEXINFO;

typedef struct tagTile
{
	D3DXVECTOR3	vPos;
	D3DXVECTOR3 vSize;

	BYTE		byOption;
	BYTE		byDrawID;

	// AstarMgr������ �߰��Ǵ� �κ�
	int iIndex = 0;
	int iParentIdx = 0;
}TILE;

typedef struct tagMapData
{
	wstring		wstrObjKey = L"";
	wstring		wstrStateKey = L"";

	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;
	D3DXVECTOR3	vCenter;

	int			iCount = 0;
	float		fScale;
	float		fRadius;
}MAP;

typedef	struct tagUnitData
{
	CString	strName;
	int		iAttack;
	int		iHp;

	BYTE	byJobIndex;
	BYTE	byItem;

}UNITDATA;

typedef struct tagTexturePath
{
	wstring		wstrObjKey		= L"";
	wstring		wstrStateKey	= L"";
	wstring		wstrPath = L"";
	int			iCount = 0;

}IMGPATH;


typedef struct tagAnimInfo_KJM
{
	wstring		wstrObjKey = L"";
	wstring		wstrStateKey = L"";  // statekey �� 
	wstring		wstrPath = L""; // ��� ���� 

	float		fFrame; // ���� ������
	float		fMax;	// �ִ� �̹����� ������ ��

	int			iAnimSpeed;  //�ִϸ��̼� �ӵ� 
}ANIMINFO_KJM;
 

static D3DXVECTOR3		Get_Mouse()
{
	POINT	Pt{};

	GetCursorPos(&Pt);
	ScreenToClient(g_hWnd, &Pt);

	return D3DXVECTOR3((float)Pt.x, (float)Pt.y, 0.f);
}