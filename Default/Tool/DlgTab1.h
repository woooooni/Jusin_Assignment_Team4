#pragma once
#include "afxwin.h"


// CDlgTab1 대화 상자입니다.

class CHierarchyFromView;
class CDlgTab1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTab1)

public:
	CDlgTab1(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTab1();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void			Update_ObjTool();
public:
	CEdit			m_EditObjName;
	CButton			m_BtnObjNameChange;

	afx_msg void	OnBnClickedObjNamechangeBtn();
	CStatic			m_TextObjType;
	CEdit			m_EditObjPosX;
	CEdit			m_EditObjPosY;
	CEdit			m_EditObjAngle;
	CEdit			m_EditObjScaleX;
	CEdit			m_EditObjScaleY;

	afx_msg void	OnEnChangeEditTextPositionx();
	afx_msg void	OnEnChangeEditTextPositionY();
	afx_msg void	OnEnChangeEditTextScaleY();
	afx_msg void	OnEnChangeEditTextScaleX();
};
