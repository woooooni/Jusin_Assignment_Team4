#include "stdafx.h"
#include "ToolObjMgr.h"
#include "Functor.h"
#include "Obj.h"
#include "MainFrm.h"

CToolObjMgr::CToolObjMgr()
	: m_pTargetObj(nullptr)
	, m_pMainFrm(nullptr)
{
}


CToolObjMgr::~CToolObjMgr()
{
	for (UINT i = 0; i < m_vecObj.size(); ++i)
		Safe_Delete<CObj*>(m_vecObj[i]);

}

void CToolObjMgr::SwapObj(CObj * _pSrc, CObj * _pDest)
{

}

void CToolObjMgr::SaveObjData()
{
	// TODO :: SAVE.

}

void CToolObjMgr::LoadObjData()
{

}

void CToolObjMgr::SetTargetObj(int _iIdx)
{
	if (m_vecObj.size() < _iIdx)
	{
		AfxMessageBox(L"SetTarget 안되요.");
		return;
	}
	m_pTargetObj = m_vecObj[_iIdx];
}

void CToolObjMgr::DeleteObj(int _iIdx)
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

void CToolObjMgr::UpdateAllView()
{
	m_pMainFrm->Update_AllView();
}
