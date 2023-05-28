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
UINT CHierarchyFormView::m_iObjCount = 0;

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
	DDX_Control(pDX, IDC_COMBO_OBJ_TYPE, m_ComboObjType);
}

BEGIN_MESSAGE_MAP(CHierarchyFormView, CFormView)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_ADDOBJ_BUTTON, &CHierarchyFormView::OnBnClickedAddObjButton)
	ON_BN_CLICKED(IDC_DELETE_OBJ_BUTTON2, &CHierarchyFormView::OnBnClickedDeleteObjButton)
	ON_BN_CLICKED(IDC_SAVE_BUTTON, &CHierarchyFormView::OnBnClickedSaveButton)
	ON_LBN_SELCHANGE(IDC_HIERARCHY_LIST, &CHierarchyFormView::OnLbnSelchangeHierarchyList)
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

	for (UINT i = 0; i < (UINT)OBJID::OBJ_END; ++i)
		m_ComboObjType.AddString(STR_OBJID[i].c_str());
}


void CHierarchyFormView::UpdateHierarchyView()
{
	UpdateData(TRUE);

	for (int i = 0; i < m_HiararchyList.GetSelCount(); ++i)
		m_HiararchyList.DeleteString(0);


	vector<CObj*> vecObj = CToolObjMgr::GetInst()->GetObjVec();
	
	auto iter = vecObj.begin();

	for (; iter != vecObj.end(); ++iter)
		m_HiararchyList.AddString((*iter)->Get_ObjName().c_str());

	UpdateData(FALSE);
}




void CHierarchyFormView::OnBnClickedAddObjButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);


	
	if (m_ComboObjType.GetCurSel() < 0)
	{
		AfxMessageBox(L"Ÿ���� �����ϼ���.");
		return;
	}

	wstring strObjName = L"OBJ_" + to_wstring(m_iObjCount++);
	CString str;
	m_ComboObjType.GetWindowTextW(str);

	CObj* pObj = nullptr;
	
	if (str == STR_OBJID[(UINT)OBJID::OBJ_PLAYER].c_str())
	{
		pObj = new CPlayer;
		pObj->Set_ObjName(L"PLAYER_" + strObjName);
		m_HiararchyList.AddString((L"PLAYER_" + strObjName).c_str());
	}

	else if (str == STR_OBJID[(UINT)OBJID::OBJ_MONSTER].c_str())
	{
		pObj = new CMonster;
		pObj->Set_ObjName(L"MONSTER_" + strObjName);
		m_HiararchyList.AddString((L"MONSTER_" + strObjName).c_str());
	}

	else
	{
		AfxMessageBox(L"�߸��� Ÿ���Դϴ�.");
		return;
	}

	CToolObjMgr::GetInst()->AddObj(pObj);

	if (m_HiararchyList.GetCurSel() < 0)
		m_HiararchyList.SetCurSel(0);
	else
		m_HiararchyList.SetCurSel(m_HiararchyList.GetCurSel() + 1);

	UpdateData(FALSE);
}


void CHierarchyFormView::OnBnClickedDeleteObjButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	CString	strFindName = L"";

	int		iSelect = m_HiararchyList.GetCurSel();

	if (LB_ERR == iSelect)
		return;

	m_HiararchyList.GetText(iSelect, strFindName);
	

	if (m_HiararchyList.GetCurSel() < 0)
		m_HiararchyList.SetCurSel(0);
	else
		m_HiararchyList.SetCurSel(m_HiararchyList.GetCurSel() - 1);

	m_iObjCount--;

	CToolObjMgr::GetInst()->DeleteObj(iSelect);
	m_HiararchyList.DeleteString(iSelect);

	UpdateData(FALSE);
}




void CHierarchyFormView::OnBnClickedSaveButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// TODO :: CALL SAVE FUNC
}



//void CHierarchyFormView::OnBnClickedButtonRenameObj()
//{
//	UpdateData(TRUE);
//	
//	int iSelectIdx = m_HiararchyList.GetCurSel();
//	if (iSelectIdx < 0)
//	{
//		AfxMessageBox(L"������Ʈ�� �����ϼ���.");
//		return;
//	}
//		
//
//	CString str;
//	GetDlgItemText(IDC_EDIT_RENAME_OBJ, str);
//
//	if (str == L"")
//	{
//		AfxMessageBox(L"�� ���� �̻� �Է��ϼ���.");
//		return;
//	}
//
//	m_HiararchyList.DeleteString(iSelectIdx);
//	m_HiararchyList.InsertString(iSelectIdx, str);
//
//	m_EditObjName.SetWindowTextW(L"");
//	UpdateData(FALSE);
//}


void CHierarchyFormView::OnLbnSelchangeHierarchyList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CToolObjMgr::GetInst()->SetTargetObj(m_HiararchyList.GetCurSel());

}
