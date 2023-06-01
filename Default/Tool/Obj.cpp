#include "stdafx.h"
#include "Obj.h"
//#include "TimeMgr.h"

D3DXVECTOR3 CObj::m_vScroll{};

CObj::CObj(OBJID _eID)
	: m_eID(_eID)
	, m_fAngle(0.f)
	, m_iAnimIdx(0)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	D3DXMatrixIdentity(&m_tInfo.matWorld);

	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	m_tInfo.vScale = { 1.f, 1.f, 0.f };
}


CObj::~CObj()
{
}

void CObj::SetAnimationInfo(const wstring& _strObjKey, const vector<ANIMINFO_KJM> _vecAnim)
{
	auto iter = m_mapAnim.find(_strObjKey);

	if (iter != m_mapAnim.end())
		m_mapAnim.erase(_strObjKey);

	m_mapAnim.insert({ _strObjKey, _vecAnim });
}