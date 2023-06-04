// MapToolTile.cpp : ���� �����Դϴ�.
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
	// �޸𸮴��� -> �̹��� �޸� Destroy
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


// CMapToolTile �����Դϴ�.

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


// CMapToolTile �޽��� ó�����Դϴ�.


void CMapToolTile::OnListBox()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
	// ���� ���ڿ��� index���� count��ŭ �����Ѵ�.
	strSelect.Delete(0, i);

	// ���ڿ��� ������ ��ȯ���ִ� �Լ�
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	UpdateData(TRUE);

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

	// üũ �ڽ��� ���õǾ� ���� ���� ���¶��
	else
	{
		g_bIndex = FALSE;
	}

	pToolView->Invalidate(FALSE);
}
