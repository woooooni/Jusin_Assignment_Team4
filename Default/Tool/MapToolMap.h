#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Terrain.h"

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
	afx_msg void OnListBox(); // ����Ʈ �ڽ����� �� ���ý� -> preview�� �̹��� �ݿ�
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnMoveMapScaleSlider(NMHDR *pNMHDR, LRESULT *pResult);

public:
	float		Get_MapSacle(void) { return m_fMapScale; }
	void		Set_Text_MapInfo(UINT _width, UINT _height);

public:
	virtual void OnInitialUpdate();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
		DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, 
	CCreateContext* pContext = NULL);
	DECLARE_MESSAGE_MAP()

private:
	int			m_iDrawID = 0;
	float		m_fMapScale = 1.f;
	CComboBox						m_MapCombo;
	CListBox						m_MapListBox;
	CSliderCtrl						m_MapScaleSlider; // �� ���� ���� slider
	CStatic							m_MapPicControl; // �� preview pic control
	CButton							m_ButtonApply;

	map<CString, CImage*>			m_mapPngImg;
	CTerrain*						m_pMyMap;
};


