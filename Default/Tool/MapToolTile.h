#pragma once
#include "afxwin.h"

// CMapToolTile �� ���Դϴ�.

class CMapToolTile : public CFormView
{
	DECLARE_DYNCREATE(CMapToolTile)

public:
	CMapToolTile();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMapToolTile();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL_TILE };
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
	afx_msg void	OnListBox();
	afx_msg void	OnBnClickedIndexCheckBox();
	afx_msg void	OnDropFiles(HDROP hDropInfo);

public:
	virtual void	OnInitialUpdate();
	virtual BOOL	Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
		DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL);
	DECLARE_MESSAGE_MAP()

public:
	CListBox		m_TileListBox;
	CStatic			m_TilePicControl;
	CButton			m_Check;
	map<CString, CImage*>			m_mapPngImg;

	int				m_iDrawID = 0;
};


