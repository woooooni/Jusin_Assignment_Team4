#pragma once


// CDlgTab4 ��ȭ �����Դϴ�.

class CDlgTab4 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTab4)

public:
	CDlgTab4(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgTab4();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
