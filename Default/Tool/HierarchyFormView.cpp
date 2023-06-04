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
#include "MainFrm.h"
#include "Terrain.h"
#include "ToolView.h"

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
	// TODO :: SAVE.

	vector<CObj*>& vecObj = CToolMgr::GetInst()->GetObjVec();
	for (auto& obj : vecObj)
	{
		if (obj->GetAnimInfoMapSize() != 4)
		{
			wstring strErr = obj->Get_ObjName() + L"애니메이션을 모두 세팅해야 합니다.";
			AfxMessageBox(strErr.c_str());
			return;
		}
	}

	CFileDialog		Dlg(FALSE, L"dat", L"*.dat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data Files(*.dat) | *.dat||", this);

	TCHAR	szPath[MAX_PATH] = L"";
	CString strMap = L"";

	CTerrain*		pTerrain = CToolMgr::GetInst()->GetMainFrm()->GetToolView()->GetTerrain();
	strMap = pTerrain->Get_MapName();

	const TCHAR* szMap = strMap.GetString();


	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Data\\");
	lstrcat(szPath, szMap);

	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		CString		strTemp = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = strTemp.GetString();

		Save_ObjData(pGetPath);
	}
	CToolMgr::GetInst()->UpdateAllView();
}


HRESULT CHierarchyFormView::Save_ObjData(const TCHAR * _pPath)
{
	HANDLE hFile = CreateFile(_pPath, GENERIC_WRITE, 0, 0,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"SAVE FAILED");
		return E_FAIL;
	}

	DWORD	dwByte = 0;
	const vector<CObj*>& vecObj = CToolMgr::GetInst()->GetObjVec();

	for (auto& iter : vecObj)
	{
		WriteFile(hFile, &(iter->Get_Info()), sizeof(INFO), &dwByte, NULL);

		float fAngle = iter->Get_Angle();
		WriteFile(hFile, &fAngle, sizeof(float), &dwByte, NULL);

		OBJID eID = iter->Get_ObjID();
		WriteFile(hFile, &eID, sizeof(float), &dwByte, NULL);

		const wchar_t* strObjKey = iter->Get_ObjKey().c_str();
		WriteFile(hFile, strObjKey, wcslen(strObjKey) * sizeof(wchar_t), &dwByte, NULL);

		const wchar_t* strObjName = iter->Get_ObjName().c_str();
		WriteFile(hFile, strObjName, wcslen(strObjName) * sizeof(wchar_t), &dwByte, NULL);


		// 애니메이션 정보 저장.
		const map<wstring, vector<ANIMINFO_KJM>> mapAnimInfo = iter->GetAnimInfoMap();
		auto iter = mapAnimInfo.begin();
		while (iter != mapAnimInfo.end())
		{
			const wchar_t* strStateKey = iter->first.c_str();
			WriteFile(hFile, strStateKey, wcslen(strStateKey) * sizeof(wchar_t), &dwByte, NULL);

			for (UINT i = 0; i < iter->second.size(); ++i)
			{
				ANIMINFO_KJM tAnimInfo = iter->second[i];
				
				WriteFile(hFile, &tAnimInfo.fFrame, sizeof(float), &dwByte, NULL);
				WriteFile(hFile, &tAnimInfo.fMax, sizeof(float), &dwByte, NULL);
				WriteFile(hFile, &tAnimInfo.iAnimSpeed, sizeof(int), &dwByte, NULL);
				
				const wchar_t* strObjKey = tAnimInfo.wstrObjKey.c_str();
				WriteFile(hFile, strObjKey, wcslen(strObjKey) * sizeof(wchar_t), &dwByte, NULL);
				
				const wchar_t* strPath = tAnimInfo.wstrPath.c_str();
				WriteFile(hFile, strPath, wcslen(strPath) * sizeof(wchar_t), &dwByte, NULL);
				
				
				const wchar_t* strAnimState = tAnimInfo.wstrStateKey.c_str();
				WriteFile(hFile, strAnimState, wcslen(strAnimState) * sizeof(wchar_t), &dwByte, NULL);
			}

			++iter;
		}
	}

	CloseHandle(hFile);
	AfxMessageBox(L"저장되었습니다.");

	return S_OK;
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

