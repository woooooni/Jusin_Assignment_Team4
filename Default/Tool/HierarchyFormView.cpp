// HierarchyFormView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "HierarchyFormView.h"


// CHierarchyFormView

IMPLEMENT_DYNCREATE(CHierarchyFormView, CFormView)

CHierarchyFormView::CHierarchyFormView()
	: CFormView(IDD_HIERARCHYFORMVIEW)
{

}

CHierarchyFormView::~CHierarchyFormView()
{
}

void CHierarchyFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHierarchyFormView, CFormView)
END_MESSAGE_MAP()


// CHierarchyFormView �����Դϴ�.

#ifdef _DEBUG
void CHierarchyFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CHierarchyFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CHierarchyFormView �޽��� ó�����Դϴ�.
