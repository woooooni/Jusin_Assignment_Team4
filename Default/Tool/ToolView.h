
// ToolView.h : CToolView 클래스의 인터페이스
//

#pragma once

#include "Include.h"
#include "Terrain.h"
#include "DlgTab3.h"
#include "ToolMgr.h"
class CToolDoc;
class CToolView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// 특성입니다.
public:
	CToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
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
public: // 추가
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

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

// 일부가 아닌 전체 맵 타일을 미니맵에 출력해오기