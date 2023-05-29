// DlgTab2.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "DlgTab2.h"
#include "afxdialogex.h"
#include "FileInfo_KJM.h"  //  ���� �� �ٵ�� �� 

// CDlgTab2 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgTab2, CDialogEx)

CDlgTab2::CDlgTab2(CWnd* pParent /*=NULL*/) : CDialogEx(IDD_DIALOG_TAB2, pParent)
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


	ON_BN_CLICKED(IDC_BUTTON1, &CDlgTab2::OnBnClickedButton_Init_AnimList_KJM)
END_MESSAGE_MAP()


// CDlgTab2 �޽��� ó�����Դϴ�.




// ��� ���� 
void CDlgTab2::OnDropFiles(HDROP hDropInfo)
{
	
	UpdateData(TRUE);
	CDialog::OnDropFiles(hDropInfo);

	TCHAR		szFilePath[MAX_PATH] = L"";
	TCHAR		szFileName[MIN_STR] = L"";

	//DragQueryFile : ��ӵ� ������ ������ ������ �Լ�
	// 0xffffffff(-1)�� �����ϸ� ��ӵ� ������ ������ ��ȯ
	int iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		CString strRelative = CFileInfo_KJM::ConvertRelativePath(szFilePath);
		CString	strFileName = PathFindFileName(strRelative);

		lstrcpy(szFileName, strFileName.GetString());
		PathRemoveExtension(szFileName);

		strFileName = szFileName;

		auto	iter = m_mapPngImg.find(strFileName);

		if (iter == m_mapPngImg.end())
		{
			CImage*	pPngImg = new CImage;
			pPngImg->Load(strRelative);

			m_mapPngImg.insert({ strFileName, pPngImg });
			m_PictureListBox.AddString(szFileName);
		}
	}

	//Horizontal_Scroll();  //  ���� ��ũ���� �� ���� ����

	UpdateData(FALSE);

	CDialogEx::OnDropFiles(hDropInfo);


}


// Picture ����Ʈ ���� ���� �� ���
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

	CImage* pImage = iter->second; //  �̹��� �����Ϳ� �̹��� ���� ����

	CRect PictureControlRect; // Picture control ��Ʈ ������ ���� ->��Ʈ ũ��
	m_Picture_Resource.GetWindowRect(&PictureControlRect);
	ScreenToClient(&PictureControlRect);

	CImage resizedImage; // ũ�⿡ �°� �����ϱ� ���� ������ �̹���
	
	resizedImage.Create(
	PictureControlRect.Width(),
	PictureControlRect.Height(),
	pImage->GetBPP()); // �̹��� ���� ��� 

	CDC* pDC = CDC::FromHandle(resizedImage.GetDC());
	CDC* pSrcDC = CDC::FromHandle(pImage->GetDC());


	// �̹��� ��Ʈ��Ī
	pDC->SetStretchBltMode(HALFTONE);
	pDC->StretchBlt(0, 0,				// �̹��� ��� ��ġ 
		PictureControlRect.Width(),		// ����� �̹��� �ʺ� 
		PictureControlRect.Height(),	// ����� �̹��� ����
		pSrcDC,							// �̹��� �ڵ�
		0, 0,							// ������ �̹����� ���� ����
		pImage->GetWidth(),				// ���� �̹����� ���� �߶� �ʺ�
		pImage->GetHeight(),			// ���� �̹����� ���� �߶� ���� 
		SRCCOPY);						// �̹��� ��� ��� (����)

	resizedImage.ReleaseDC();
	pImage->ReleaseDC();  // dc������ ���������� �ٽ� �������� �Ѵ� ���� �ؾ��Ѵ� �ȱ׷� ���� ���� 
	
	pImage->Destroy();
	pImage->Attach(resizedImage.Detach());
	
	// Picture Control�� �̹��� �׸���
	
	CDC* pPictureDC = m_Picture_Resource.GetDC();
	CRect pictureRect;

	m_Picture_Resource.GetClientRect(&pictureRect);
	
	pPictureDC->FillSolidRect(&pictureRect, RGB(255, 255, 255)); // Picture Control ����� ������� ä���
	pImage->Draw(pPictureDC->m_hDC, pictureRect);
	m_Picture_Resource.ReleaseDC(pPictureDC);
	
	UpdateData(FALSE);

}

// ���ҽ� ����Ʈ ����� ��ư
void CDlgTab2::OnBnClickedDeleteButton()
{
	// TODO: Add your control notification handler code here

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


	CImage* pImage = iter->second; //  �̹��� �����Ϳ� �̹��� ���� ����

	CRect PictureControlRect; // Picture control ��Ʈ ������ ���� ->��Ʈ ũ��
	
	m_Picture_Resource.GetWindowRect(&PictureControlRect);
	ScreenToClient(&PictureControlRect);

	CImage resizedImage; // ũ�⿡ �°� �����ϱ� ���� ������ �̹���
	resizedImage.Create(PictureControlRect.Width(),
		PictureControlRect.Height(),
		pImage->GetBPP()); // �̹��� ���� ��� 


	CDC* pDC = CDC::FromHandle(resizedImage.GetDC());
	CDC* pSrcDC = CDC::FromHandle(pImage->GetDC());


	// �̹��� ��Ʈ��Ī
	pDC->SetStretchBltMode(HALFTONE);
	pDC->StretchBlt(0, 0,
		PictureControlRect.Width(),
		PictureControlRect.Height(),
		pSrcDC, 0, 0, pImage->GetWidth(),
		pImage->GetHeight(),
		SRCCOPY);

	resizedImage.ReleaseDC();
	pImage->ReleaseDC();
	//pImage->Destroy();
	//pImage->Attach(resizedImage.Detach());


	// Picture Control�� �̹��� �׸���

	CDC* pPictureDC = m_Picture_Resource.GetDC();
	CRect pictureRect;

	m_Picture_Resource.GetClientRect(&pictureRect);

	pPictureDC->FillSolidRect(&pictureRect, RGB(255, 255, 255)); // Picture Control ����� ������� ä���

	pImage->Draw(pPictureDC->m_hDC, pictureRect);

	m_Picture_Resource.ReleaseDC(pPictureDC);
	
	*pResult = 0;
}

