#pragma once

class CObj;
class CToolObjMgr
{
	SINGLETON(CToolObjMgr);

public:
	
	void				SwapObj(CObj* _pSrc, CObj* _pDest);
	void				SaveObjData();
	void				LoadObjData();

public:
	void				AddObj(CObj* _pObj)				{ m_vecObj.push_back(_pObj); }

	CObj*				GetTargetedObj()				{ return m_pTargetObj; }
	void				SetTargetObj(int _iIdx);
	void				DeleteObj(int _iIdx);

	vector<CObj*>&		GetObjVec()					{ return m_vecObj; }

private:
	vector<CObj*>		m_vecObj;
	CObj*				m_pTargetObj;
};

