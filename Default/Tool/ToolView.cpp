
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "MiniView.h"
#include "ToolView.h"
#include "Device.h"
#include "TextureMgr.h"
#include "MainFrm.h"
#include "RenderMgr_JWA.h"
#include "InspectorFormView.h"
#include "DlgTab3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND		g_hWnd;

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_BN_CLICKED(IDC_CHECK1_JWA, &CToolView::OnBnClickedIndexCheckBox)
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView() : m_pTerrain(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	CRenderMgr_JWA::Get_Instance()->Set_MapObjInfo(&m_pMapObjInfo);
	
}

CToolView::~CToolView()
{
	for (auto& iter : m_pMapObjInfo)
	{
		for (size_t i = 0; i < iter.second.size(); ++i)
		{
			if (iter.second[i])
			{
				delete iter.second[i];
				iter.second[i] = nullptr;
			}
		}
		iter.second.clear();
	}
	m_pMapObjInfo.clear();

	CRenderMgr_JWA::Get_Instance()->Destroy_Instance();
}

void CToolView::OnInitialUpdate()
{
	g_hWnd = m_hWnd;

	CScrollView::OnInitialUpdate();

	// SetScrollSizes : 스크롤 바의 사이즈를 지정하는 함수
	// MM_TEXT : 픽셀 단위로 조정하겠다는 옵션
	// 스크롤 가로 사이즈, 스크롤 세로 사이즈

	SetScrollSizes(MM_TEXT, CSize(TILEX * TILECX, TILEY * TILECY / 2));


	//// AfxGetMainWnd : 현재의 메인 창의 주소를 반환하는 함수
	//CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	//RECT	rcWnd{};

	//// GetWindowRect : 현재 창의 렉트 정보를 얻어오는 함수
	//pMainFrm->GetWindowRect(&rcWnd);

	//// SetRect : 매개 변수대로 렉트의 정보를 지정하는 함수
	//SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	//RECT	rcMainView{};

	//// GetClientRect : 현재 view창의 RECT 정보를 얻어오는 함수
	//GetClientRect(&rcMainView);

	//float		fRowFrm = float(rcWnd.right - rcMainView.right);
	//float		fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	////SetWindowPos : 매개 변수대로 새롭게 윈도우 위치와 크기를 조정하는 함수
	// // (배치할 윈도우의 Z순서에 대한 포인터, X좌표, Y좌표, 가로 크기, 세로 크기, 배치할 윈도우의 크기및 위치 지정 옵션)
	//pMainFrm->SetWindowPos(nullptr,  // 순서 변경을 안하기 때문에 NULL
	//						0, 
	//						0, 
	//						int(WINCX + fRowFrm),
	//						int(WINCY + fColFrm), 
	//						SWP_NOZORDER);	// 현재 순서를 유지하겠다는 옵션


	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (FAILED(CDevice::Get_Instance()->Initialize()))
	{
		AfxMessageBox(L"Device Create Failed");
		return;
	}

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Cube.png", TEX_SINGLE, L"Cube")))
	{
		AfxMessageBox(L"CubeTexture Create Failed");
		return;
	}

	m_pTerrain = new CTerrain;
	//m_pTileTool = new CDlgTab3;

	if (FAILED(m_pTerrain->Initialize()))
	{
		AfxMessageBox(L"Terrain Init Failed");
		return;
	}

	m_pTerrain->Set_MainView(this);
}

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	CDevice::Get_Instance()->Render_Begin();

	m_pTerrain->Render();
	//m_pTerrain->Index_Render();

	CDevice::Get_Instance()->Render_End();

}
void CToolView::OnDestroy()
{
	CScrollView::OnDestroy();
	Safe_Delete(m_pTerrain);

	CTextureMgr::Get_Instance()->Destroy_Instance();
	CDevice::Get_Instance()->Destroy_Instance();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
#pragma region 안봐
BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


// CToolView 그리기
#pragma endregion 안봐

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnLButtonDown(nFlags, point);

	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	CInspectorFormView*		pInspectorForm = dynamic_cast<CInspectorFormView*>(pMainFrm->m_MainSplitter.GetPane(0, 2));
	CDlgTab3*		pMapTool = pInspectorForm->dlg3;

	m_pTerrain->Tile_Change({ float(point.x + GetScrollPos(0)),
		float(point.y + GetScrollPos(1)), 0.f }, pMapTool->m_iDrawID);

	Invalidate(FALSE);
	pMapTool->Invalidate(FALSE);

	// AfxGetMainWnd : 현재 쓰레드로부터 WND를 반환하는 함수
	// GetParentFrame : 현재 VIEW를 둘러싸고 있는 상위 FrameWnd를 반환
	// AfxGetApp : 메인 쓰레드를 갖고 있는 현재 메인 APP을 반환
}


void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		CInspectorFormView*		pInspectorForm = dynamic_cast<CInspectorFormView*>(pMainFrm->m_MainSplitter.GetPane(0, 2));
		CDlgTab3*		pMapTool = pInspectorForm->dlg3;

		m_pTerrain->Tile_Change({ float(point.x + GetScrollPos(0)),
			float(point.y + GetScrollPos(1)), 0.f }, pMapTool->m_iDrawID);

		Invalidate(FALSE);
		pMapTool->Invalidate(FALSE);
	}
	/*
	if (!m_bTrackMouse)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = -1;

		if (TrackMouseEvent(&tme))
		{
			//m_bTrackMouse = TRUE;
		}
	}
	*/

	CScrollView::OnMouseMove(nFlags, point);

}


void CToolView::UpdateToolView()
{

}

void CToolView::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	//m_pTerrain->Tile_Change({ float(point.x + GetScrollPos(0)), float(point.y + GetScrollPos(1)), 0.f }, 0);
	//Invalidate(FALSE);

	//CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());

	CScrollView::OnMouseHover(nFlags, point);
}


void CToolView::OnMouseLeave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CScrollView::OnMouseLeave();
}


void CToolView::OnBnClickedIndexCheckBox()
{
	/*
	if (m_pTileTool->m_Check.GetCheck() == BST_CHECKED)
	{
		m_pTerrain->Set_IndexTrue();
		m_pTerrain->Index_Render();
		Invalidate(FALSE);
	}

	// 체크 박스가 선택되어 있지 않은 상태라면
	else if (m_pTileTool->m_Check.GetCheck() == BST_UNCHECKED)
	{
		m_pTerrain->Set_IndexFalse();
		m_pTerrain->Index_Render();
		Invalidate(FALSE);
	}
	*/
	//CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	//CToolView*		pMainView = dynamic_cast<CToolView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

	//m_pTerrain->Index_Render();

	//pMainView->Invalidate(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
