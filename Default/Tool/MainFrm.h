
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

#include "MyForm.h"
#include "MiniView.h"
#include "Include.h"


class CToolView;
class CHierarchyFormView;
class CInspectorFormView;
class CFileDirectoryView;

class CMainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void			Update_AllView();

public:
	CHierarchyFormView* GetHierarchyView()		{ return m_pHierarchy; }
	CInspectorFormView* GetInspectorView()		{ return m_pInspector; }
	CToolView*			GetToolView()			{ return m_pToolView; }
	CFileDirectoryView*	GetFileDirectoryView()	{ return m_pFileDirectoryView; }

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CToolBar				m_wndToolBar;
	CStatusBar				m_wndStatusBar;

public:
	CSplitterWnd			m_MainSplitter;
	CSplitterWnd			m_SecondSplitter;

private:
	CHierarchyFormView*		m_pHierarchy;
	CInspectorFormView*		m_pInspector;
	CToolView*				m_pToolView;
	CFileDirectoryView*		m_pFileDirectoryView;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};




