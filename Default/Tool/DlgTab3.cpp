// DlgTab3.cpp : ���� �����Դϴ�.
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
#include "ToolMgr.h"


// CDlgTab3 ��ȭ �����Դϴ�.

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

	//pTerrainIndex = new CTerrain;
	/*
	DragAcceptFiles(true);

	ChangeWindowMessageFilterEx(m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_hWnd, WM_COPYDATA, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);

	TCHAR szTile[MAX_STR] = L"";
	TCHAR szIndex[MIN_STR] = L"";
	CString strRelative = L"";
	CString strFileName = L"";

	
	for (size_t i = 0; i < TILE_TEX; ++i)
	{
		TCHAR szTile[MAX_STR] = L"Tile";
		TCHAR szIndex[MIN_STR] = L"";
		swprintf_s(szIndex, L"%d", (int)i);
		strFileName = lstrcat(szTile, szIndex);

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
	*/
	m_RadioTile.SetCheck(TRUE);
	
	// ��
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

	
	// �̴ϸ�
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
	//ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_RADIO2_JWA, &CDlgTab3::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1_JWA, &CDlgTab3::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_BUTTON1_JWA, &CDlgTab3::OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON1_JWA2, &CDlgTab3::OnLoadData)
END_MESSAGE_MAP()


// CDlgTab3 �޽��� ó�����Դϴ�.

void CDlgTab3::OnListBox()
{
	//UpdateData(TRUE);
	
	/*
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
	// ���� ���ڿ��� index���� count��ŭ �����Ѵ�.
	strSelect.Delete(0, i);

	// ���ڿ��� ������ ��ȯ���ִ� �Լ�
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
	*/
	//UpdateData(FALSE);
}

// ������ ����� ���� �� ȣ��( : WM_DROPFILES)
void CDlgTab3::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	/*
	CDialogEx::OnDropFiles(hDropInfo);	

	TCHAR szFilePath[MAX_STR] = L"";
	TCHAR szFileName[MIN_STR] = L"";
	CString strRelative = L"";
	CString strFileName = L"";

	// DragQueryFile(hDropInfo, index, outstring, buffersize)
	// �� ��° ���ڿ� -1�� �����ϸ� �巡�� �� ��ӵ� ��ü ���� ������ ����.
	int iCount = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	for (int i = 0; i < iCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_STR);

		strRelative = CFileInfo::ConvertRelativePath(szFilePath);
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
	*/
}


void CDlgTab3::OnSaveData()
{
	// Ÿ��(��) ����
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

		DWORD	dwByte;
		int		iTileX = 0;
		int		iTileY = 0;

		CTerrain*		pTerrain = CToolMgr::GetInst()->GetMainFrm()->GetToolView()->GetTerrain();
		CString			strMapFile = pTerrain->m_strMyMap;

		vector<TILE*>& vecTile = pTerrain->Get_VecTile();

		// Ÿ���� � �� �������� ���� ������ �����ؾ� �Ѵ�.
		pTerrain->Get_StageInfo(iTileX, iTileY);

		WriteFile(hFile, &iTileX, sizeof(int), &dwByte, NULL);
		WriteFile(hFile, &iTileY, sizeof(int), &dwByte, NULL);

		for (auto pTile : vecTile)
		{
			WriteFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);
		}

		WriteFile(hFile, &strMapFile, sizeof(CString), &dwByte, NULL);

		CloseHandle(hFile);

		/*
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
		*/
	}
}

void CDlgTab3::OnLoadData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// ����ʱ�ȭ�� �������� �ʰ� �ʰ� Ÿ�ϸ� �ҷ��� �� �ֵ��� �Ѵ�.
	CFileDialog Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data Files(*.dat) | *.dat||", this);

	TCHAR szPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\SaveData");
	Dlg.m_ofn.lpstrInitialDir = szPath;

	CTerrain*		pTerrain = CToolMgr::GetInst()->GetMainFrm()->GetToolView()->GetTerrain();

	if (IDOK == Dlg.DoModal())
	{
		CString		strTemp = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = strTemp.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_READ,
			0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD	dwByte;
		int iTileX = 0;
		int iTileY = 0;
		CString strMapName = L"";

		// ���� ������ �ʿ� X �̹� ������� �ִ� �Ϳ� ������ �ٽ� �ϸ� �ȴ�.
		// ������ ������� �ҷ��;��Ѵ�.
		// iTileX, iTileY, vecTile, strMapFile(���̸�)
		
		//vector<TILE*>& vecTile = pTerrain->m_vecTile;
		vector<TILE*>& vecTile = pTerrain->Get_VecTile();

		ReadFile(hFile, &iTileX, sizeof(int), &dwByte, NULL);
		ReadFile(hFile, &iTileY, sizeof(int), &dwByte, NULL);

		pTerrain->Get_StageInfo(iTileX, iTileY); // �ҷ��� iTileX, Y�� ���� �Ϸ�

		while (true)
		{
			TILE*	pTile = new TILE;
			ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);

			if (0 == dwByte)
			{
				Safe_Delete(pTile);
				break;
			}

			vecTile.push_back(pTile); // vecTile ���� �Ϸ�
		}
		
		ReadFile(hFile, &strMapName, sizeof(CString), &dwByte, NULL);

		pTerrain->Set_MyMap(strMapName);

		CloseHandle(hFile);
	}

	CDevice::Get_Instance()->Render_Begin();
	pTerrain->Render();
	CDevice::Get_Instance()->Render_End();
	
}

void CDlgTab3::OnBnClickedRadio2()
{
	ShowForm(1);
}

void CDlgTab3::OnBnClickedRadio1()
{
	ShowForm(0);
}
