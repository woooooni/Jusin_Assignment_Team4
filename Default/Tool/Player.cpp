#include "stdafx.h"
#include "Player.h"
#include "TextureMgr.h"
#include "Texture.h"
#include "Device.h"


CPlayer::CPlayer()
	: CObj(OBJID::OBJ_PLAYER)
{
}


CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Initialize(void)
{
	return E_NOTIMPL;
}

int CPlayer::Update(void)
{
	return 0;
}

void CPlayer::Late_Update(void)
{
}

void CPlayer::Render(void)
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", 5);

	//float fX = pTexInfo->tImgInfo.Width * 0.5f;
	//float fY = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(0.f));
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x,
		m_tInfo.vPos.y,
		0.f);

	matWorld = matScale * matRotZ * matTrans;

	//// 이미지에 행렬 반영
	//CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(m_tInfo.vSize.x / 2.f, m_tInfo.vSize.y / 2.f, 0.f), // 출력 중심 좌표
		&D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f), // 출력 위치
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CPlayer::Release(void)
{
}
