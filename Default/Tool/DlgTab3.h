#pragma once
#include "afxwin.h"
#include <atlimage.h>

#include "MiniView.h"
#include "MapToolMap.h"
#include "Terrain.h"

// CDlgTab3 대화 상자입니다.

class CDlgTab3 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTab3)

public:
	CDlgTab3(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTab3();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnListBox();
	afx_msg void OnBnClickedInDexCheckBox();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSaveData();

	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio1();

	virtual BOOL OnInitDialog();
	void		 ShowForm(int iIndex);

public:
	CButton			m_RadioTile;
	CButton			m_Check;  // Show Index 체크 박스

	CListBox		m_ListBox;
	CStatic			m_TilePicControl;
	CStatic			m_MiniPicControl;
	int				m_iDrawID = 0;
	map<CString, CImage*>			m_mapPngImg;

	CMiniView*		m_pMini;
	CMapToolMap*	m_pMapForm;

	CTerrain*		pTerrainIndex;
};
