#pragma once



// CMapToolMap �� ���Դϴ�.

class CMapToolMap : public CFormView
{
	DECLARE_DYNCREATE(CMapToolMap)

public:
	CMapToolMap();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMapToolMap();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL_MAP };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

public:
	virtual void OnInitialUpdate();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
		DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, 
	CCreateContext* pContext = NULL);
	DECLARE_MESSAGE_MAP()
};


