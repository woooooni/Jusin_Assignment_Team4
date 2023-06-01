// DlgTab1.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "DlgTab1.h"
#include "afxdialogex.h"
#include "ToolMgr.h"
#include "Obj.h"
// CDlgTab1 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgTab1, CDialogEx)

CDlgTab1::CDlgTab1(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_TAB1, pParent)
{

}

CDlgTab1::~CDlgTab1()
{
}

void CDlgTab1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJNAMECHANGE_EDIT, m_EditObjName);
	DDX_Control(pDX, IDC_OBJNAMECHANGE_BTN, m_BtnObjNameChange);
	DDX_Control(pDX, IDC_OBJTYPE_TEXT_TAG, m_TextObjType);
	DDX_Control(pDX, IDC_EDIT_TEXT_POSITIONX, m_EditObjPosX);
	DDX_Control(pDX, IDC_EDIT_TEXT_POSITIONY, m_EditObjPosY);
	DDX_Control(pDX, IDC_EDIT_TEXT_ANGLE, m_EditObjAngle);
	DDX_Control(pDX, IDC__TEXT_SCALEX, m_EditObjScaleX);
	DDX_Control(pDX, IDC_EDIT_TEXT_SCALEY, m_EditObjScaleY);
}

void CDlgTab1::Update_ObjTool()
{
	UpdateData(TRUE);
	CObj* pObj = CToolMgr::GetInst()->GetTargetedObj();
	if (pObj == nullptr)
	{
		SetDlgItemText(m_EditObjName.GetDlgCtrlID(), L"");
		SetDlgItemText(m_TextObjType.GetDlgCtrlID(), L"");

		SetDlgItemText(m_EditObjPosX.GetDlgCtrlID(), L"");
		SetDlgItemText(m_EditObjPosY.GetDlgCtrlID(), L"");
		SetDlgItemText(m_EditObjAngle.GetDlgCtrlID(), L"");
		SetDlgItemText(m_EditObjScaleX.GetDlgCtrlID(), L"");
		SetDlgItemText(m_EditObjScaleY.GetDlgCtrlID(), L"");
	}
	else
	{
		D3DXVECTOR3 vObjPos = pObj->Get_Info().vPos;
		D3DXVECTOR3 vObjScale = pObj->Get_Info().vScale;

		SetDlgItemText(m_EditObjName.GetDlgCtrlID(), pObj->Get_ObjName().c_str());
		SetDlgItemText(m_TextObjType.GetDlgCtrlID(), STR_OBJID[(UINT)pObj->Get_ObjID()].c_str());

		SetDlgItemText(m_EditObjPosX.GetDlgCtrlID(), to_wstring(vObjPos.x).c_str());
		SetDlgItemText(m_EditObjPosY.GetDlgCtrlID(), to_wstring(vObjPos.y).c_str());
		SetDlgItemText(m_EditObjAngle.GetDlgCtrlID(), to_wstring(pObj->Get_Angle()).c_str());
		SetDlgItemText(m_EditObjScaleX.GetDlgCtrlID(), to_wstring(vObjScale.x).c_str());
		SetDlgItemText(m_EditObjScaleY.GetDlgCtrlID(), to_wstring(vObjScale.y).c_str());
	}
	UpdateData(FALSE);
}


BEGIN_MESSAGE_MAP(CDlgTab1, CDialogEx)
	ON_BN_CLICKED(IDC_OBJNAMECHANGE_BTN, &CDlgTab1::OnBnClickedObjNamechangeBtn)
END_MESSAGE_MAP()


// CDlgTab1 �޽��� ó�����Դϴ�.


void CDlgTab1::OnBnClickedObjNamechangeBtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CObj* pObj = CToolMgr::GetInst()->GetTargetedObj();
	if (nullptr == pObj)
		return;

	CString str;
	GetDlgItemText(IDC_OBJNAMECHANGE_EDIT, str);
	pObj->Set_ObjName(str.operator LPCWSTR());

	CToolMgr::GetInst()->UpdateAllView();
}


BOOL CDlgTab1::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			// ESC Ű �̺�Ʈ�� ���� ó�� �߰�
			return TRUE;
		}
		else if (pMsg->wParam == VK_RETURN)
		{
			CObj* pObj = CToolMgr::GetInst()->GetTargetedObj();
			if (nullptr == pObj)
				return FALSE;

			CString str;

			D3DXVECTOR3 vPos = pObj->Get_Info().vPos;
			D3DXVECTOR3 vScale = pObj->Get_Info().vScale;

			m_EditObjPosX.GetWindowTextW(str);
			vPos.x = _tstof(str);

			m_EditObjPosY.GetWindowTextW(str);
			vPos.y = _tstof(str);

			m_EditObjAngle.GetWindowTextW(str);
			pObj->Set_Angle(_tstof(str));

			m_EditObjScaleX.GetWindowTextW(str);
			vScale.x = _tstof(str);

			m_EditObjScaleY.GetWindowTextW(str);
			vScale.y = _tstof(str);

			pObj->Set_Pos(vPos);
			pObj->Set_Scale(vScale);

			CToolMgr::GetInst()->UpdateAllView();
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
