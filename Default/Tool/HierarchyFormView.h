#pragma once
#include "afxcmn.h"



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
	CTreeCtrl m_TreeControl;
	virtual void OnInitialUpdate();

	//void AddObj(CObj* _pObj);

private:
	HTREEITEM m_rootItem;
};


