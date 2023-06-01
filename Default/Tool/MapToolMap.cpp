// MapToolMap.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Device.h"
#include "Tool.h"
#include "ToolMgr.h"
#include "ToolView.h"
#include "TextureMgr.h"
#include "MainFrm.h"
#include "InspectorFormView.h"

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
	DDX_Control(pDX, IDC_SLIDER_FORMAP_JWA, m_MapScaleSlider);
	DDX_Control(pDX, IDC_PICTURE_FORMAP_JWA, m_MapPicControl);
	DDX_Control(pDX, IDC_MAPLIST_JWA, m_MapListBox);
	DDX_Control(pDX, IDC_BUTTON_APPLY_JWA, m_ButtonApply);
}

void CMapToolMap::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

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
			m_MapListBox.AddString(szMap);
		}
	}

	// m_pMyMap 초기화
	m_pMyMap = CToolMgr::GetInst()->GetMainFrm()->GetToolView()->GetTerrain();

	// Map Scale Slider 초기화
	//m_MapScaleSlider.SetRange(200, 800);
	//m_MapScaleSlider.SetPos(400);
	m_MapScaleSlider.SetRange(0, 200);
	m_MapScaleSlider.SetPos(100);
	m_MapScaleSlider.SetTicFreq(1);
	m_MapScaleSlider.SetPageSize(1);

	if (m_MapCombo.GetCount() > 0)
		m_MapCombo.SetCurSel(0);

}

BOOL CMapToolMap::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT nID, CCreateContext * pContext)
{
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

BEGIN_MESSAGE_MAP(CMapToolMap, CFormView)
	ON_LBN_SELCHANGE(IDC_MAPLIST_JWA, &CMapToolMap::OnListBox)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_JWA, &CMapToolMap::OnBnClickedButtonApply)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_FORMAP_JWA, &CMapToolMap::OnMoveMapScaleSlider)
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

// 맵을 선택할 경우
void CMapToolMap::OnListBox()
{
	int iSelect = m_MapListBox.GetCurSel();

	if (LB_ERR == iSelect)
		return;

	CString strMapName = L"";
	m_MapListBox.GetText(iSelect, strMapName);

	m_pMyMap->Set_MyMap(strMapName);

	CMainFrame*				pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	CInspectorFormView*		pInspectorForm = dynamic_cast<CInspectorFormView*>(pMainFrm->m_MainSplitter.GetPane(0, 2));
	CDlgTab3*		pTool = pInspectorForm->dlg3;
	
	auto iter = m_mapPngImg.find(strMapName);
	if (iter == m_mapPngImg.end())
		return;

	int i = 0;
	for (; i < strMapName.GetLength(); ++i)
	{
		if (0 != isdigit(strMapName[i]))
			break;
	}

	strMapName.Delete(0, i);
	m_iDrawID = _tstoi(strMapName);

	CDevice::Get_Instance()->Render_Begin();

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_SecondSplitter.GetPane(0, 0));

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"PreMap", L"Map", m_iDrawID);
	if (nullptr == (pTexInfo))
		return;

	// 미리 MapHeight와 Width를 설정함.
	const TEXINFO*		pTexture = CTextureMgr::Get_Instance()->Get_Texture(
		L"Map", L"Map", m_iDrawID);
	
	int iMapHeight = pTexture->tImgInfo.Height;
	int iMapWidth = pTexture->tImgInfo.Width;

	CTerrain*		pTerrain = CToolMgr::GetInst()->GetMainFrm()->GetToolView()->GetTerrain();
	pTerrain->Set_MapInfo(iMapWidth, iMapHeight);
	//

	D3DXMATRIX mScale, mWorld, mTrans;
	D3DXMatrixScaling(&mScale, WINCX / pTexInfo->tImgInfo.Width * 1.2f, WINCY / pTexInfo->tImgInfo.Height * 1.2f, 0.f);
	D3DXMatrixTranslation(&mTrans, 10.f, 10.f, 0.f);
	 
	mWorld = mScale * mTrans;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&mWorld);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CDevice::Get_Instance()->Render_End(m_MapPicControl.m_hWnd);
}

void CMapToolMap::Set_Text_MapInfo(UINT _width, UINT _height)
{
	TCHAR szBuffer[MIN_STR];
	wsprintf(szBuffer, L"%d x %d", _width, _height);

	SetDlgItemText(IDC_STATE_SIZE_JWA, szBuffer);
}


void CMapToolMap::OnBnClickedButtonApply()
{
	m_MapScaleSlider.SetPos(100);
	m_pMyMap->Set_MapScale(1.f);

	m_pMyMap->Init_Tile();
	m_pMyMap->Render();

	CToolView* pToolView = CToolMgr::GetInst()->GetMainFrm()->GetToolView();
	CMiniView* pMiniview = CToolMgr::GetInst()->GetMainFrm()->GetInspectorView()->GetDlgTab3()->GetMiniView();

	// Apply버튼 눌렀을 경우 listbox 선택 해제시킴
	m_MapListBox.SetCurSel(-1);

	pToolView->Invalidate(FALSE);
	pMiniview->Invalidate(FALSE);
}


void CMapToolMap::OnMoveMapScaleSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iBarPos = m_MapScaleSlider.GetPos();
	m_fMapScale = 0.2f + (0.008f * iBarPos);
	m_pMyMap->Set_MapScale(m_fMapScale);

	CDevice::Get_Instance()->Render_Begin();
	m_pMyMap->Render();
	CDevice::Get_Instance()->Render_End();

	CMiniView* pMiniview = CToolMgr::GetInst()->GetMainFrm()->GetInspectorView()->GetDlgTab3()->GetMiniView();
	pMiniview->Invalidate(FALSE);

	*pResult = 0;
}
