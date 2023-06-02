// DlgTab2.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "DlgTab2.h"
#include "afxdialogex.h"
#include "FileInfo.h"  //  ���� �� �ٵ�� �� 
#include "ToolMgr.h"
#include "MainFrm.h"
#include "InspectorFormView.h"
#include "Obj.h"
#include "ToolView.h"
#include "TextureMgr.h"

// CDlgTab2 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgTab2, CDialogEx)

CDlgTab2::CDlgTab2(CWnd* pParent /*=NULL*/) : CDialogEx(IDD_DIALOG_TAB2, pParent)
, m_AnimSpeed(0)
, m_CurrentSpeed(0)
, m_StateKey(_T(""))
, m_AnimCount()
{

}

CDlgTab2::~CDlgTab2()
{

}

void CDlgTab2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PICTURELIST_KJM, m_PictureListBox);
	DDX_Control(pDX, IDC_PICTRUESTATIC_KJM, m_Picture_Resource);


	DDX_Control(pDX, IDC_ANIMLIST_KJM, m_AnimListBox);
	DDX_Control(pDX, IDC_ANIMSTATIC_KJM, m_AnimPicture);
	DDX_Text(pDX, IDC_EDIT_ANIMSPEED_KJM, m_AnimSpeed);

	DDX_Text(pDX, IDC_STATIC_SPEEDCHECK_KJM, m_CurrentSpeed);

	DDX_Text(pDX, IDC_STATIC_STATEKEY_VALUE_KJM, m_StateKey);
	DDX_Text(pDX, IDC_STATIC_ANIMCOUNT_VALUE_KJM, m_AnimCount);

	DDX_Control(pDX, IDC_COMBO_STATEKEY_KJM, StateKeyComboBox);
}

BEGIN_MESSAGE_MAP(CDlgTab2, CDialogEx)
	
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_PICTURELIST_KJM, &CDlgTab2::OnLbnSelchangePictureList)
	
	ON_BN_CLICKED(IDC_BUTTON_PICTUREDELETE_KJM, &CDlgTab2::OnBnClickedDeleteButton)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PICTURELIST_KJM, &CDlgTab2::OnDeltaposSpinAnimPicture)

	
	ON_BN_CLICKED(IDC_BUTTON_ADD_KJM, &CDlgTab2::OnBnClickedButtonAdd)


	ON_LBN_SELCHANGE(IDC_ANIMLIST_KJM, &CDlgTab2::OnLbnSelchangeAnimlistKjm)
	ON_BN_CLICKED(IDC_BUTTON_PLAY_KJM, &CDlgTab2::OnBnClickedButtonPlay)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOP_KJM, &CDlgTab2::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_ANIMDELETE_KJM, &CDlgTab2::OnBnClickedButton_Delete_AinmList)
	ON_BN_CLICKED(IDC_BUTTON_INIT_PICTURELIST_KJM, &CDlgTab2::OnBnClickedButton_Init_PictureList)


	ON_BN_CLICKED(IDC_BUTTON_INIT_ANIMLIST_KJM, &CDlgTab2::OnBnClickedButton_Init_AnimList_KJM)


	ON_BN_CLICKED(IDC_BUTTON_ANIMSPEED_OK_KJM, &CDlgTab2::OnBnClickedButtonAnimspeedOk)
	ON_BN_CLICKED(IDC_BUTTON_ALL_KJM, &CDlgTab2::OnBnClickedButtonAllKjm)


	ON_CBN_SELCHANGE(IDC_COMBO_STATEKEY_KJM, &CDlgTab2::OnCbnSelchangeComboStatekeyKjm)





	ON_BN_CLICKED(IDC_BUTTON_APPLY_ANIMINFO_KJM, &CDlgTab2::OnBnClickedButtonApplyAniminfoKjm)
END_MESSAGE_MAP()


// CDlgTab2 �޽��� ó�����Դϴ�.

