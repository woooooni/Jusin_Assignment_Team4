// FileDirectoryView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "FileDirectoryView.h"


// CFileDirectoryView

IMPLEMENT_DYNCREATE(CFileDirectoryView, CFormView)

CFileDirectoryView::CFileDirectoryView()
	: CFormView(IDD_FILEDIRECTORYVIEW)
{

}

CFileDirectoryView::~CFileDirectoryView()
{
}

void CFileDirectoryView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFileDirectoryView, CFormView)
END_MESSAGE_MAP()


// CFileDirectoryView �����Դϴ�.

#ifdef _DEBUG
void CFileDirectoryView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFileDirectoryView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFileDirectoryView �޽��� ó�����Դϴ�.
