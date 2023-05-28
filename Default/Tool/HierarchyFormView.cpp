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
	DDX_Control(pDX, IDC_HIERARCHY_LIST, m_HiararchyList);
}

BEGIN_MESSAGE_MAP(CHierarchyFormView, CFormView)
	ON_WM_MOUSEMOVE()
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


	//// ��ũ�� �����
	CSize sizeScroll;
	sizeScroll.cx = 10;  //0���ָ� �ƿ� �Ⱥ��̰� ��Ʈ�� ǥ�ø� ���� �ּҰ��� ����
	sizeScroll.cy = 800;
	SetScrollSizes(MM_TEXT, sizeScroll);
}


void CHierarchyFormView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CFormView::OnMouseMove(nFlags, point);
}
