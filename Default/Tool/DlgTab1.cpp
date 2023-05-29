// DlgTab1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "DlgTab1.h"
#include "afxdialogex.h"
#include "ToolObjMgr.h"
#include "Obj.h"
// CDlgTab1 대화 상자입니다.

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
	CObj* pObj = CToolObjMgr::GetInst()->GetTargetedObj();
	if (pObj == nullptr)
	{
		m_EditObjName.SetWindowTextW(L"");
		m_TextObjType.SetWindowTextW(L"");

		m_EditObjPosX.SetWindowTextW(L"");
		m_EditObjPosY.SetWindowTextW(L"");
		m_EditObjAngle.SetWindowTextW(L"");
		m_EditObjScaleX.SetWindowTextW(L"");
		m_EditObjScaleY.SetWindowTextW(L"");
	}
	else
	{
		m_EditObjName.SetWindowTextW(pObj->Get_ObjName().c_str());
		m_TextObjType.SetWindowTextW(STR_OBJID[(UINT)pObj->Get_ObjID()].c_str());

		D3DXVECTOR3 vObjPos = pObj->Get_Info().vPos;
		D3DXVECTOR3 vObjScale = pObj->Get_Info().vSize;

		m_EditObjPosX.SetWindowTextW(to_wstring(vObjPos.x).c_str());
		m_EditObjPosY.SetWindowTextW(to_wstring(vObjPos.y).c_str());
		m_EditObjScaleX.SetWindowTextW(to_wstring(vObjScale.x).c_str());
		m_EditObjScaleY.SetWindowTextW(to_wstring(vObjScale.y).c_str());
	}
}


BEGIN_MESSAGE_MAP(CDlgTab1, CDialogEx)
	ON_BN_CLICKED(IDC_OBJNAMECHANGE_BTN, &CDlgTab1::OnBnClickedObjNamechangeBtn)
	ON_EN_CHANGE(IDC_EDIT_TEXT_POSITIONX, &CDlgTab1::OnEnChangeEditTextPositionx)
	ON_EN_CHANGE(IDC_EDIT_TEXT_POSITIONY, &CDlgTab1::OnEnChangeEditTextPositionY)
	ON_EN_CHANGE(IDC_EDIT_TEXT_SCALEY, &CDlgTab1::OnEnChangeEditTextScaleY)
	ON_EN_CHANGE(IDC__TEXT_SCALEX, &CDlgTab1::OnEnChangeEditTextScaleX)
END_MESSAGE_MAP()


// CDlgTab1 메시지 처리기입니다.


void CDlgTab1::OnBnClickedObjNamechangeBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CObj* pObj = CToolObjMgr::GetInst()->GetTargetedObj();
	if (nullptr == pObj)
		return;

	CString str;
	GetDlgItemText(IDC_OBJNAMECHANGE_EDIT, str);
	pObj->Set_ObjName(str.operator LPCWSTR());

	CToolObjMgr::GetInst()->UpdateAllView();
}


void CDlgTab1::OnEnChangeEditTextPositionx()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlgTab1::OnEnChangeEditTextPositionY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlgTab1::OnEnChangeEditTextScaleY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlgTab1::OnEnChangeEditTextScaleX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
