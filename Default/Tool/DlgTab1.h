#pragma once
#include "afxwin.h"


// CDlgTab1 ��ȭ �����Դϴ�.

class CHierarchyFromView;
class CDlgTab1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTab1)

public:
	CDlgTab1(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgTab1();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