// Initilalize
BOOL CDlgTab2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	::DragAcceptFiles(g_hWnd, TRUE);
	m_PictureListBox.DragAcceptFiles(TRUE);


	ChangeWindowMessageFilterEx(m_PictureListBox.GetSafeHwnd(), WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_PictureListBox.GetSafeHwnd(), WM_COPYDATA, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_PictureListBox.GetSafeHwnd(), 0x0049, MSGFLT_ALLOW, NULL);


	StateKeyComboBox.InsertString(0, _T("IDLE"));
	StateKeyComboBox.InsertString(0, _T("MOVE"));
	StateKeyComboBox.InsertString(0, _T("ATTACK"));
	StateKeyComboBox.InsertString(0, _T("DEAD"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


// ��� ���� 
void CDlgTab2::OnDropFiles(HDROP hDropInfo)
{
	
	UpdateData(TRUE);

	TCHAR		szFilePath[MAX_PATH] = L"";		// ���� ��� 
	TCHAR		szFileName[MIN_STR] = L"";		// ���� �̸� 

	//DragQueryFile : ��ӵ� ������ ������ ������ �Լ�
	// 0xffffffff(-1)�� �����ϸ� ��ӵ� ������ ������ ��ȯ
	int iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);  //��ӵ� ������ ������ ��� 

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);  
		// ( ��� ��������,  i��° ���ؽ� ,�Լ��� ��ȯ�� �� ������ ������ ���� �̸��� �޴� ���� �ּ�, ������ ũ�� 

		CString strRelative = CFileInfo::ConvertRelativePath(szFilePath); // ������ ���� ��η� ���� 
		CString	strFileName = PathFindFileName(strRelative);  //���� �̸��� ã���� -> ��� �� ���� 
	
		lstrcpy(szFileName, strFileName.GetString()); // ���� ���� 
		PathRemoveExtension(szFileName); //���� �� ���� ( Ȯ���ڸ� ����) 

		PathRemoveFileSpec(szFilePath);
		PathRemoveFileSpec(szFilePath);
		CString strObjkey = PathFindFileName(szFilePath);
	


		strFileName = szFileName;

		auto	iter = m_mapPngImg.find(strFileName);

		if (iter == m_mapPngImg.end())
		{
			CImage*	pPngImg = new CImage;
			pPngImg->Load(strRelative);


			m_vecPicturePath.push_back(strRelative);  // ��� ���� �� �� ��� ���� 
			m_vecPictureObjkey.push_back(strObjkey);

			m_mapPngImg.insert({ strFileName, pPngImg });
			m_PictureListBox.AddString(szFileName);
		}
	}



	//Horizontal_Scroll();  //  ���� ��ũ���� �� ���� ����

	UpdateData(FALSE);

	CDialogEx::OnDropFiles(hDropInfo);
}


// Picture ����Ʈ ���� ���� �� picture box ���
void CDlgTab2::OnLbnSelchangePictureList()   
{
		
	UpdateData(TRUE);

	CString			strSelectName;

	int	iSelect = m_PictureListBox.GetCurSel();

	if (LB_ERR == iSelect)
		return;

	m_PictureListBox.GetText(iSelect, strSelectName);

	auto iter = m_mapPngImg.find(strSelectName);

	if (iter == m_mapPngImg.end())
		return;

	CRect rect;//���� ��Ʈ���� ũ�⸦ ������ CRect ��ü
	m_Picture_Resource.GetWindowRect(rect);//GetWindowRect�� ����ؼ� ���� ��Ʈ���� ũ�⸦ �޴´�.
	CDC* dc; //���� ��Ʈ���� DC�� ������  CDC ������
	dc = m_Picture_Resource.GetDC(); //���� ��Ʈ���� DC�� ��´�.
	(*iter->second).StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//�̹����� ���� ��Ʈ�� ũ��� ����
	
	m_Picture_Resource.ReleaseDC(dc);




	UpdateData(FALSE);

}



// ���ҽ� ����Ʈ ����� ��ư
void CDlgTab2::OnBnClickedDeleteButton()
{

	UpdateData(TRUE);

	CString		strFindName = L"";

	int		iSelect = m_PictureListBox.GetCurSel();

	if (LB_ERR == iSelect)
		return;

	m_PictureListBox.GetText(iSelect, strFindName);

	auto& iter = m_mapPngImg.find(strFindName);

	if (iter == m_mapPngImg.end())
		return;

	CImage* pImage = iter->second; //  �̹��� �����Ϳ� �̹��� ���� ����
	

	Safe_Delete(pImage);


	m_mapPngImg.erase(strFindName);
	m_PictureListBox.DeleteString(iSelect);
	
	m_Picture_Resource.SetBitmap(NULL); 

	UpdateData(FALSE);

}

