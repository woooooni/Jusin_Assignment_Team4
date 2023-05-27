#pragma once
#include "afxwin.h"


// CDlgTab2 ��ȭ �����Դϴ�.

class CDlgTab2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTab2)

public:
	CDlgTab2(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgTab2();


// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB2 };
#endif


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.



	afx_msg void OnLbnSelchangePictureList();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	

	afx_msg void OnBnClickedDeleteButton();


	DECLARE_MESSAGE_MAP()


public:

	map<CString, CImage*>		m_mapPngImg;  // Ű���̶� �̹���
	
	int							m_iDrawID = 0;


	CListBox m_PictureListBox;  // �ִϸ��̼� ����Ʈ �ڽ��� �� �ֵ�
	CStatic m_AnimPicture;		// �ִϸ��̼� �̹��� ��� �ֵ� 

	CRect m_Picture_Rect;		// �̹����� ����� ��Ʈ ũ��  

	
	afx_msg void OnDeltaposSpinAnimPicture(NMHDR *pNMHDR, LRESULT *pResult);


	afx_msg void OnBnClickedButtonPlay();
};
