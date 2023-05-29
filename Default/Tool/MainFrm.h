
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once

#include "MyForm.h"
#include "MiniView.h"
#include "Include.h"


class CHierarchyFormView;
class CInspectorFormView;
class CToolView;

class CMainFrame : public CFrameWnd
{
	
protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void			Update_AllView();

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CToolBar				m_wndToolBar;
	CStatusBar				m_wndStatusBar;

public:
	CSplitterWnd			m_MainSplitter;
	CSplitterWnd			m_SecondSplitter;

private:
	CHierarchyFormView*		m_pHierarchy;
	CInspectorFormView*		m_pInspector;
	CToolView*				m_pToolView;

// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};




