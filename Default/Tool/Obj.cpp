#include "stdafx.h"
#include "Obj.h"
#include "Texture.h"
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

void CObj::InsertAnimationInfo(const wstring& _strStateKey, const vector<ANIMINFO_KJM> _vecAnim)
{
	auto iter = m_mapAnimInfo.find(_strStateKey);

	if (iter != m_mapAnimInfo.end())
		m_mapAnimInfo.erase(_strStateKey);

	m_mapAnimInfo.insert({ _strStateKey, _vecAnim });
}

void CObj::InsertAnimTexture(const wstring & _strStateKey, const vector<CTexture*>& _vecTex)
{
	auto iter = m_mapAnimTex.find(_strStateKey);

	if (iter != m_mapAnimTex.end())
	{
		auto& iterMap = m_mapAnimTex.begin();
		while(iterMap != m_mapAnimTex.end())
		{
			for (UINT i = 0; i < iterMap->second.size(); ++i)
				Safe_Delete<CTexture*>(iterMap->second[i]);

			iterMap = m_mapAnimTex.erase(iterMap);
		}
	}

	m_mapAnimTex.insert({ _strStateKey, _vecTex });
}
