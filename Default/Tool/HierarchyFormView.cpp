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
	DDX_Control(pDX, IDC_HIAERACHY_TREE, m_TreeControl);
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


void CHierarchyFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_rootItem = m_TreeControl.InsertItem(L"ROOT", 0, 0, TVI_ROOT, TVI_LAST);


	//// ��ũ�� �����
	CSize sizeScroll;
	sizeScroll.cx = 10;  //0���ָ� �ƿ� �Ⱥ��̰� ��Ʈ�� ǥ�ø� ���� �ּҰ��� ����
	sizeScroll.cy = 800;
	SetScrollSizes(MM_TEXT, sizeScroll);
}
