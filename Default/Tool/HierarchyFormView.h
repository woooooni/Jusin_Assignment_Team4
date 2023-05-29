#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CHierarchyFormView 폼 뷰입니다.

class CInspectorFormView;
class CHierarchyFormView : public CFormView
{
	DECLARE_DYNCREATE(CHierarchyFormView)

protected:
	CHierarchyFormView();           // 동적 만들기에 사용되는 protected 생성자입니다.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

	//void AddObj(CObj* _pObj);

public:
	void UpdateHierarchyView();

public:

	afx_msg void OnBnClickedAddObjButton();
	afx_msg void OnBnClickedDeleteObjButton();

	afx_msg void OnBnClickedSaveButton();

public:
	static UINT				m_iObjCount;

	CListBox				m_HiararchyList;
	CComboBox				m_ComboObjType;

	afx_msg void OnLbnSelchangeHierarchyList();
};


