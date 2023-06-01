
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
#include "InspectorFormView.h"
#include "DlgTab3.h"
#include "MapToolTile.h"
#include "ToolMgr.h"
#include "Obj.h"

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
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView() 
	: m_pTerrain(nullptr)
	, m_eEditMode(EDIT_MODE::EDIT_OBJ)
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CToolView::~CToolView()
{
}

void CToolView::OnInitialUpdate()
{
	g_hWnd = m_hWnd;
	CScrollView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, CSize(TILEX * TILECX, TILEY * TILECY / 2));

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
	if (FAILED(m_pTerrain->Initialize()))
	{
		AfxMessageBox(L"Terrain Init Failed");
		return;
	}
}

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CObj::SetScroll({ float(GetScrollPos(0)), float(GetScrollPos(1)), 0 });
	CDevice::Get_Instance()->Render_Begin();

	m_pTerrain->Render();
	CToolMgr::GetInst()->RenderObj();

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


	if (m_eEditMode == EDIT_MODE::EDIT_TILE)
	{
		CInspectorFormView*	pInspectorForm	= CToolMgr::GetInst()->GetMainFrm()->GetInspectorView();
		CDlgTab3*			pMapTool		= pInspectorForm->dlg3;
		CMapToolTile*		pTileTool		= pMapTool->m_pTileForm;
		CMiniView*			pMiniview		= pMapTool->GetMiniView();

		if (m_bIsSelectTile)
		{
			m_pTerrain->Tile_Change({ float(point.x + GetScrollPos(0)), float(point.y + GetScrollPos(1)), 0.f }, pTileTool->m_iDrawID);
		}

		Invalidate(FALSE);
		pMiniview->Invalidate(FALSE);
	}
	else
	{
		CToolMgr::GetInst()->PickObj({ float(point.x + GetScrollPos(0)), float(point.y + GetScrollPos(1)), 0.f });
	}
}


void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (m_eEditMode == EDIT_MODE::EDIT_TILE)
		{
			CInspectorFormView*	pInspectorForm	= CToolMgr::GetInst()->GetMainFrm()->GetInspectorView();
			CDlgTab3*			pMapTool		= pInspectorForm->dlg3;
			CMapToolTile*		pTileTool		= pMapTool->m_pTileForm;
			CMiniView*			pMiniview		= pMapTool->GetMiniView();


			m_pTerrain->Tile_Change({ float(point.x + GetScrollPos(0)), float(point.y + GetScrollPos(1)), 0.f }, pTileTool->m_iDrawID);

			Invalidate(FALSE);
			pMiniview->Invalidate(FALSE);
		}
		else
		{
			bool bPicked = CToolMgr::GetInst()->PickObj({ float(point.x + GetScrollPos(0)), float(point.y + GetScrollPos(1)), 0.f });
			if (bPicked)
			{
				const D3DXVECTOR3& vMousePos = { float(point.x + GetScrollPos(0)), float(point.y + GetScrollPos(1)), 0.f };
				CToolMgr::GetInst()->GetTargetedObj()->Set_Pos(vMousePos);
				CToolMgr::GetInst()->UpdateAllView();
			}
		}
	}

	CScrollView::OnMouseMove(nFlags, point);
}


void CToolView::UpdateToolView()
{
	OnDraw(nullptr);
}

void CToolView::OnMouseHover(UINT nFlags, CPoint point)
{
	CScrollView::OnMouseHover(nFlags, point);
}


void CToolView::OnMouseLeave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CScrollView::OnMouseLeave();
}



BOOL CToolView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == 'R')
		{
			CObj* pObj = CToolMgr::GetInst()->GetTargetedObj();

			if (pObj == nullptr)
				return TRUE;

			float fAngle = pObj->Get_Angle();
			fAngle += 45.f;

			if (fAngle >= 360.f)
				fAngle = 0.f;
			pObj->Set_Angle(fAngle);

			CToolMgr::GetInst()->UpdateAllView();
			return TRUE;
		}
	}
	return CScrollView::PreTranslateMessage(pMsg);
}
