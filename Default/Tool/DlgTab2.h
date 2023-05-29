#pragma once
#include "afxwin.h"
#include "afxcmn.h"


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

	afx_msg void OnLbnSelchangePictureList();	//  ����Ʈ �ڽ� �ٲٱ�
	afx_msg void OnDropFiles(HDROP hDropInfo); // ��� �ϱ�
	
	afx_msg void OnBnClickedDeleteButton(); // ����� ��ư
	afx_msg void OnDeltaposSpinAnimPicture(NMHDR *pNMHDR, LRESULT *pResult); // ���� ��ư


	afx_msg void OnBnClickedButtonAdd();   // �߰� ��ư -> list anim �� �� 


	afx_msg void OnLbnSelchangeAnimlistKjm();	// �ִϸ��̼� ����Ʈ
	afx_msg void OnBnClickedButtonPlay();		// �÷��� ��ư
	afx_msg void OnTimer(UINT_PTR nIDEvent);	// Ÿ�̸� 
	afx_msg void OnBnClickedButtonStop();		// ��ž ��ư


	afx_msg void OnBnClickedButton_Delete_AinmList();	// �ִϸ��̼Ǹ���Ʈ �����
	afx_msg void OnBnClickedButton_Init_PictureList();	// �̹�������Ʈ �ʱ�ȭ


	DECLARE_MESSAGE_MAP()


public:

	map<CString, CImage*>		m_mapPngImg;  // Ű���̶� �̹���
	int							m_iDrawID = 0;


	CListBox m_PictureListBox;  //  ���ҽ� ����Ʈ �ڽ��� �� �ֵ�
	CStatic m_Picture_Resource;		// ���ҽ� �̹��� ��� �ֵ� 
	CRect m_Picture_Rect;		// �̹����� ����� ��Ʈ ũ��  




	CListBox m_AnimListBox;  // �ִϸ��̼� ����Ʈ �ڽ��� �� �ֵ� 
	CStatic m_AnimPicture;    // �ִϸ��̼� �̹��� ��� �ֵ�
	CRect m_AnimPicture_Rect;		// �ִϸ��̼��� ����� ��Ʈ ũ��  
	
	int m_nCurrentIndex;

	UINT_PTR m_nTimerID;


	afx_msg void OnBnClickedButton_Init_AnimList_KJM();
};
