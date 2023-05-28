#pragma once

class CObj;
class CToolObjMgr
{
	SINGLETON(CToolObjMgr);

public:
	
	void SwapObj(CObj* _pSrc, CObj* _pDest);
	void SaveObjData();
	void LoadObjData();

public:
	void AddObj(CObj* _pObj)			{ m_objList.push_back(_pObj); }
	CObj* GetTargetedObj()				{ return m_pTargetObj; }
	void SetTargetObj(CObj* _pObj)		{ m_pTargetObj = _pObj; }
	void DeleteObj();

private:
	list<CObj*> m_objList;
	CObj*		m_pTargetObj;
};

