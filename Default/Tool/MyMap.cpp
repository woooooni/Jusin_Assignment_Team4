#include "stdafx.h"
#include "MyMap.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ToolView.h"


CMyMap::CMyMap()
{
}


CMyMap::~CMyMap()
{
}

HRESULT CMyMap::Initialize(void)
{
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Stage/Terrain/Map/Map%d.png", TEX_MULTI, L"Map", L"Map", 5)))
	{
		AfxMessageBox(L"Map Texture Create Failed");
		return E_FAIL;
	}

	return S_OK;
}