// �ִϸ��̼� ����Ʈ�� �߰� �ϱ� ��ư 
void CDlgTab2::OnBnClickedButtonAdd()  
{

	CString		strFindName = L"";
	int		iSelect = m_PictureListBox.GetCurSel();
	if (LB_ERR == iSelect)
		return;

	m_PictureListBox.GetText(iSelect, strFindName);

	auto& iter = m_mapPngImg.find(strFindName);
	if (iter == m_mapPngImg.end())
		return;

	CImage* pImage = iter->second;

	CListBox* pSrcListBox = (CListBox*)GetDlgItem(IDC_PictureLIST_KJM);
	CListBox* pDestListBox = (CListBox*)GetDlgItem(IDC_AnimLIST_KJM);

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

}

// anim ����Ʈ ���� ���� �� ���
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

	m_AnimPicture.SetBitmap(*(iter->second)); // �ִϸ��̼ǿ� �� �̹��� ����ϰ� ����

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

	CImage* pImage = iter->second; //  �̹��� �����Ϳ� �̹��� ���� ����

	CRect PictureControlRect; // Picture control ��Ʈ ������ ���� ->��Ʈ ũ��
	m_AnimPicture.GetWindowRect(&PictureControlRect);
	ScreenToClient(&PictureControlRect);

	CImage resizedImage; // ũ�⿡ �°� �����ϱ� ���� ������ �̹���
	resizedImage.Create(PictureControlRect.Width(),
		PictureControlRect.Height(),
		pImage->GetBPP()); // �̹��� ���� ��� 

	CDC* pDC = CDC::FromHandle(resizedImage.GetDC());
	CDC* pSrcDC = CDC::FromHandle(pImage->GetDC());

	// �̹��� ��Ʈ��Ī
	pDC->SetStretchBltMode(HALFTONE);
	pDC->StretchBlt(0, 0,
		PictureControlRect.Width(),
		PictureControlRect.Height(),
		pSrcDC, 0, 0, pImage->GetWidth(),
		pImage->GetHeight(),
		SRCCOPY);

	resizedImage.ReleaseDC();
	pImage->ReleaseDC();
	pImage->Destroy();
	pImage->Attach(resizedImage.Detach());

	// Picture Control�� �̹��� �׸���

	CDC* pPictureDC = m_AnimPicture.GetDC();
	CRect pictureRect;

	m_AnimPicture.GetClientRect(&pictureRect);

	pPictureDC->FillSolidRect(&pictureRect, RGB(255, 255, 255)); // Picture Control ����� ������� ä���

	pImage->Draw(pPictureDC->m_hDC, pictureRect);

	m_AnimPicture.ReleaseDC(pPictureDC);

	UpdateData(FALSE);

}

// �÷��� ��ư 
void CDlgTab2::OnBnClickedButtonPlay()
{

	int iAnimMax = m_AnimListBox.GetCount();
	int iSelect = 0;

	if (LB_ERR == iSelect)
		return;
	
	if (iAnimMax <= 0)
	{
		return;
	}

	m_nTimerID = SetTimer(1, 100, nullptr); // �ð�
	
	CString strSelectName;
	m_AnimListBox.GetText(iSelect, strSelectName);

	auto iter = m_mapPngImg.find(strSelectName);
	
	if (iter == m_mapPngImg.end())
		return;

	m_AnimPicture.SetBitmap(*(iter->second));

}

// Ÿ�̸� -> Ÿ�̸ӿ� ���� Ŀ�� �ε��� �̵� 
void CDlgTab2::OnTimer(UINT_PTR nIDEvent)
{
	
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_AnimLIST_KJM);

	m_nCurrentIndex++;

	if (m_nCurrentIndex >= pListBox->GetCount())
		m_nCurrentIndex = 0;

	pListBox->SetCurSel(m_nCurrentIndex);

	CString strSelectName;
	pListBox->GetText(m_nCurrentIndex, strSelectName);

	auto iter = m_mapPngImg.find(strSelectName);
	if (iter == m_mapPngImg.end())
		return;

	CImage* pImage = iter->second;
	CDC* pPictureDC = m_AnimPicture.GetDC();
	
	CRect pictureRect;
	m_AnimPicture.GetClientRect(&pictureRect);

	pPictureDC->FillSolidRect(&pictureRect, RGB(255, 255, 255));
	pImage->Draw(pPictureDC->m_hDC, pictureRect);

	m_AnimPicture.ReleaseDC(pPictureDC);

	CDialogEx::OnTimer(nIDEvent);
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
	

	UpdateData(FALSE);
}


// �ִϸ��̼� ����Ʈ �ʱ�ȭ ��ư
void CDlgTab2::OnBnClickedButton_Init_AnimList_KJM()
{
	UpdateData(TRUE);

	m_AnimListBox.ResetContent();
	KillTimer(m_nTimerID);
	m_nTimerID = 0;
	m_AnimPicture.SetBitmap(NULL);

	UpdateData(FALSE);	
}