// ���� ��ư ����� 
void CDlgTab2::OnDeltaposSpinAnimPicture(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	pNMUpDown->iDelta *= 1;

	CString			strSelectName;

	int		iSelect = m_PictureListBox.GetCurSel();

	if (LB_ERR == iSelect)
		return;


	iSelect += pNMUpDown->iDelta;  // ���� ��ư�� ���⿡ ���� �ε��� ����

									 // �ε����� ������ ����� �ʵ��� ó��
	if (iSelect < 0)
	{
		iSelect = 0;
	}
	else if (iSelect >= m_PictureListBox.GetCount())
	{
		iSelect = m_PictureListBox.GetCount() - 1;
	}	 


	m_PictureListBox.SetCurSel(iSelect);

	m_PictureListBox.GetText(iSelect, strSelectName);

	auto iter = m_mapPngImg.find(strSelectName);

	if (iter == m_mapPngImg.end())
		return;


	m_Picture_Resource.SetBitmap(*(iter->second)); // �ִϸ��̼ǿ� �� �̹��� ����ϰ� ����

	int i = 0;

	for (; i < strSelectName.GetLength(); ++i)
	{
		// isdigit : �Ű� ������ ���޹��� ���ڰ� ���������� �������� �ƴϸ� ���������� �������� �Ǻ��ϴ� �Լ�
		// ���������� ���ڷ� �Ǻ��ϸ� 0�� �ƴ� ���� ��ȯ

		if (0 != isdigit(strSelectName[i]))
			break;
	}

	// Delete(index, count) : �ε��� ��ġ�κ��� ī��Ʈ��ŭ ���ڸ� �����ϴ� �Լ�
	strSelectName.Delete(0, i);

	//_tstoi : ���ڸ� ���������� ��ȯ�ϴ� �Լ�
	m_iDrawID = _tstoi(strSelectName);



	CRect rect;//���� ��Ʈ���� ũ�⸦ ������ CRect ��ü
	m_Picture_Resource.GetWindowRect(rect);//GetWindowRect�� ����ؼ� ���� ��Ʈ���� ũ�⸦ �޴´�.
	CDC* dc; //���� ��Ʈ���� DC�� ������  CDC ������
	dc = m_Picture_Resource.GetDC(); //���� ��Ʈ���� DC�� ��´�.
	(*iter->second).StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//�̹����� ���� ��Ʈ�� ũ��� ����
																					 
	*pResult = 0;
}

// �ִϸ��̼� ����Ʈ�� �߰� �ϱ� ��ư 
void CDlgTab2::OnBnClickedButtonAdd()  
{
	UpdateData(TRUE);

	CString		strFindName = L"";
	int		iSelect = m_PictureListBox.GetCurSel();
	if (LB_ERR == iSelect)
		return;

	m_PictureListBox.GetText(iSelect, strFindName);

	auto& iter = m_mapPngImg.find(strFindName);
	if (iter == m_mapPngImg.end())
		return;


	CListBox* pSrcListBox = (CListBox*)GetDlgItem(IDC_PICTURELIST_KJM);
	CListBox* pDestListBox = (CListBox*)GetDlgItem(IDC_ANIMLIST_KJM);

	// 1. ���� ����Ʈ �ڽ��� ���� ���õ� �׸� ��������
	int nCurSel = pSrcListBox->GetCurSel();

	if (nCurSel == LB_ERR)
		return; // ���õ� �׸��� ������ ����

	CString strItemText;
	pSrcListBox->GetText(nCurSel, strItemText);

	// 2. ������ �׸��� ��� ����Ʈ �ڽ��� �ű��
	pDestListBox->AddString(strItemText);

	// 3. ���� ����Ʈ �ڽ����� ���õ� �׸� ����
	//pSrcListBox->DeleteString(nCurSel);


	m_vecAnimPath.push_back(m_vecPicturePath[iSelect]);
	m_vecAnimObjkey.push_back(m_vecPictureObjkey[iSelect]);

	UpdateData(FALSE);
}

// anim ����Ʈ ���� ���� �� anim box ���
void CDlgTab2::OnLbnSelchangeAnimlistKjm()
{
	UpdateData(TRUE);

	CString			strSelectName;

	int	iSelect = m_AnimListBox.GetCurSel();

	if (LB_ERR == iSelect)
		return;

	m_AnimListBox.GetText(iSelect, strSelectName);

	auto iter = m_mapPngImg.find(strSelectName);

	if (iter == m_mapPngImg.end())
		return;

	// ��� 
	CRect rect;//���� ��Ʈ���� ũ�⸦ ������ CRect ��ü
	m_AnimPicture.GetWindowRect(rect);//GetWindowRect�� ����ؼ� ���� ��Ʈ���� ũ�⸦ �޴´�.
	CDC* dc; //���� ��Ʈ���� DC�� ������  CDC ������
	dc = m_AnimPicture.GetDC(); //���� ��Ʈ���� DC�� ��´�.
	(*iter->second).StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//�̹����� ���� ��Ʈ�� ũ��� ����
		

	m_AnimCount = m_AnimListBox.GetCount();


	UpdateData(FALSE);

}

