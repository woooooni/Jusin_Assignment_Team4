#pragma once

typedef struct tagInfo
{
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vSize;		// 실제 오브젝트 크기.
	D3DXVECTOR3		vDir;
	D3DXVECTOR3		vLook;
	
	D3DXVECTOR3		vScale;		// 오브젝트 크기 배율.

	D3DXMATRIX		matWorld;
}INFO;

typedef	struct tagFrame
{
	float		fFrame; // 프레임을 세기 위한 변수
	float		fMax;	// 최대 이미지의 프레임 수
}FRAME;

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9		pTexture;   // 컴 객체

	D3DXIMAGE_INFO			tImgInfo;	// 구조체

}TEXINFO;

typedef struct tagTile
{
	D3DXVECTOR3	vPos;
	D3DXVECTOR3 vSize;

	BYTE		byOption;
	BYTE		byDrawID;

	// AstarMgr때문에 추가되는 부분
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
	wstring		wstrStateKey = L"";  // statekey 값 
	wstring		wstrPath = L""; // 경로 정보 

	float		fFrame; // 시작 프레임
	float		fMax;	// 최대 이미지의 프레임 수

	int			iAnimSpeed;  //애니메이션 속도 
}ANIMINFO_KJM;
 

static D3DXVECTOR3		Get_Mouse()
{
	POINT	Pt{};

	GetCursorPos(&Pt);
	ScreenToClient(g_hWnd, &Pt);

	return D3DXVECTOR3((float)Pt.x, (float)Pt.y, 0.f);
}