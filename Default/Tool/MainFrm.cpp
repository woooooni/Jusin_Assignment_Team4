
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "Tool.h"

#include "ToolMgr.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "HierarchyFormView.h"
#include "InspectorFormView.h"
#include "FileDirectoryView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
	: m_pHierarchy(nullptr)
	, m_pInspector(nullptr)
	, m_pToolView(nullptr)
	, m_pFileDirectoryView(nullptr)
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;	

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 메시지 처리기



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// CreateStatic : 분할 창을 만들기 위한 함수(부모 윈도우 주소, 행의 수, 열의 수, 창 스타일, 자식창 id)
	//m_MainSplitter.CreateStatic(this, 1, 2);

	// CreateView : 분할 창에 표시할 view 창을 만드는 함수(배치할 행, 배치할 열, 배치할 view창의 생성 크기, 초기 크기, pContext)

	//m_MainSplitter.CreateView(0, 0, RUNTIME_CLASS(CMyForm), CSize(300, WINCY), pContext);
	//m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CToolView), CSize(WINCX, WINCY), pContext);


	//m_MainSplitter.CreateStatic(this, 2, 2);

	//m_MainSplitter.CreateView(0, 0, RUNTIME_CLASS(CMiniView), CSize(300, 300), pContext);
	//m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CMyForm), CSize(300, 300), pContext);

	//m_MainSplitter.CreateView(1, 0, RUNTIME_CLASS(CMyForm), CSize(300, 300), pContext);
	//m_MainSplitter.CreateView(1, 1, RUNTIME_CLASS(CToolView), CSize(WINCX, WINCY), pContext);

	m_MainSplitter.CreateStatic(this, 1, 3);

	m_SecondSplitter.CreateStatic(&m_MainSplitter, 2, 1,
		WS_CHILD | WS_VISIBLE, // WS_CHILD : 종속적인 자식 창, WS_VISIBLE | 생성 후 바로 화면에 표시하겠다는 옵션
		m_MainSplitter.IdFromRowCol(0, 0)); //: IdFromRowCol(행, 열) - 지정한 행과 열의 창 id값을 반환


	m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CHierarchyFormView), CSize(0, WINCY), pContext);
	m_MainSplitter.CreateView(0, 2, RUNTIME_CLASS(CInspectorFormView), CSize(0, WINCY), pContext);

	
	m_SecondSplitter.CreateView(0, 0, RUNTIME_CLASS(CToolView), CSize(0, 500), pContext);
	m_SecondSplitter.CreateView(1, 0, RUNTIME_CLASS(CFileDirectoryView), CSize(0, 200), pContext);

	m_pHierarchy			= dynamic_cast<CHierarchyFormView*>(m_MainSplitter.GetPane(0, 1));
	m_pInspector			= dynamic_cast<CInspectorFormView*>(m_MainSplitter.GetPane(0, 2));
	m_pToolView				= dynamic_cast<CToolView*>(m_SecondSplitter.GetPane(0, 0));
	m_pFileDirectoryView	= dynamic_cast<CFileDirectoryView*>(m_SecondSplitter.GetPane(1, 0));

	if (m_pHierarchy == nullptr || m_pInspector == nullptr 
	  || m_pToolView == nullptr || m_pFileDirectoryView == nullptr)
	{
		AfxMessageBox(L"초기화 실패");
		return FALSE;
	}
	//SetColumnInfo(열 번호, 열의 크기 지정, 허용 가능한 최소 크기)

	// m_MainSplitter를 3칸으로 나눴고, 각 칸마다 크기를 설정.
	m_MainSplitter.SetColumnInfo(0, 700, 100);
	m_MainSplitter.SetColumnInfo(1, 300, 100);
	m_MainSplitter.SetColumnInfo(2, 700, 100);

	// 크기를 다시 계산해 반영.
	m_MainSplitter.RecalcLayout();
	m_SecondSplitter.RecalcLayout();

	CToolMgr::GetInst()->SetMainFrm(this);

	return TRUE;
}

void CMainFrame::Update_AllView()
{
	m_pHierarchy->UpdateHierarchyView();
	m_pInspector->UpdateInspectorView();
	m_pToolView->UpdateToolView();
}
