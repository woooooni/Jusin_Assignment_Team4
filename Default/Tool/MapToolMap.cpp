// MapToolMap.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapToolMap.h"


// CMapToolMap

IMPLEMENT_DYNCREATE(CMapToolMap, CFormView)

CMapToolMap::CMapToolMap()
	: CFormView(IDD_MAPTOOL_MAP)
{

}

CMapToolMap::~CMapToolMap()
{
}

void CMapToolMap::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

void CMapToolMap::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
}

BOOL CMapToolMap::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT nID, CCreateContext * pContext)
{
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

BEGIN_MESSAGE_MAP(CMapToolMap, CFormView)
END_MESSAGE_MAP()


// CMapToolMap �����Դϴ�.

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


// CMapToolMap �޽��� ó�����Դϴ�.
