#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CHierarchyFormView �� ���Դϴ�.

class CHierarchyFormView : public CFormView
{
	DECLARE_DYNCREATE(CHierarchyFormView)

protected:
	CHierarchyFormView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CHierarchyFormView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HIERARCHYFORMVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

	//void AddObj(CObj* _pObj);

public:
	CListBox m_HiararchyList;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


