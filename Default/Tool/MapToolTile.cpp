// MapToolTile.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Device.h"
#include "MapToolTile.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "TextureMgr.h"
#include "FileInfo.h"
// CMapToolTile

BOOL g_bIndex = FALSE;

IMPLEMENT_DYNCREATE(CMapToolTile, CFormView)

CMapToolTile::CMapToolTile()
	: CFormView(IDD_MAPTOOL_TILE)
{

}

CMapToolTile::~CMapToolTile()
{
	// 메모리누수 -> 이미지 메모리 Destroy
	for (auto iter : m_mapPngImg)
	{
		iter.second->Destroy();
		Safe_Delete(iter.second);
	}
	m_mapPngImg.clear();
}

void CMapToolTile::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1_JWA, m_TileListBox);
	DDX_Control(pDX, IDC_PICTURE_JWA, m_TilePicControl);
	DDX_Control(pDX, IDC_CHECK1_JWA, m_Check);
}

void CMapToolTile::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	DragAcceptFiles(true);

	ChangeWindowMessageFilterEx(m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_hWnd, WM_COPYDATA, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);

	
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
			m_TileListBox.AddString(szTile);
		}

	}

	//m_Check.SetCheck(TRUE);
	
}

BOOL CMapToolTile::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT nID, CCreateContext * pContext)
{
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

BEGIN_MESSAGE_MAP(CMapToolTile, CFormView)
	ON_LBN_SELCHANGE(IDC_LIST1_JWA, &CMapToolTile::OnListBox)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_CHECK1_JWA, &CMapToolTile::OnBnClickedIndexCheckBox)
END_MESSAGE_MAP()


// CMapToolTile 진단입니다.

#ifdef _DEBUG
void CMapToolTile::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMapToolTile::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMapToolTile 메시지 처리기입니다.


void CMapToolTile::OnListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//UpdateData(TRUE);

	CString strSelect = L"";

	int iSelect = m_TileListBox.GetCurSel();

	if (LB_ERR == iSelect)
		return;

	m_TileListBox.GetText(iSelect, strSelect);

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
	m_iDrawID = _tstoi(strSelect);
	//m_iDrawID = _wtoi(strSelect.GetString());

	CDevice::Get_Instance()->Render_Begin();

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_SecondSplitter.GetPane(0, 0));
	pToolView->m_bIsSelectTile = true;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", m_iDrawID);
	if (nullptr == (pTexInfo))
		return;

	D3DXMATRIX mScale, mWorld, mTrans;
	D3DXMatrixScaling(&mScale, WINCX / pTexInfo->tImgInfo.Width * 0.9f, WINCY / pTexInfo->tImgInfo.Height, 0.f);
	D3DXMatrixTranslation(&mTrans, 55.f, 20.f, 0.f);

	mWorld = mScale*mTrans;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&mWorld);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CDevice::Get_Instance()->Render_End(m_TilePicControl.m_hWnd);

	//UpdateData(FALSE);
}


void CMapToolTile::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);

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

		strRelative = CFileInfo::ConvertRelativePath(szFilePath);
		strFileName = PathFindFileName(strRelative);
		lstrcpy(szFileName, strFileName.GetString());
		PathRemoveExtension(szFileName);

		strFileName = szFileName;

		auto iter = m_mapPngImg.find(strFileName);

		if (iter == m_mapPngImg.end())
		{
			CImage* pPngImg = new CImage;
			pPngImg->Load(strRelative);

			m_mapPngImg.insert({ strFileName, pPngImg });
			m_TileListBox.AddString(szFileName);
		}
	}

	::DragFinish(hDropInfo);

	UpdateData(FALSE);

	CFormView::OnDropFiles(hDropInfo);
}


void CMapToolTile::OnBnClickedIndexCheckBox()
{

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_SecondSplitter.GetPane(0, 0));

	if (m_Check.GetCheck())
	{
		g_bIndex = TRUE;
	}

	// 체크 박스가 선택되어 있지 않은 상태라면
	else
	{
		g_bIndex = FALSE;
	}

	pToolView->Invalidate(FALSE);
}
