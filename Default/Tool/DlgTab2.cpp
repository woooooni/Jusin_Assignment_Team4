// DlgTab2.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "DlgTab2.h"
#include "afxdialogex.h"
#include "FileInfo_KJM.h"  //  ���� �� �ٵ�� �� 

// CDlgTab2 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgTab2, CDialogEx)

CDlgTab2::CDlgTab2(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_TAB2, pParent)
{

}

CDlgTab2::~CDlgTab2()
{
}

void CDlgTab2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST1, Picture_Control);
	DDX_Control(pDX, IDC_LIST1, m_PictureListBox);
	DDX_Control(pDX, IDC_PictureStatic_KJM, m_AnimPicture);
}


BEGIN_MESSAGE_MAP(CDlgTab2, CDialogEx)
	//ON_BN_CLICKED(IDC_BUTTON1, &CDlgTab2::OnBnClickedPrint)
	//ON_LBN_SELCHANGE(IDC_LIST1, &CDlgTab2::OnLbnSelchangeList)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CDlgTab2::OnLbnSelchangePictureList)
	
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgTab2::OnBnClickedDeleteButton)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDlgTab2::OnDeltaposSpinAnimPicture)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgTab2::OnBnClickedButtonPlay)
END_MESSAGE_MAP()


// CDlgTab2 �޽��� ó�����Դϴ�.



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


void CDlgTab2::OnLbnSelchangePictureList()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);


	CString			strSelectName;

	int	iSelect = m_PictureListBox.GetCurSel();

	if (LB_ERR == iSelect)
		return;

	m_PictureListBox.GetText(iSelect, strSelectName);

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
	
//	pImage->Destroy();
//	pImage->Attach(resizedImage.Detach());



	// Picture Control�� �̹��� �׸���
	
	CDC* pPictureDC = m_AnimPicture.GetDC();
	CRect pictureRect;
	
	m_AnimPicture.GetClientRect(&pictureRect);
	
	pPictureDC->FillSolidRect(&pictureRect, RGB(255, 255, 255)); // Picture Control ����� ������� ä���
	
	pImage->Draw(pPictureDC->m_hDC, pictureRect);

	m_AnimPicture.ReleaseDC(pPictureDC);



	UpdateData(FALSE);

}





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

	CImage* pImage = iter->second;
	

	//Safe_Delete(pImage);


	m_mapPngImg.erase(strFindName);

	m_PictureListBox.DeleteString(iSelect);

	

	UpdateData(FALSE);

}



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

	//	pImage->Destroy();
	//	pImage->Attach(resizedImage.Detach());



	// Picture Control�� �̹��� �׸���

	CDC* pPictureDC = m_AnimPicture.GetDC();
	CRect pictureRect;

	m_AnimPicture.GetClientRect(&pictureRect);

	pPictureDC->FillSolidRect(&pictureRect, RGB(255, 255, 255)); // Picture Control ����� ������� ä���

	pImage->Draw(pPictureDC->m_hDC, pictureRect);

	m_AnimPicture.ReleaseDC(pPictureDC);






	// TODO: Add your control notification handler code here
	

	*pResult = 0;
}


void CDlgTab2::OnBnClickedButtonPlay()
{
	// TODO: Add your control notification handler code here


	CString			strSelectName;
	
	int iPlay = 0;
		
	m_PictureListBox.SetCurSel(iPlay);

	m_PictureListBox.GetText(iPlay, strSelectName);

	auto iter = m_mapPngImg.find(strSelectName);



	m_AnimPicture.SetBitmap(*(iter->second)); // �ִϸ��̼ǿ� �� �̹��� ����ϰ� ����

	int i = 0;

	for (; i < strSelectName.GetLength(); ++i)
	{
		if (0 != isdigit(strSelectName[i]))
			break;
	}
	strSelectName.Delete(0, i);
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

	//	pImage->Destroy();
	//	pImage->Attach(resizedImage.Detach());



	// Picture Control�� �̹��� �׸���
	
	CDC* pPictureDC = m_AnimPicture.GetDC();
	CRect pictureRect;

	m_AnimPicture.GetClientRect(&pictureRect);

	pPictureDC->FillSolidRect(&pictureRect, RGB(255, 255, 255)); // Picture Control ����� ������� ä���

	pImage->Draw(pPictureDC->m_hDC, pictureRect);

	m_AnimPicture.ReleaseDC(pPictureDC);

}
