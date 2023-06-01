#include "stdafx.h"
#include "Player.h"
#include "TextureMgr.h"
#include "Device.h"
#include "AstarMgr.h"
#include "TimeMgr.h"


CPlayer::CPlayer()
{
}


CPlayer::~CPlayer()
{
	Release();
}

HRESULT CPlayer::Initialize(void)
{
	m_tInfo.vPos = { 400.f, 300.f, 0.f };
	m_wstrObjKey = L"Player";
	m_wstrStateKey = L"Dash";

	m_tFrame = { 0.f, 10.f };

	return S_OK;
}

int CPlayer::Update(void)
{
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		CAstarMgr::Get_Instance()->Start_Astar(m_tInfo.vPos, Get_Mouse() - __super::m_vScroll);
	}

	Move_Astar();

	D3DXMATRIX			matTrans;
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x + __super::m_vScroll.x,
		m_tInfo.vPos.y + __super::m_vScroll.y,
		m_tInfo.vPos.z);

	m_tInfo.matWorld = matTrans;

	return 0;
}

void CPlayer::Late_Update(void)
{
	__super::Move_Frame();

}

void CPlayer::Render(void)
{
	const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_wstrObjKey.c_str(),
		m_wstrStateKey.c_str(),
		(int)m_tFrame.fFrame);

	if (nullptr == pTexInfo)
		return;

	float		fX = pTexInfo->tImgInfo.Width / 2.f;
	float		fY = pTexInfo->tImgInfo.Height / 2.f;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&m_tInfo.matWorld);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(fX, fY, 0.f),
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CPlayer::Release(void)
{
}

void CPlayer::Move_Astar()
{
	list<TILE*>& BestList = CAstarMgr::Get_Instance()->Get_BestList();

	if (!BestList.empty())
	{
		D3DXVECTOR3		vDir = BestList.front()->vPos - m_tInfo.vPos;

		float fDistance = D3DXVec3Length(&vDir);
		D3DXVec3Normalize(&vDir, &vDir);

		m_tInfo.vPos += vDir * 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

		if (3.f >= fDistance)
			BestList.pop_front();

	}

}
