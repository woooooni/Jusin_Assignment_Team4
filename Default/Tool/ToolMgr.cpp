#include "stdafx.h"
#include "ToolMgr.h"
#include "Functor.h"
#include "Obj.h"
#include "MainFrm.h"
#include "Device.h"

CToolMgr::CToolMgr()
	: m_pTargetObj(nullptr)
	, m_pMainFrm(nullptr)
{
}


CToolMgr::~CToolMgr()
{
	for (UINT i = 0; i < m_vecObj.size(); ++i)
		Safe_Delete<CObj*>(m_vecObj[i]);

}

void CToolMgr::SwapObj(CObj * _pSrc, CObj * _pDest)
{

}

void CToolMgr::SaveObjData()
{
	// TODO :: SAVE.

}

void CToolMgr::LoadObjData()
{

}

void CToolMgr::SetTargetObj(int _iIdx)
{
	if (m_vecObj.size() < _iIdx)
	{
		AfxMessageBox(L"SetTarget 안되요.");
		return;
	}
	m_pTargetObj = m_vecObj[_iIdx];
}

void CToolMgr::DeleteObj(int _iIdx)
{
	if (m_vecObj.size() < _iIdx)
	{
		AfxMessageBox(L"Delete 안되요.");
		return;
	}

	auto& iter = m_vecObj.begin();
	for (; iter != m_vecObj.end(); ++iter)
	{
		if ((*iter) == m_vecObj[_iIdx])
		{
			Safe_Delete<CObj*>((*iter));
			m_vecObj.erase(iter);
			m_pTargetObj = nullptr;
			return;
		}
	}
}

void CToolMgr::UpdateAllView()
{
	m_pMainFrm->Update_AllView();
}

void CToolMgr::RenderObj()
{
	for (auto& iter : m_vecObj)
		iter->Render();
}
