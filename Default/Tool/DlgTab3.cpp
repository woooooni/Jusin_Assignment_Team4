// DlgTab3.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "ToolView.h"
#include "RenderMgr_JWA.h"
#include "MainFrm.h"
#include "Device.h"
#include "DlgTab3.h"
#include "TextureMgr.h"
#include "TileMgr.h"
#include "afxdialogex.h"
#include "FileInfo.h"


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
	DDX_Control(pDX, IDC_CHECK1_JWA, m_Check);
	DDX_Control(pDX, IDC_LIST1_JWA, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE_JWA, m_TilePicControl);
	DDX_Control(pDX, IDC_PICTURE_MINI_JWA, m_MiniPicControl);
	DDX_Control(pDX, IDC_RADIO1_JWA, m_RadioTile);
	DDX_Control(pDX, IDC_RADIO2_JWA, m_RadioTile);
}

BOOL CDlgTab3::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//pTerrainIndex = new CTerrain;

	DragAcceptFiles(true);

	ChangeWindowMessageFilterEx(m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_hWnd, WM_COPYDATA, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_hWnd, 0x0049/*WM_COPYGLOBALDATA*/, MSGFLT_ALLOW, NULL);

	TCHAR szTile[MAX_STR] = L"";
	TCHAR szIndex[MIN_STR] = L"";
	CString strRelative = L""; //
	CString strFileName = L""; //

	for (size_t i = 0; i < TILE_TEX; ++i)
	{
		TCHAR szTile[MAX_STR] = L"Tile";
		TCHAR szIndex[MIN_STR] = L"";
		swprintf_s(szIndex, L"%d", (int)i);
		strFileName = lstrcat(szTile, szIndex); ///////

		strRelative = L"../Texture/Stage/Terrain/Tile";

		auto iter = m_mapPngImg.find(strFileName);

		if (iter == m_mapPngImg.end())
		{
			CImage* pPngImg = new CImage;
			pPngImg->Load(strRelative);

			m_mapPngImg.insert({ strFileName, pPngImg });
			m_ListBox.AddString(szTile);
		}
	}
	m_RadioTile.SetCheck(TRUE);
	//m_Check.SetCheck(TRUE);


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
	
	// 맵
	/*
	CCreateContext mapcontext;
	ZeroMemory(&mapcontext, sizeof(mapcontext));

	CRect maprect;

	GetDlgItem(IDC_STATIC_JWA)->GetWindowRect(&maprect);
	ScreenToClient(&maprect);

	m_pMapForm = new CMapToolMap;
	m_pMapForm->Create(NULL, NULL, WS_CHILD | WS_VSCROLL | WS_HSCROLL, maprect, this, IDD_MAPTOOL_MAP, &mapcontext);
	m_pMapForm->OnInitialUpdate();
	m_pMapForm->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_JWA)->DestroyWindow();
	*/
	return TRUE;
}

void CDlgTab3::ShowForm(int iIndex)
{
	switch (iIndex)
	{
	case 0:
		m_pMapForm->ShowWindow(SW_HIDE);
		//m_pTileForm->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pMapForm->ShowWindow(SW_SHOW);
		//m_pTileForm->ShowWindow(SW_SHOW);
		break;
	}
}

BEGIN_MESSAGE_MAP(CDlgTab3, CDialogEx)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1_JWA, &CDlgTab3::OnListBox)
	ON_BN_CLICKED(IDC_RADIO2_JWA, &CDlgTab3::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1_JWA, &CDlgTab3::OnBnClickedRadio1)
	//ON_BN_CLICKED(IDC_CHECK1_JWA, &CToolView::OnBnClickedIndexCheckBox)
	ON_BN_CLICKED(IDC_CHECK1_JWA, &CDlgTab3::OnBnClickedInDexCheckBox)
	ON_BN_CLICKED(IDC_BUTTON1_JWA, &CDlgTab3::OnSaveData)
END_MESSAGE_MAP()


// CDlgTab3 메시지 처리기입니다.

