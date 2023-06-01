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
#include "ToolMgr.h"
#include "MiniView.h"


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


// CDlgTab3 메시지 처리기입니다.


void CDlgTab3::OnSaveData()
{
	/*
	CString의 버퍼는 TCHAR타입, CStringA의 버터는 char타입이다.
	서로의 생성자에서 자동으로 변환된다.
	*/
	// 타일 저장
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

		// 타일을 몇개 깔 것인지에 대한 정보도 저장해야 한다.
		// 선언한 iTileX, iTileY에 값을 받아옴
		
		
		//WriteFile(hFile, &strMapFile, sizeof(CString), &dwByte, NULL); // m_strMyMap ( Map 이름 저장 )
		
 		pTerrain->Get_StageInfo(strMapName, iTileX, iTileY);
		pTerrain->Get_MapScale(fScale);

		dwStrByte = sizeof(TCHAR) * (strMapName.GetLength() + 1);

		WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, NULL);
		WriteFile(hFile, strMapName.GetString(), dwStrByte, &dwByte, NULL);

		WriteFile(hFile, &iTileX, sizeof(int), &dwByte, NULL); // m_iTileX 저장
		WriteFile(hFile, &iTileY, sizeof(int), &dwByte, NULL); // m_iTileY 저장
		WriteFile(hFile, &fScale, sizeof(float), &dwByte, NULL); // m_fMapSacle 저장

		for (auto pTile : vecTile) // 타일 저장
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// List초기화는 진행하지 않고 맵과 타일만 불러올 수 있도록 한다.
	CFileDialog Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data Files(*.dat) | *.dat||", this);

	TCHAR szPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\SaveData");
	Dlg.m_ofn.lpstrInitialDir = szPath;

	CToolView*		pToolView = CToolMgr::GetInst()->GetMainFrm()->GetToolView();
	CTerrain*		pTerrain = CToolMgr::GetInst()->GetMainFrm()->GetToolView()->GetTerrain();

	// 새로 생성할 필요 X 이미 만들어져 있는 것에 세팅을 다시 하면 된다.
	// 저장한 순서대로 불러와야한다.
	// iTileX, iTileY, strMapFile(맵이름), vecTile

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

		/* 이 내용 잠시 Terrain으로 옮겨감 (Load_TileData내로)

		HANDLE hFile = CreateFile(pGetPath, GENERIC_READ,
		0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
		return;

		//DWORD	dwStrByte = 0;
		TILE* pTile = nullptr;

		// 타일 불러오기전 vecTile 이 비어있지 않은 상태라면 Clear -------------------------> vecTile가지고 오는부분이 문제인 것 같음. 체크할 것
		vector<TILE*>& vecTile = pTerrain->Get_VecTile();
		for_each(vecTile.begin(), vecTile.end(), CDeleteObj());
		vecTile.clear();

		// vecTile이 비워진 상황이라면
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

		vecTile.push_back(pTile); // vecTile 세팅 완료
	}

	CloseHandle(hFile);
}


		*/
	}

		//CString strMapName = L"";

		/*
		//ReadFile(hFile, &strMapName, sizeof(CString), &dwByte, NULL);

		pTerrain->Set_StageInfo(iTileX, iTileY); // 불러온 iTileX, Y값 세팅 완료
		//pTerrain->Set_MyMap(strMapName); // 맵 이름 세팅 완료
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
