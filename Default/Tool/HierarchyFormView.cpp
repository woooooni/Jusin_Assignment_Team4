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
	DDX_Control(pDX, IDC_HIERARCHY_LIST, m_HiararchyList);
}

BEGIN_MESSAGE_MAP(CHierarchyFormView, CFormView)
	ON_WM_MOUSEMOVE()
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


	//// 스크롤 숨기기
	CSize sizeScroll;
	sizeScroll.cx = 10;  //0을주면 아예 안보이고 컨트롤 표시를 위한 최소값을 지정
	sizeScroll.cy = 800;
	SetScrollSizes(MM_TEXT, sizeScroll);
}


void CHierarchyFormView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnMouseMove(nFlags, point);
}