void CDlgTab3::OnListBox()
{
	//UpdateData(TRUE);
	
	CString strSelect = L"";

	int iSelect = m_ListBox.GetCurSel();

	if (LB_ERR == iSelect)
		return;

	m_ListBox.GetText(iSelect, strSelect);

	auto iter = m_mapPngImg.find(strSelect);
	if (iter == m_mapPngImg.end())
		return;

	m_TilePicControl.SetBitmap(*(iter->second));

	int i = 0;

	for (; i < strSelect.GetLength(); ++i)
	{
		if (0 != isdigit(strSelect[i]))
			break;
	}
	// 현재 문자열의 index부터 count만큼 삭제한다.
	strSelect.Delete(0, i);

	// 문자열을 정수로 변환해주는 함수
	//m_iDrawID = _tstoi(strSelect);
	m_iDrawID = _wtoi(strSelect.GetString());

	
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	if (nullptr == (pFrameWnd))
		return;

	CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 0));
	if (nullptr == (pToolView))
		return;
	
	CDevice::Get_Instance()->Render_Begin();

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", m_iDrawID);
	if (nullptr == (pTexInfo))
		return;

	D3DXMATRIX mScale, mWorld, mTrans;
	D3DXMatrixScaling(&mScale, WINCX / pTexInfo->tImgInfo.Width, WINCY / pTexInfo->tImgInfo.Height, 0.f);
	D3DXMatrixTranslation(&mTrans, 55.f, 20.f, 0.f);

	mWorld = mScale*mTrans;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&mWorld);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CDevice::Get_Instance()->Render_End(m_TilePicControl.m_hWnd);
	
	//UpdateData(FALSE);
}

// 파일을 끌어다 놓을 때 호출( : WM_DROPFILES)
void CDlgTab3::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnDropFiles(hDropInfo);	

	TCHAR szFilePath[MAX_STR] = L"";
	TCHAR szFileName[MIN_STR] = L"";
	CString strRelative = L"";
	CString strFileName = L"";

	// DragQueryFile(hDropInfo, index, outstring, buffersize)
	// 두 번째 인자에 -1을 전달하면 드래그 앤 드롭된 전체 파일 개수를 리턴.
	int iCount = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	for (int i = 0; i < iCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_STR);

		strRelative = CFileInfo::Convert_RelativePath(szFilePath);
		strFileName = PathFindFileName(strRelative);
		lstrcpy(szFileName, strFileName.GetString());
		PathRemoveExtension(szFileName);

	//	m_ListBox.AddString(szFileName);

		strFileName = szFileName;

		auto iter = m_mapPngImg.find(strFileName);

		if (iter == m_mapPngImg.end())
		{
			CImage* pPngImg = new CImage;
			pPngImg->Load(strRelative);

			m_mapPngImg.insert({ strFileName, pPngImg });
			m_ListBox.AddString(szFileName);
		}
	}

	::DragFinish(hDropInfo);
}

void CDlgTab3::OnBnClickedInDexCheckBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	if (m_Check.GetCheck() == BST_CHECKED)
	{
		pTerrainIndex->Set_IndexTrue();
		pTerrainIndex->Index_Render();
		Invalidate(FALSE);
	}

	// 체크 박스가 선택되어 있지 않은 상태라면
	else if (m_Check.GetCheck() == BST_UNCHECKED)
	{
		pTerrainIndex->Set_IndexFalse();
		pTerrainIndex->Index_Render();
		Invalidate(FALSE);
	}
}


void CDlgTab3::OnSaveData()
{
	CFileDialog		Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data Files(*.dat) | *.dat||", this);

	TCHAR	szPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\SaveData");
	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		CString		strTemp = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = strTemp.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			ERR_MSG(L"SAVE FAILED");
			return;
		}

		CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		if (nullptr == pMainFrm)
			return;

		CToolView*		pTool = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
		if (nullptr == pTool)
			return;
		 
		CTerrain*		pTerrain = pTool->m_pTerrain;

		vector<TILE*>& vecTile = pTerrain->Get_VecTile();

		DWORD	dwByte = 0;

		for (auto& iter : vecTile)
			WriteFile(hFile, iter, sizeof(TILE), &dwByte, nullptr);

		CloseHandle(hFile);
	}
}

void CDlgTab3::OnBnClickedRadio2()
{
	//ShowForm(1);
}

void CDlgTab3::OnBnClickedRadio1()
{
	//ShowForm(0);
}

