#include "stdafx.h"
#include "Monster.h"
#include "Device.h"
#include "TextureMgr.h"
CMonster::CMonster()
	: CObj(OBJID::OBJ_MONSTER)
{

}


CMonster::~CMonster()
{
}

HRESULT CMonster::Initialize(void)
{
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Stage/Player/Dash/AKIHA_AKI_%d.png", TEX_MULTI, L"Monster", L"Dash", 10)))
	{
		AfxMessageBox(L"TileTexture Create Failed");
		return E_FAIL;
	}

	m_wstrObjKey = L"Monster";
	m_wstrStateKey = L"Dash";
	return E_NOTIMPL;
}

int CMonster::Update(void)
{
	return 0;
}

void CMonster::Late_Update(void)
{
}

void CMonster::Render(void)
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Monster", L"Dash", 5);

	
	m_tInfo.vSize = { float(pTexInfo->tImgInfo.Width * m_tInfo.vScale.x), float(pTexInfo->tImgInfo.Height * m_tInfo.vScale.y), 0.f };

	float fX = m_tInfo.vSize.x * 0.5f;
	float fY = m_tInfo.vSize.y * 0.5f;

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f  * m_tInfo.vScale.x, 1.f * m_tInfo.vScale.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fAngle));
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x - m_vScroll.x,
		m_tInfo.vPos.y - m_vScroll.y,
		0.f);

	matWorld = matScale * matRotZ * matTrans;

	//// 이미지에 행렬 반영
	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(m_tInfo.vSize.x / 2.f, m_tInfo.vSize.y / 2.f, 0.f), // 출력 중심 좌표
		nullptr, // 출력 위치
		D3DCOLOR_ARGB(255, 255, 255, 255));

	
}

void CMonster::Release(void)
{
}
