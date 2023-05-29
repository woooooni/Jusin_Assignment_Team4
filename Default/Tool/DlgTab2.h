#pragma once
#include "afxwin.h"
#include "afxcmn.h"


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

	afx_msg void OnLbnSelchangePictureList();	//  리스트 박스 바꾸기
	afx_msg void OnDropFiles(HDROP hDropInfo); // 드롭 하기
	
	afx_msg void OnBnClickedDeleteButton(); // 지우기 버튼
	afx_msg void OnDeltaposSpinAnimPicture(NMHDR *pNMHDR, LRESULT *pResult); // 스핀 버튼


	afx_msg void OnBnClickedButtonAdd();   // 추가 버튼 -> list anim 에 쓸 


	afx_msg void OnLbnSelchangeAnimlistKjm();	// 애니메이션 리스트
	afx_msg void OnBnClickedButtonPlay();		// 플레이 버튼
	afx_msg void OnTimer(UINT_PTR nIDEvent);	// 타이머 
	afx_msg void OnBnClickedButtonStop();		// 스탑 버튼


	afx_msg void OnBnClickedButton_Delete_AinmList();	// 애니메이션리스트 지우기
	afx_msg void OnBnClickedButton_Init_PictureList();	// 이미지리스트 초기화


	DECLARE_MESSAGE_MAP()


public:

	map<CString, CImage*>		m_mapPngImg;  // 키값이랑 이미지
	int							m_iDrawID = 0;


	CListBox m_PictureListBox;  //  리소스 리스트 박스에 둘 애들
	CStatic m_Picture_Resource;		// 리소스 이미지 띄울 애들 
	CRect m_Picture_Rect;		// 이미지를 출력할 렉트 크기  




	CListBox m_AnimListBox;  // 애니메이션 리스트 박스에 둘 애들 
	CStatic m_AnimPicture;    // 애니메이션 이미지 띄울 애들
	CRect m_AnimPicture_Rect;		// 애니메이션을 출력할 렉트 크기  
	
	int m_nCurrentIndex;

	UINT_PTR m_nTimerID;


	afx_msg void OnBnClickedButton_Init_AnimList_KJM();
};
