#pragma once
#include "afxwin.h"


// CDlgTab2 대화 상자입니다.

class CDlgTab2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTab2)

public:
	CDlgTab2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTab2();


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB2 };
#endif


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.



	afx_msg void OnLbnSelchangePictureList();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	

	afx_msg void OnBnClickedDeleteButton();


	DECLARE_MESSAGE_MAP()


public:

	map<CString, CImage*>		m_mapPngImg;  // 키값이랑 이미지
	
	int							m_iDrawID = 0;


	CListBox m_PictureListBox;  // 애니메이션 리스트 박스에 둘 애들
	CStatic m_AnimPicture;		// 애니메이션 이미지 띄울 애들 

	CRect m_Picture_Rect;		// 이미지를 출력할 렉트 크기  

	
	afx_msg void OnDeltaposSpinAnimPicture(NMHDR *pNMHDR, LRESULT *pResult);


	afx_msg void OnBnClickedButtonPlay();
};
