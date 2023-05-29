#include "stdafx.h"
#include "Tile.h"
#include "Device.h"
#include "TileMgr.h"
#include "TextureMgr.h"
#include "ToolView.h"


CTile::CTile()
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tInfo.vPos, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_tInfo.vSize, sizeof(D3DXVECTOR3));
}

CTile::~CTile()
{
	Release();
}

void CTile::Initialize()
{
	ZeroMemory(&m_tInfo, sizeof(INFO));

	m_tInfo.vSize = { 1.f,1.f,0.f };
	m_tInfo.byDrawID = 2;
	m_tInfo.byOption = 0;
}

int CTile::Update()
{
	return 0;
}

void CTile::Render()
{
}

void CTile::Render_MiniView()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", m_tInfo.byDrawID);

	float fX = pTexInfo->tImgInfo.Width * 0.5f;
	float fY = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f / 2.f, 1.f / 1.4f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(0.f));
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x / 2.f,
		m_tInfo.vPos.y / 1.4f,
		0.f);

	matWorld = matScale * matRotZ * matTrans;

	// 이미지에 행렬 반영
	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(fX, fY, 0.f), // 출력 중심 좌표
		nullptr, // 출력 위치
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CTile::Release()
{
}
