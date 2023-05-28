// HierarchyFormView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "HierarchyFormView.h"
#include "ToolObjMgr.h"
#include "Obj.h"
#include "Player.h"
#include "Monster.h"

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
	ON_BN_CLICKED(IDC_ADDOBJ_BUTTON, &CHierarchyFormView::OnBnClickedAddObjButton)
	ON_BN_CLICKED(IDC_DELETE_OBJ_BUTTON2, &CHierarchyFormView::OnBnClickedDeleteObjButton)
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


void CHierarchyFormView::UpdateHierarchyView()
{
	UpdateData(TRUE);




	UpdateData(FALSE);
}

void CHierarchyFormView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);
	CFormView::OnMouseMove(nFlags, point);

	UpdateData(FALSE);
}



void CHierarchyFormView::OnBnClickedAddObjButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CObj* pObj = nullptr;
	//TODO :: ADD OBJ FROM OBJ_MGR
	//CToolObjMgr::GetInst()->AddObj();
	
	
	m_HiararchyList.AddString(L"OBJ");

	m_HiararchyList.SetFocus();
	if (m_HiararchyList.GetSelCount() == 0)
		m_HiararchyList.SetCurSel(0);
	else
		m_HiararchyList.SetCurSel(m_HiararchyList.GetCurSel());


	UpdateData(FALSE);
	
}


void CHierarchyFormView::OnBnClickedDeleteObjButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CString		strFindName = L"";

	int		iSelect = m_HiararchyList.GetCurSel();

	if (LB_ERR == iSelect)
		return;

	m_HiararchyList.GetText(iSelect, strFindName);
	//TODO :: DELETE OBJ FROM OBJ_MGR

	m_HiararchyList.DeleteString(iSelect);

	UpdateData(FALSE);
}


