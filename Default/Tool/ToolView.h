
// ToolView.h : CToolView Ŭ������ �������̽�
//

#pragma once

#include "Include.h"
#include "Terrain.h"
#include "DlgTab3.h"
#include "ToolMgr.h"
class CToolDoc;
class CToolView : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// Ư���Դϴ�.
public:
	CToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void	OnInitialUpdate();
	afx_msg void	OnDestroy();

public:
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void	OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void	OnMouseLeave();


public:
	void			UpdateToolView();
	void			SetToolMode(EDIT_MODE _eState)
	{
		if (m_eEditMode == _eState)
			return;
		m_eEditMode = _eState;

		CToolMgr::GetInst()->UpdateAllView();
	}
	EDIT_MODE		GetToolMode() { return m_eEditMode; }
public: // �߰�
	CTerrain*		GetTerrain() { return m_pTerrain; }
	void			Set_TileIndex(int _iIndex) { m_iTileIndex = _iIndex; }

public:
	CTerrain*		m_pTerrain;
	CDlgTab3*		m_pTileTool;
	bool			m_bIsSelectTile = false;
	int				m_iTileIndex;

private:
	EDIT_MODE		m_eEditMode;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#ifndef _DEBUG  // ToolView.cpp�� ����� ����
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

// �Ϻΰ� �ƴ� ��ü �� Ÿ���� �̴ϸʿ� ����ؿ���