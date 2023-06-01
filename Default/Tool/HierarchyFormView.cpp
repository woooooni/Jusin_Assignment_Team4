// HierarchyFormView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "HierarchyFormView.h"
#include "ToolMgr.h"
#include "Obj.h"
#include "Player.h"
#include "Monster.h"
#include "ToolMgr.h"

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

	for (UINT i = 0; i < (UINT)OBJID::OBJ_END; ++i)
		m_ComboObjType.AddString(STR_OBJID[i].c_str());
}


void CHierarchyFormView::UpdateHierarchyView()
{
	UpdateData(TRUE);
	m_HiararchyList.SetCurSel(-1);
	m_HiararchyList.ResetContent();

	const vector<CObj*>& vecObj = CToolMgr::GetInst()->GetObjVec();
	
	auto& iter = vecObj.begin();

	int iIdx = 0;
	for (; iter != vecObj.end(); ++iter)
	{
		m_HiararchyList.AddString((*iter)->Get_ObjName().c_str());

		if (CToolMgr::GetInst()->GetTargetedObj() != nullptr && (*iter) == CToolMgr::GetInst()->GetTargetedObj())
			m_HiararchyList.SetCurSel(iIdx);

		++iIdx;
	}
		

	UpdateData(FALSE);
}




void CHierarchyFormView::OnBnClickedAddObjButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_ComboObjType.GetCurSel() < 0)
	{
		AfxMessageBox(L"타입을 선택하세요.");
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
		AfxMessageBox(L"잘못된 타입입니다.");
		return;
	}

	pObj->Initialize();
	CToolMgr::GetInst()->AddObj(pObj);

	if (m_HiararchyList.GetCurSel() < 0)
		m_HiararchyList.SetCurSel(0);
	else
		m_HiararchyList.SetCurSel(m_HiararchyList.GetCurSel() + 1);

	UpdateData(FALSE);
	CToolMgr::GetInst()->UpdateAllView();
}


void CHierarchyFormView::OnBnClickedDeleteObjButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

	CToolMgr::GetInst()->DeleteObj(iSelect);
	m_HiararchyList.DeleteString(iSelect);

	CToolMgr::GetInst()->UpdateAllView();

	UpdateData(FALSE);
	CToolMgr::GetInst()->UpdateAllView();
}




void CHierarchyFormView::OnBnClickedSaveButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO :: CALL SAVE FUNC
}



//void CHierarchyFormView::OnBnClickedButtonRenameObj()
//{
//	UpdateData(TRUE);
//	
//	int iSelectIdx = m_HiararchyList.GetCurSel();
//	if (iSelectIdx < 0)
//	{
//		AfxMessageBox(L"오브젝트를 선택하세요.");
//		return;
//	}
//		
//
//	CString str;
//	GetDlgItemText(IDC_EDIT_RENAME_OBJ, str);
//
//	if (str == L"")
//	{
//		AfxMessageBox(L"한 글자 이상 입력하세요.");
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iSelect = m_HiararchyList.GetCurSel();

	if (LB_ERR == iSelect)
		return;

	CToolMgr::GetInst()->SetTargetObj(iSelect);
	CToolMgr::GetInst()->UpdateAllView();
}

