#include "stdafx.h"
#include "Logo.h"
#include "TextureMgr.h"
#include "SceneMgr.h"
#include "Device.h"


CLogo::CLogo()
{
}


CLogo::~CLogo()
{
	Release_Scene();
}

HRESULT CLogo::Ready_Scene()
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ImgLoadThreadFunc, this, 0, nullptr);

	return S_OK;
}

void CLogo::Update_Scene()
{
	if (GetAsyncKeyState(VK_RETURN))
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);

		DeleteCriticalSection(&m_Crt);

		CSceneMgr::Get_Instance()->Change_SceneMgr(CSceneMgr::STAGE);
		return;
	}
}

void CLogo::Late_Update_Scene()
{
}

void CLogo::Render_Scene()
{
	const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Logo");
	if (nullptr == pTexInfo)
		return;

	float		fX = pTexInfo->tImgInfo.Width / 2.f;
	float		fY = pTexInfo->tImgInfo.Height / 2.f;

	D3DXMATRIX		matTrans;
	D3DXMatrixTranslation(&matTrans, WINCX / 2.f, WINCY / 2.f, 0.f);

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(fX, fY, 0.f),
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	wstring wstrFullPath = CTextureMgr::Get_Instance()->Get_String();

	D3DXMatrixTranslation(&matTrans, 50.f, 500.f, 0.f);

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);

	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
		wstrFullPath.c_str(),
		wstrFullPath.length(),
		nullptr,
		0,
		D3DCOLOR_ARGB(255, 0, 0, 255));
}

void CLogo::Release_Scene()
{
}

size_t __stdcall CLogo::ImgLoadThreadFunc(LPVOID lpParam)
{
	CLogo*	pLogo = reinterpret_cast<CLogo*>(lpParam);

	EnterCriticalSection(&(pLogo->Get_Crt()));

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/JusinLogo.png", TEX_SINGLE, L"Logo")))
	{
		ERR_MSG(L"Logo Image Insert failed");
		return E_FAIL;
	}

	if (FAILED(CTextureMgr::Get_Instance()->Read_ImgPath(L"../Data/ImgPath.txt")))
	{
		ERR_MSG(L"ImgPath Load Failed");
		return E_FAIL;
	}


	CTextureMgr::Get_Instance()->Set_String(L"로딩 완료");

	LeaveCriticalSection(&(pLogo->Get_Crt()));

	return 0;
}
