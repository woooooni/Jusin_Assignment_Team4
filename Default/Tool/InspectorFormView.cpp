// InspectorFormView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "InspectorFormView.h"
#include "Functor.h"


// CInspectorFormView

IMPLEMENT_DYNCREATE(CInspectorFormView, CFormView)

CInspectorFormView::CInspectorFormView()
	: CFormView(IDD_INSPECTORFORMVIEW)
{

}

CInspectorFormView::~CInspectorFormView()
{
	Safe_Delete(dlg1);
	Safe_Delete(dlg2);
	Safe_Delete(dlg3);
	//Safe_Delete(dlg4);
}

void CInspectorFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INSPECTOR_TAB, m_tab);
}

BEGIN_MESSAGE_MAP(CInspectorFormView, CFormView)
	
	ON_NOTIFY(TCN_SELCHANGE, IDC_INSPECTOR_TAB, &CInspectorFormView::OnSelchangeInspectorTab)
END_MESSAGE_MAP()


// CInspectorFormView 진단입니다.

#ifdef _DEBUG
void CInspectorFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CInspectorFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CInspectorFormView 메시지 처리기입니다.



void CInspectorFormView::OnSelchangeInspectorTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (IDC_INSPECTOR_TAB == pNMHDR->idFrom)
	{
		int iSelect = m_tab.GetCurSel();

		switch (iSelect)
		{
		case 0:
			dlg1->ShowWindow(SW_SHOW);
			dlg2->ShowWindow(SW_HIDE);
			dlg3->ShowWindow(SW_HIDE);
			break;
		case 1:
			dlg1->ShowWindow(SW_HIDE);
			dlg2->ShowWindow(SW_SHOW);
			dlg3->ShowWindow(SW_HIDE);
			break;
		case 2:
			dlg1->ShowWindow(SW_HIDE);
			dlg2->ShowWindow(SW_HIDE);
			dlg3->ShowWindow(SW_SHOW);
			break;
		}
	}

	*pResult = 0;
}


void CInspectorFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CRect rect;
	m_tab.GetWindowRect(rect);

	m_tab.InsertItem(0, L"오브젝트 툴");
	m_tab.InsertItem(1, L"애니메이션 툴");
	m_tab.InsertItem(2, L"맵 툴");

	m_tab.SetCurSel(0);

	dlg1 = new CDlgTab1;
	dlg1->Create(IDD_DIALOG_TAB1, &m_tab);
	dlg1->MoveWindow(0, 20, rect.Width(), rect.Height());
	dlg1->ShowWindow(SW_SHOW);

	dlg2 = new CDlgTab2;
	dlg2->Create(IDD_DIALOG_TAB2, &m_tab);
	dlg2->MoveWindow(0, 20, rect.Width(), rect.Height());
	dlg2->ShowWindow(SW_HIDE);

	dlg3 = new CDlgTab3;
	dlg3->Create(IDD_DIALOG_TAB3, &m_tab);
	dlg3->MoveWindow(0, 20, rect.Width(), rect.Height());
	dlg3->ShowWindow(SW_HIDE);
}