// �÷��� ��ư 
void CDlgTab2::OnBnClickedButtonPlay()
{
	CString			strSelectName;
	
	int iAnimMax = m_AnimListBox.GetCount();
	int iSelect = 0;

	if (LB_ERR == iSelect)
		return;
	
	if (iAnimMax <= 0)
	{
		return;
	}
	if (m_AnimSpeed <= 0)
	{
		m_AnimSpeed = 1;
	}
		
	m_AnimListBox.GetText(iSelect, strSelectName);

	auto iter = m_mapPngImg.find(strSelectName);

	if (iter == m_mapPngImg.end())
		return;

	m_nTimerID = SetTimer(1, (1000/m_AnimSpeed), nullptr); // �ִϸ��̼� ���ǵ�� ���� �� // �ð� ���� 
	
	CRect rect;//���� ��Ʈ���� ũ�⸦ ������ CRect ��ü
	m_AnimPicture.GetWindowRect(rect);//GetWindowRect�� ����ؼ� ���� ��Ʈ���� ũ�⸦ �޴´�.
	CDC* dc; //���� ��Ʈ���� DC�� ������  CDC ������
	dc = m_AnimPicture.GetDC(); //���� ��Ʈ���� DC�� ��´�.
	(*iter->second).StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//�̹����� ���� ��Ʈ�� ũ��� ����
	



}

// Ÿ�̸� -> Ÿ�̸ӿ� ���� Ŀ�� �ε��� �̵� 
void CDlgTab2::OnTimer(UINT_PTR nIDEvent)
{
	
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_ANIMLIST_KJM);

	m_nCurrentIndex++;

	if (m_nCurrentIndex >= pListBox->GetCount())
		m_nCurrentIndex = 0;

	pListBox->SetCurSel(m_nCurrentIndex);

	CString strSelectName;
	pListBox->GetText(m_nCurrentIndex, strSelectName);

	auto iter = m_mapPngImg.find(strSelectName);
	if (iter == m_mapPngImg.end())
		return;

	CObj* pObj = CToolMgr::GetInst()->GetTargetedObj();
	if (pObj != nullptr)
	{
		pObj->Set_AnimIdx(m_nCurrentIndex);
		CToolMgr::GetInst()->UpdateAllView();
	}

	

	// �ִϸ��̼� �信�� ������ ģ��
	CRect rect;//���� ��Ʈ���� ũ�⸦ ������ CRect ��ü
	m_AnimPicture.GetWindowRect(rect);//GetWindowRect�� ����ؼ� ���� ��Ʈ���� ũ�⸦ �޴´�.
	CDC* dc; //���� ��Ʈ���� DC�� ������  CDC ������
	dc = m_AnimPicture.GetDC(); //���� ��Ʈ���� DC�� ��´�.
	(*iter->second).StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//�̹����� ���� ��Ʈ�� ũ��� ����
}
	

// ��ž ��ư -> Ÿ�̸� ���߰� ����
void CDlgTab2::OnBnClickedButtonStop()
{
	KillTimer(m_nTimerID);
	m_nTimerID = 0;
}

// �ִϸ��̼� ����Ʈ ����� ��ư
void CDlgTab2::OnBnClickedButton_Delete_AinmList()
{
	UpdateData(TRUE);

	CString		strFindName = L"";

	int		iSelect = m_AnimListBox.GetCurSel();

	if (LB_ERR == iSelect)
		return;

	m_AnimListBox.GetText(iSelect, strFindName);

	auto& iter = m_mapPngImg.find(strFindName);

	if (iter == m_mapPngImg.end())
		return;

	CImage* pImage = iter->second;

	Safe_Delete(pImage);


	m_mapPngImg.erase(strFindName);

	m_AnimListBox.DeleteString(iSelect);
	
	m_AnimPicture.SetBitmap(NULL);

	UpdateData(FALSE);
}


// �̹��� ����Ʈ �ʱ�ȭ ��ư 
void CDlgTab2::OnBnClickedButton_Init_PictureList()
{
	UpdateData(TRUE);

	m_PictureListBox.ResetContent();
	m_Picture_Resource.SetBitmap(NULL);

	m_vecPicturePath.clear();

	UpdateData(FALSE);
}


