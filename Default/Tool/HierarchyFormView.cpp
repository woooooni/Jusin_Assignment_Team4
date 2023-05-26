// HierarchyFormView.cpp : 구현 파일입니다.
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


// CHierarchyFormView 진단입니다.

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


// CHierarchyFormView 메시지 처리기입니다.


void CHierarchyFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_rootItem = m_TreeControl.InsertItem(L"ROOT", 0, 0, TVI_ROOT, TVI_LAST);


	//// 스크롤 숨기기
	CSize sizeScroll;
	sizeScroll.cx = 10;  //0을주면 아예 안보이고 컨트롤 표시를 위한 최소값을 지정
	sizeScroll.cy = 800;
	SetScrollSizes(MM_TEXT, sizeScroll);
}
