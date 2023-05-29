#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CHierarchyFormView �� ���Դϴ�.

class CInspectorFormView;
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


