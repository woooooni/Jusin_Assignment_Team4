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
#include "MiniView.h"


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
	switch (iIndex)
	{
	case 0:
		m_pTileForm->ShowWindow(SW_SHOW);
		m_pMapForm->ShowWindow(SW_HIDE);
		break;
	case 1:
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


// CDlgTab3 �޽��� ó�����Դϴ�.


void CDlgTab3::OnSaveData()
{
	/*
	CString�� ���۴� TCHARŸ��, CStringA�� ���ʹ� charŸ���̴�.
	������ �����ڿ��� �ڵ����� ��ȯ�ȴ�.
	*/
	// Ÿ�� ����
	CFileDialog		Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data Files(*.dat) | *.dat||", this);

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
/*
		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			ERR_MSG(L"SAVE FAILED");
			return;
		}

		DWORD	dwByte = 0;
		DWORD	dwStrByte = 0;
		int		iTileX = 0;
		int		iTileY = 0;
		float	fScale = 0.f;
		CString strMapName = L"";


		//vector<TILE*>&		vecTile	   = pTerrain->Get_VecTile();
		vector<TILE*>		vecTile = pTerrain->Get_VecTileForSave();

		// Ÿ���� � �� �������� ���� ������ �����ؾ� �Ѵ�.
		// ������ iTileX, iTileY�� ���� �޾ƿ�
		
		
		//WriteFile(hFile, &strMapFile, sizeof(CString), &dwByte, NULL); // m_strMyMap ( Map �̸� ���� )
		
 		pTerrain->Get_StageInfo(strMapName, iTileX, iTileY);
		pTerrain->Get_MapScale(fScale);

		dwStrByte = sizeof(TCHAR) * (strMapName.GetLength() + 1);

		WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, NULL);
		WriteFile(hFile, strMapName.GetString(), dwStrByte, &dwByte, NULL);

		WriteFile(hFile, &iTileX, sizeof(int), &dwByte, NULL); // m_iTileX ����
		WriteFile(hFile, &iTileY, sizeof(int), &dwByte, NULL); // m_iTileY ����
		WriteFile(hFile, &fScale, sizeof(float), &dwByte, NULL); // m_fMapSacle ����

		for (auto pTile : vecTile) // Ÿ�� ����
		{
			
			//WriteFile(hFile, &(pTile->vPos), sizeof(D3DXVECTOR3), &dwByte, NULL);
			//WriteFile(hFile, &(pTile->vSize), sizeof(D3DXVECTOR3), &dwByte, NULL);
			//WriteFile(hFile, &(pTile->byOption), sizeof(BYTE), &dwByte, NULL);
			//WriteFile(hFile, &(pTile->byDrawID), sizeof(BYTE), &dwByte, NULL);
			
			WriteFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);
		}

		CloseHandle(hFile);
*/
	}

}

void CDlgTab3::OnLoadData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// List�ʱ�ȭ�� �������� �ʰ� �ʰ� Ÿ�ϸ� �ҷ��� �� �ֵ��� �Ѵ�.
	CFileDialog Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data Files(*.dat) | *.dat||", this);

	TCHAR szPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\SaveData");
	Dlg.m_ofn.lpstrInitialDir = szPath;

	CToolView*		pToolView = CToolMgr::GetInst()->GetMainFrm()->GetToolView();
	CTerrain*		pTerrain = CToolMgr::GetInst()->GetMainFrm()->GetToolView()->GetTerrain();

	// ���� ������ �ʿ� X �̹� ������� �ִ� �Ϳ� ������ �ٽ� �ϸ� �ȴ�.
	// ������ ������� �ҷ��;��Ѵ�.
	// iTileX, iTileY, strMapFile(���̸�), vecTile

	if (IDOK == Dlg.DoModal())
	{
		CString		strTemp = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = strTemp.GetString();

	//	HANDLE hFile = CreateFile(pGetPath, GENERIC_READ,
	//		0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	//	ReadFile(hFile, &iTileX, sizeof(int), &dwByte, NULL);
	//	ReadFile(hFile, &iTileY, sizeof(int), &dwByte, NULL);

	//	pTerrain->Set_StageInfo(iTileX, iTileY);
		pTerrain->Load_TileData(pGetPath);

		/* �� ���� ��� Terrain���� �Űܰ� (Load_TileData����)

		HANDLE hFile = CreateFile(pGetPath, GENERIC_READ,
		0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
		return;

		//DWORD	dwStrByte = 0;
		TILE* pTile = nullptr;

		// Ÿ�� �ҷ������� vecTile �� ������� ���� ���¶�� Clear -------------------------> vecTile������ ���ºκ��� ������ �� ����. üũ�� ��
		vector<TILE*>& vecTile = pTerrain->Get_VecTile();
		for_each(vecTile.begin(), vecTile.end(), CDeleteObj());
		vecTile.clear();

		// vecTile�� ����� ��Ȳ�̶��
		while (true)
		{
		pTile = new TILE;

		//ReadFile(hFile, pTile->vPos, sizeof(D3DXVECTOR3), &dwByte, NULL);
		//ReadFile(hFile, pTile->vSize, sizeof(D3DXVECTOR3), &dwByte, NULL);
		//ReadFile(hFile, LPVOID(pTile->byOption), sizeof(BYTE), &dwByte, NULL);
		//ReadFile(hFile, LPVOID(pTile->byDrawID), sizeof(BYTE), &dwByte, NULL);


		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);
		if (0 == dwByte)
		{
			Safe_Delete(pTile);
			break;
		}

		vecTile.push_back(pTile); // vecTile ���� �Ϸ�
	}

	CloseHandle(hFile);
}


		*/
	}

		//CString strMapName = L"";

		/*
		//ReadFile(hFile, &strMapName, sizeof(CString), &dwByte, NULL);

		pTerrain->Set_StageInfo(iTileX, iTileY); // �ҷ��� iTileX, Y�� ���� �Ϸ�
		//pTerrain->Set_MyMap(strMapName); // �� �̸� ���� �Ϸ�
		*/

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
}