// �ִϸ��̼� ����Ʈ �ʱ�ȭ ��ư
void CDlgTab2::OnBnClickedButton_Init_AnimList_KJM()
{
	UpdateData(TRUE);

	m_AnimListBox.ResetContent();
	m_AnimPicture.SetBitmap(NULL);
	m_vecAnimPath.clear();

	KillTimer(m_nTimerID);
	m_nTimerID = 0;
	

	UpdateData(FALSE);	
}


// �ӵ� ���� ok ��ư
void CDlgTab2::OnBnClickedButtonAnimspeedOk() 
{

	UpdateData(TRUE);
	m_AnimSpeed = GetDlgItemInt(IDC_EDIT_ANIMSPEED_KJM);
	
	m_CurrentSpeed = m_AnimSpeed;

	UpdateData(FALSE);
	
}


// �ѹ��� �߰� all ��ư
void CDlgTab2::OnBnClickedButtonAllKjm()
{
	CString		strFindName = L"";

	m_vecAnimPath.clear();

	int		iSelect  = m_PictureListBox.GetCurSel();
	if (iSelect == -1 || iSelect >= 0 )
	{
		iSelect = 0;
	}
	int 	MaxIndex = m_PictureListBox.GetCount();


	for (; iSelect < MaxIndex;)
	{
		m_PictureListBox.GetText(iSelect, strFindName);

		auto& iter = m_mapPngImg.find(strFindName);

		if (iter == m_mapPngImg.end())
			return;

		CImage* pImage = iter->second;

		CListBox* pSrcListBox = (CListBox*)GetDlgItem(IDC_PICTURELIST_KJM);
		CListBox* pDestListBox = (CListBox*)GetDlgItem(IDC_ANIMLIST_KJM);

		// 1. ���� ����Ʈ �ڽ��� ���� ���õ� �׸� ��������
		//int nCurSel = pSrcListBox->GetCurSel();
		//if (nCurSel == LB_ERR)
		//	return; // ���õ� �׸��� ������ ����

		CString strItemText;
		pSrcListBox->GetText(iSelect, strItemText);


		// 2. ������ �׸��� ��� ����Ʈ �ڽ��� �ű��
		pDestListBox->AddString(strItemText);

		m_vecAnimPath.push_back(m_vecPicturePath[iSelect]);
		m_vecAnimObjkey.push_back(m_vecPictureObjkey[iSelect]);
		// 3. ���� ����Ʈ �ڽ����� ���õ� �׸� ����
		//pSrcListBox->DeleteString(nCurSel);
		iSelect++;
	}
	

}

// statekey ���� �޺� �ڽ� 
void CDlgTab2::OnCbnSelchangeComboStatekeyKjm()
{
	UpdateData(TRUE);

	int iSelect = StateKeyComboBox.GetCurSel();

	if (LB_ERR == iSelect)
		return;
	
	CString StateName;

	StateKeyComboBox.GetLBText(iSelect, StateName);
	StateKeyComboBox.SetWindowTextW(StateName);

	m_StateKey = StateName;
	
	CObj* pObj = CToolMgr::GetInst()->GetTargetedObj();
	if (pObj != nullptr)
		pObj->Set_ObjState(wstring(StateName));



	UpdateData(FALSE);
}


// �ִϸ��̼� ���� ��ư
void CDlgTab2::OnBnClickedButtonApplyAniminfoKjm()
{
	CObj* pObj = CToolMgr::GetInst()->GetTargetedObj();
	if (pObj == nullptr)
		return;


	CString str;
	GetDlgItemText(IDC_COMBO_STATEKEY_KJM, str);

	vector<ANIMINFO_KJM>	vecAnim;
	vector<CTexture*>		vecTex;

	int iCount = m_vecAnimPath.size();
	for (UINT i = 0; i < iCount; ++i)
	{
		ANIMINFO_KJM tInfo;

		tInfo.wstrObjKey	= m_vecAnimObjkey.back();
		tInfo.wstrStateKey	= wstring(m_StateKey);
		tInfo.wstrPath = m_vecAnimPath[i];
		tInfo.iAnimSpeed = m_CurrentSpeed;

		vecAnim.push_back(tInfo);

		CTexture* pTex = new CSingleTexture;
		pTex->Insert_Texture(tInfo.wstrPath.c_str());

		vecTex.push_back(pTex);
	}

	pObj->InsertAnimationInfo(wstring(str), vecAnim);
	pObj->InsertAnimTexture(wstring(str), vecTex);

	pObj->Set_ObjState(wstring(m_StateKey));
	pObj->Set_AnimIdx(0);
	CToolMgr::GetInst()->UpdateAllView();
}
