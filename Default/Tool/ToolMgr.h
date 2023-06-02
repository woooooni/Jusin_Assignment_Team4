#pragma once

class CObj;
class CMainFrame;
class CToolMgr
{
	SINGLETON(CToolMgr);

public:
	
	void				SwapObj(CObj* _pSrc, CObj* _pDest);

public:
	void				AddObj(CObj* _pObj)				{ m_vecObj.push_back(_pObj); }

	CObj*				GetTargetedObj()				{ return m_pTargetObj; }
	void				SetTargetObj(int _iIdx);
	void				DeleteObj(int _iIdx);

	vector<CObj*>&		GetObjVec()					{ return m_vecObj; }

public:
	void				SetMainFrm(CMainFrame* _pMainFrm)	{ m_pMainFrm = _pMainFrm; }
	CMainFrame*			GetMainFrm()						{ return m_pMainFrm; }
	void				UpdateAllView();

public:
	void	RenderObj();
	bool	PickObj(const D3DXVECTOR3 & vPos);

private:
	
	bool	ObjPicking_Dot(const D3DXVECTOR3 & vPos, const int & iIndex);

private:
	vector<CObj*>		m_vecObj;
	CObj*				m_pTargetObj;
	CMainFrame*			m_pMainFrm;
};

