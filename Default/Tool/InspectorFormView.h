#pragma once
#include "afxcmn.h"

// CInspectorFormView �� ���Դϴ�.

#include "DlgTab1.h"
#include "DlgTab2.h"
#include "DlgTab3.h"
#include "DlgTab4.h"

class CInspectorFormView : public CFormView
{
	DECLARE_DYNCREATE(CInspectorFormView)

protected:
	CInspectorFormView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CInspectorFormView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSPECTORFORMVIEW };
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
	void UpdateInspectorView();

public:
	CTabCtrl		m_tab;
	afx_msg void OnSelchangeInspectorTab(NMHDR *pNMHDR, LRESULT *pResult);

	CDlgTab1*		dlg1;
	CDlgTab2*		dlg2;
	CDlgTab3*		dlg3;
	CDlgTab4*		dlg4;
	virtual void OnInitialUpdate();
};


