#pragma once
#include "afxwin.h"
#include <atlimage.h>

#include "MiniView.h"
#include "MapToolMap.h"
#include "MapToolTile.h"
#include "Terrain.h"

// CDlgTab3 ��ȭ �����Դϴ�.

class CDlgTab3 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTab3)

public:
	CDlgTab3(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgTab3();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnListBox();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSaveData();
	afx_msg void OnLoadData();

	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio1();

	virtual BOOL OnInitDialog();

public:
	CMiniView*	GetMiniView()	{ return m_pMini; }
	void		 ShowForm(int iIndex);

public:
	CButton			m_RadioTile;

	CStatic			m_MiniPicControl;
	int				m_iDrawID = 0;
	map<CString, CImage*>			m_mapPngImg;

	CMiniView*		m_pMini;
	CMapToolMap*	m_pMapForm;
	CMapToolTile*	m_pTileForm;
};
