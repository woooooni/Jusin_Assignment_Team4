#pragma once



// CFileDirectoryView �� ���Դϴ�.

class CFileDirectoryView : public CFormView
{
	DECLARE_DYNCREATE(CFileDirectoryView)

protected:
	CFileDirectoryView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CFileDirectoryView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILEDIRECTORYVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};


