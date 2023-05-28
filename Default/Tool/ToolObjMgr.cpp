#include "stdafx.h"
#include "ToolObjMgr.h"
#include "Functor.h"

CToolObjMgr::CToolObjMgr()
	: m_pTargetObj(nullptr)
{
}


CToolObjMgr::~CToolObjMgr()
{
}

void CToolObjMgr::SwapObj(CObj * _pSrc, CObj * _pDest)
{

}

void CToolObjMgr::SaveObjData()
{

}

void CToolObjMgr::LoadObjData()
{

}

void CToolObjMgr::DeleteObj()
{
	m_objList.remove(m_pTargetObj);
	Safe_Delete<CObj*>(m_pTargetObj);
	m_pTargetObj = nullptr;
}
