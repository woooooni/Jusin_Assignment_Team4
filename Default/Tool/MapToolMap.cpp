// MapToolMap.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapToolMap.h"
#include "TextureMgr.h"
#include "Device.h"
#include "MainFrm.h"
#include "ToolView.h"


// CMapToolMap

IMPLEMENT_DYNCREATE(CMapToolMap, CFormView)

CMapToolMap::CMapToolMap()
	: CFormView(IDD_MAPTOOL_MAP)
{

}

CMapToolMap::~CMapToolMap()
{
	for (auto iter : m_mapPngImg)
	{
		iter.second->Destroy();
		Safe_Delete(iter.second);
	}
	m_mapPngImg.clear();
}

void CMapToolMap::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1_JWA, m_MapCombo);
	DDX_Control(pDX, IDC_SLIDER_FORMAP_JWA, m_MapSacleSlider);
	DDX_Control(pDX, IDC_PICTURE_FORMAP_JWA, m_MapPicControl);
	DDX_Control(pDX, IDC_MAPLIST_JWA, m_MapListBox);
}

void CMapToolMap::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	/*
	CString strRelative = L"../Texture/Stage/Map";
	CString strAllFiles = L"\\*.*";
	CString strPath = strRelative + strAllFiles;

	// CFileFind : MFC의 클래스로 파일을 찾는데 유용하게 사용할 수 있음.
	CFileFind finder;
	BOOL bIsExist = finder.FindFile(strPath);
	// FindeFile() 주어진 경로의 파일을 찾는다.
	// *.*을 경로의 마지막 부분에 넣으면 그 경로의 하위로 모든 파일을 찾는다.
	// 파일이 존재하지 않으면 0을 리턴한다.

	CString strFileName = L"";
	CString strDirName = L""; // 경로 이름

	while (bIsExist)
	{
		bIsExist = finder.FindNextFile();
		// FindNextFile() 파일이 존재할 경우 다음 파일을 찾을 수 있도록 해준다.

		if (finder.IsDirectory()) // IsDirectory() 파일이 디렉토리이면 0이 아닌 수를 리턴한다.
		{
			if (strDirName == _T(".") || strDirName == _T("..") || strDirName == _T("Thumbs.db"))
				continue;

			m_MapCombo.AddString(strDirName);
		}
	}
	*/

	CString strRelative = L"";
	CString strFileName = L"";

	for (size_t i = 0; i < MAP_TEX; ++i)
	{
		TCHAR szMap[MAX_STR] = L"Map";
		TCHAR szIndex[MIN_STR] = L"";
		swprintf_s(szIndex, L"%d", (int)i);
		strFileName = lstrcat(szMap, szIndex);

		strRelative = L"../Texture/Stage/Terrain/Map";

		auto iter = m_mapPngImg.find(strFileName);

		if (iter == m_mapPngImg.end())
		{
			CImage* pPngImg = new CImage;
			pPngImg->Load(strRelative);

			m_mapPngImg.insert({ strFileName, pPngImg });
			//m_MapCombo.AddString(szMap);
			m_MapListBox.AddString(szMap);
		}

	}

}

BOOL CMapToolMap::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT nID, CCreateContext * pContext)
{
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

BEGIN_MESSAGE_MAP(CMapToolMap, CFormView)
	ON_CBN_SELCHANGE(IDC_COMBO1_JWA, &CMapToolMap::OnSelectMap)
	ON_LBN_SELCHANGE(IDC_MAPLIST_JWA, &CMapToolMap::OnListBox)
END_MESSAGE_MAP()


// CMapToolMap 진단입니다.

#ifdef _DEBUG
void CMapToolMap::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMapToolMap::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMapToolMap 메시지 처리기입니다.


void CMapToolMap::OnSelectMap()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strSelect = L"";

	int iSelect = m_MapCombo.GetCurSel();

	if (LB_ERR == iSelect)
		return;

	m_MapCombo.GetLBText(iSelect, strSelect);

	auto iter = m_mapPngImg.find(strSelect);
	if (iter == m_mapPngImg.end())
		return;

	//m_MapPicControl.SetBitmap(*(iter->second));

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

	CDevice::Get_Instance()->Render_Begin();

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_SecondSplitter.GetPane(0, 0));

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Map", L"Map", m_iDrawID);
	if (nullptr == (pTexInfo))
		return;

	D3DXMATRIX mScale, mWorld, mTrans;
	D3DXMatrixScaling(&mScale, WINCX / pTexInfo->tImgInfo.Width * 0.9f, WINCY / pTexInfo->tImgInfo.Height, 0.f);
	D3DXMatrixTranslation(&mTrans, 55.f, 20.f, 0.f);

	mWorld = mScale*mTrans;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&mWorld);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CDevice::Get_Instance()->Render_End(m_MapPicControl.m_hWnd);
}


void CMapToolMap::OnListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strSelect = L"";

	int iSelect = m_MapListBox.GetCurSel();

	if (LB_ERR == iSelect)
		return;

	m_MapListBox.GetText(iSelect, strSelect);

	auto iter = m_mapPngImg.find(strSelect);
	if (iter == m_mapPngImg.end())
		return;

	//m_MapPicControl.SetBitmap(*(iter->second));

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

	CDevice::Get_Instance()->Render_Begin();

	//CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	//CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_SecondSplitter.GetPane(0, 0));

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Map", L"Map", m_iDrawID);
	if (nullptr == (pTexInfo))
		return;

	D3DXMATRIX mScale, mWorld, mTrans;
	D3DXMatrixScaling(&mScale, WINCX / pTexInfo->tImgInfo.Width * 0.9f, WINCY / pTexInfo->tImgInfo.Height, 0.f);
	D3DXMatrixTranslation(&mTrans, 55.f, 20.f, 0.f);

	mWorld = mScale*mTrans;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&mWorld);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CDevice::Get_Instance()->Render_End(m_MapPicControl.m_hWnd);
}
