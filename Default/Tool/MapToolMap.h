#pragma once
#include "afxwin.h"
#include "afxcmn.h"



// CMapToolMap 폼 뷰입니다.

class CMapToolMap : public CFormView
{
	DECLARE_DYNCREATE(CMapToolMap)

public:
	CMapToolMap();           // 동적 만들기에 사용되는 protected 생성자입니다.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	afx_msg void OnSelectMap();
	afx_msg void OnListBox();

public:
	virtual void OnInitialUpdate();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
		DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, 
	CCreateContext* pContext = NULL);
	DECLARE_MESSAGE_MAP()

public:
	int				m_iDrawID = 0;
	CComboBox		m_MapCombo;
	CListBox		m_MapListBox;
	CSliderCtrl		m_MapSacleSlider;
	CStatic			m_MapPicControl;

	map<CString, CImage*>			m_mapPngImg;
	map<CImage*, CImage*>			m_mapPreview;
};


