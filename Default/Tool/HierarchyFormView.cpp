// HierarchyFormView.cpp : ���� �����Դϴ�.
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


void CHierarchyFormView::UpdateHierarchyView()
{
	UpdateData(TRUE);




	UpdateData(FALSE);
}

void CHierarchyFormView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	UpdateData(TRUE);
	CFormView::OnMouseMove(nFlags, point);

	UpdateData(FALSE);
}



void CHierarchyFormView::OnBnClickedAddObjButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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


