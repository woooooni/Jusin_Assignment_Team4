
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "Device.h"
#include "TextureMgr.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND		g_hWnd;

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView() : m_pTerrain(nullptr)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CToolView::~CToolView()
{
}

void CToolView::OnInitialUpdate()
{
	g_hWnd = m_hWnd;

	CScrollView::OnInitialUpdate();

	// SetScrollSizes : ��ũ�� ���� ����� �����ϴ� �Լ�
	// MM_TEXT : �ȼ� ������ �����ϰڴٴ� �ɼ�
	// ��ũ�� ���� ������, ��ũ�� ���� ������

	SetScrollSizes(MM_TEXT, CSize(TILEX * TILECX, TILEY * TILECY / 2));


	//// AfxGetMainWnd : ������ ���� â�� �ּҸ� ��ȯ�ϴ� �Լ�
	//CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	//RECT	rcWnd{};

	//// GetWindowRect : ���� â�� ��Ʈ ������ ������ �Լ�
	//pMainFrm->GetWindowRect(&rcWnd);

	//// SetRect : �Ű� ������� ��Ʈ�� ������ �����ϴ� �Լ�
	//SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	//RECT	rcMainView{};

	//// GetClientRect : ���� viewâ�� RECT ������ ������ �Լ�
	//GetClientRect(&rcMainView);

	//float		fRowFrm = float(rcWnd.right - rcMainView.right);
	//float		fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	////SetWindowPos : �Ű� ������� ���Ӱ� ������ ��ġ�� ũ�⸦ �����ϴ� �Լ�
	// // (��ġ�� �������� Z������ ���� ������, X��ǥ, Y��ǥ, ���� ũ��, ���� ũ��, ��ġ�� �������� ũ��� ��ġ ���� �ɼ�)
	//pMainFrm->SetWindowPos(nullptr,  // ���� ������ ���ϱ� ������ NULL
	//						0, 
	//						0, 
	//						int(WINCX + fRowFrm),
	//						int(WINCY + fColFrm), 
	//						SWP_NOZORDER);	// ���� ������ �����ϰڴٴ� �ɼ�


	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

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


	CDevice::Get_Instance()->Render_End();
}
void CToolView::OnDestroy()
{
	CScrollView::OnDestroy();

	Safe_Delete(m_pTerrain);

	CTextureMgr::Get_Instance()->Destroy_Instance();
	CDevice::Get_Instance()->Destroy_Instance();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
#pragma region �Ⱥ�
BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����


// CToolView �׸���
#pragma endregion �Ⱥ�

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnLButtonDown(nFlags, point);

	// point : ���콺 ��ǥ�� ���� ����.

	m_pTerrain->Tile_Change({ float(point.x + GetScrollPos(0)), float(point.y + GetScrollPos(1)), 0.f }, 0);

	// Invalidate : ȣ�� �� �����쿡 WM_PAINT�� WM_ERASEBKGND �޼����� �߻� ��Ŵ, �̶� OnDraw�Լ��� �ٽ� �ѹ� ȣ��
	// ���ڰ� FALSE : WM_PAINT�� �߻�
	// ���ڰ� TRUE : WM_PAINT�� WM_ERASEBKGND �޼����� �߻�
	
	Invalidate(FALSE);


	// AfxGetMainWnd : ���� ������κ��� WND�� ��ȯ�ϴ� �Լ�
	//CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

	// AfxGetApp : ���� �����带 ���� �ִ� ���� ���� APP�� ��ȯ
	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());

	// GetParentFrame : ���� VIEW�� �ѷ��ΰ� �ִ� ���� FrameWnd�� ��ȯ
	//CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(GetParentFrame());

	CMiniView*		pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	pMiniView->Invalidate(FALSE);


}


void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnMouseMove(nFlags, point);
	
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		//m_pTerrain->Tile_Change({ float(point.x + GetScrollPos(0)), float(point.y + GetScrollPos(1)), 0.f }, 0);
		//Invalidate(FALSE);

		//CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		//CMiniView*		pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));
		//pMiniView->Invalidate(FALSE);
	}

}
