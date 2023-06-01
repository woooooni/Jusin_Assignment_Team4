// DlgTab3.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "Device.h"
#include "DlgTab3.h"
#include "TextureMgr.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "InspectorFormView.h"
#include "ToolMgr.h"

// CDlgTab3 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTab3, CDialogEx)

CDlgTab3::CDlgTab3(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_TAB3, pParent)
{
}

CDlgTab3::~CDlgTab3()
{
}

void CDlgTab3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE_MINI_JWA, m_MiniPicControl);
	DDX_Control(pDX, IDC_RADIO1_JWA, m_RadioTile);
	DDX_Control(pDX, IDC_RADIO2_JWA, m_RadioTile);
}

BOOL CDlgTab3::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_RadioTile.SetCheck(TRUE);
	
	// 맵
	CCreateContext mapcontext;
	ZeroMemory(&mapcontext, sizeof(mapcontext));

	CRect maprect;

	GetDlgItem(IDC_STATIC_JWA)->GetWindowRect(&maprect);
	ScreenToClient(&maprect);

	m_pMapForm = new CMapToolMap;
	m_pMapForm->Create(NULL, NULL, WS_CHILD | WS_VSCROLL | WS_HSCROLL, maprect, this, IDD_MAPTOOL_MAP, &mapcontext);
	m_pMapForm->OnInitialUpdate();
	m_pMapForm->ShowWindow(SW_HIDE);
	
	m_pTileForm = new CMapToolTile;
	m_pTileForm->Create(NULL, NULL, WS_CHILD | WS_VSCROLL | WS_HSCROLL, maprect, this, IDD_MAPTOOL_TILE, &mapcontext);
	m_pTileForm->OnInitialUpdate();
	m_pTileForm->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_STATIC_JWA)->DestroyWindow();

	
	// 미니맵
	CCreateContext context;
	ZeroMemory(&context, sizeof(context));
	CRect rect;

	GetDlgItem(IDC_PICTURE_MINI_JWA)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	m_pMini = new CMiniView();
	m_pMini->Create(NULL, NULL, WS_CHILD | WS_VSCROLL | WS_HSCROLL, rect, this, IDC_PICTURE_MINI_JWA, &context);
	m_pMini->OnInitialUpdate();
	m_pMini->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_PICTURE_MINI_JWA)->DestroyWindow();
	
	return TRUE;
}

void CDlgTab3::ShowForm(int iIndex)
{
	CToolView* pToolView = CToolMgr::GetInst()->GetMainFrm()->GetToolView();

	if (pToolView == nullptr)
	{
		AfxMessageBox(L"GetToolView Failed.");
	}
	switch (iIndex)
	{
	case 0:
		pToolView->SetToolMode(EDIT_MODE::EDIT_TILE);
		m_pTileForm->ShowWindow(SW_SHOW);
		m_pMapForm->ShowWindow(SW_HIDE);
		break;
	case 1:
		pToolView->SetToolMode(EDIT_MODE::EDIT_MAP);
		m_pTileForm->ShowWindow(SW_HIDE);
		m_pMapForm->ShowWindow(SW_SHOW);
		break;
	}
}

BEGIN_MESSAGE_MAP(CDlgTab3, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO2_JWA, &CDlgTab3::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1_JWA, &CDlgTab3::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_BUTTON1_JWA, &CDlgTab3::OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON1_JWA2, &CDlgTab3::OnLoadData)
END_MESSAGE_MAP()


// CDlgTab3 메시지 처리기입니다.


void CDlgTab3::OnSaveData()
{

	CFileDialog		Dlg(FALSE, L"dat", L"*.dat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data Files(*.dat) | *.dat||", this);

	TCHAR	szPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\SaveData");
	Dlg.m_ofn.lpstrInitialDir = szPath;

	CTerrain*		pTerrain = CToolMgr::GetInst()->GetMainFrm()->GetToolView()->GetTerrain();

	if (IDOK == Dlg.DoModal())
	{
		CString		strTemp = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = strTemp.GetString();

		pTerrain->Save_TileData(pGetPath);
	}

}

void CDlgTab3::OnLoadData()
{

	CFileDialog Dlg(TRUE, L"dat", L"*.dat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data Files(*.dat) | *.dat||", this);

	TCHAR szPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\SaveData");
	Dlg.m_ofn.lpstrInitialDir = szPath;

	CToolView*		pToolView = CToolMgr::GetInst()->GetMainFrm()->GetToolView();
	CTerrain*		pTerrain = CToolMgr::GetInst()->GetMainFrm()->GetToolView()->GetTerrain();

	if (IDOK == Dlg.DoModal())
	{
		CString		strTemp = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = strTemp.GetString();

		pTerrain->Load_TileData(pGetPath);
	}

	CDevice::Get_Instance()->Render_Begin();
	pTerrain->Map_Render();
	pTerrain->Load_TileRender();
	CDevice::Get_Instance()->Render_End();

	pToolView->Invalidate(FALSE);
	m_pMini->Invalidate(FALSE);

}

void CDlgTab3::OnBnClickedRadio2()
{
	ShowForm(1);
}

void CDlgTab3::OnBnClickedRadio1()
{
	ShowForm(0);

	// Map에서 Tile로 탭 전환을 했을 때 Apply되지 않는점 수정
	//CTerrain*		pTerrain = CToolMgr::GetInst()->GetMainFrm()->GetToolView()->GetTerrain();
	//pTerrain->Init_Tile(); -> 하면 타일 초기화된다... .........
	//pTerrain->Render();

	CMiniView* pMiniview = CToolMgr::GetInst()->GetMainFrm()->GetInspectorView()->GetDlgTab3()->GetMiniView();
	pMiniview->Invalidate(FALSE);
}
