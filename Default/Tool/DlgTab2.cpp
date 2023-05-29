// DlgTab2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "DlgTab2.h"
#include "afxdialogex.h"
#include "FileInfo_KJM.h"  //  파일 명 다듬는 용 

// CDlgTab2 대화 상자입니다.

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


// CDlgTab2 메시지 처리기입니다.




// 드롭 파일 
void CDlgTab2::OnDropFiles(HDROP hDropInfo)
{
	
	UpdateData(TRUE);
	CDialog::OnDropFiles(hDropInfo);

	TCHAR		szFilePath[MAX_PATH] = L"";
	TCHAR		szFileName[MIN_STR] = L"";

	//DragQueryFile : 드롭된 파일을 정보를 얻어오는 함수
	// 0xffffffff(-1)로 지정하면 드롭된 파일의 개수를 반환
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

	//Horizontal_Scroll();  //  가로 스크롤은 안 쓸거 같음

	UpdateData(FALSE);

	CDialogEx::OnDropFiles(hDropInfo);


}


// Picture 리스트 내용 갱신 및 출력
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

	m_Picture_Resource.SetBitmap(*(iter->second)); // 애니메이션에 쓸 이미지 출력하고 있음

	int i = 0;

	for (; i < strSelectName.GetLength(); ++i)
	{
		// isdigit : 매개 변수로 전달받은 문자가 숫자형태의 글자인지 아니면 글자형태의 글자인지 판별하는 함수
		// 숫자형태의 글자로 판별하면 0이 아닌 값을 반환

		if (0 != isdigit(strSelectName[i]))
			break;
	}

	// Delete(index, count) : 인덱스 위치로부터 카운트만큼 문자를 삭제하는 함수
	strSelectName.Delete(0, i);

	//_tstoi : 문자를 정수형으로 변환하는 함수
	m_iDrawID = _tstoi(strSelectName);

	CImage* pImage = iter->second; //  이미지 포인터에 이미지 값을 저장

	CRect PictureControlRect; // Picture control 렉트 정보를 저장 ->렉트 크기
	m_Picture_Resource.GetWindowRect(&PictureControlRect);
	ScreenToClient(&PictureControlRect);

	CImage resizedImage; // 크기에 맞게 조정하기 위해 저장할 이미지
	
	resizedImage.Create(
	PictureControlRect.Width(),
	PictureControlRect.Height(),
	pImage->GetBPP()); // 이미지 정보 출력 

	CDC* pDC = CDC::FromHandle(resizedImage.GetDC());
	CDC* pSrcDC = CDC::FromHandle(pImage->GetDC());


	// 이미지 스트레칭
	pDC->SetStretchBltMode(HALFTONE);
	pDC->StretchBlt(0, 0,				// 이미지 출력 위치 
		PictureControlRect.Width(),		// 출력할 이미지 너비 
		PictureControlRect.Height(),	// 출력할 이미지 높이
		pSrcDC,							// 이미지 핸들
		0, 0,							// 가져올 이미지의 시작 지점
		pImage->GetWidth(),				// 원본 이미지로 부터 잘라낼 너비
		pImage->GetHeight(),			// 원본 이미지로 부터 잘라낼 높이 
		SRCCOPY);						// 이미지 출력 방법 (복사)

	resizedImage.ReleaseDC();
	pImage->ReleaseDC();  // dc정보는 가져왔으면 다시 가져가야 한다 삭제 해야한다 안그럼 오류 난다 
	
	pImage->Destroy();
	pImage->Attach(resizedImage.Detach());
	
	// Picture Control에 이미지 그리기
	
	CDC* pPictureDC = m_Picture_Resource.GetDC();
	CRect pictureRect;

	m_Picture_Resource.GetClientRect(&pictureRect);
	
	pPictureDC->FillSolidRect(&pictureRect, RGB(255, 255, 255)); // Picture Control 배경을 흰색으로 채우기
	pImage->Draw(pPictureDC->m_hDC, pictureRect);
	m_Picture_Resource.ReleaseDC(pPictureDC);
	
	UpdateData(FALSE);

}

// 리소스 리스트 지우기 버튼
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

	CImage* pImage = iter->second; //  이미지 포인터에 이미지 값을 저장
	

	Safe_Delete(pImage);


	m_mapPngImg.erase(strFindName);
	m_PictureListBox.DeleteString(iSelect);
	
	m_Picture_Resource.SetBitmap(NULL); 

	UpdateData(FALSE);

}

// 스핀 버튼 만들기 
void CDlgTab2::OnDeltaposSpinAnimPicture(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	pNMUpDown->iDelta *= 1;

	CString			strSelectName;

	int		iSelect = m_PictureListBox.GetCurSel();

	if (LB_ERR == iSelect)
		return;


	iSelect += pNMUpDown->iDelta;  // 스핀 버튼의 방향에 따라 인덱스 변경

									 // 인덱스가 범위를 벗어나지 않도록 처리
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


	m_Picture_Resource.SetBitmap(*(iter->second)); // 애니메이션에 쓸 이미지 출력하고 있음

	int i = 0;

	for (; i < strSelectName.GetLength(); ++i)
	{
		// isdigit : 매개 변수로 전달받은 문자가 숫자형태의 글자인지 아니면 글자형태의 글자인지 판별하는 함수
		// 숫자형태의 글자로 판별하면 0이 아닌 값을 반환

		if (0 != isdigit(strSelectName[i]))
			break;
	}

	// Delete(index, count) : 인덱스 위치로부터 카운트만큼 문자를 삭제하는 함수
	strSelectName.Delete(0, i);

	//_tstoi : 문자를 정수형으로 변환하는 함수
	m_iDrawID = _tstoi(strSelectName);


	CImage* pImage = iter->second; //  이미지 포인터에 이미지 값을 저장

	CRect PictureControlRect; // Picture control 렉트 정보를 저장 ->렉트 크기
	
	m_Picture_Resource.GetWindowRect(&PictureControlRect);
	ScreenToClient(&PictureControlRect);

	CImage resizedImage; // 크기에 맞게 조정하기 위해 저장할 이미지
	resizedImage.Create(PictureControlRect.Width(),
		PictureControlRect.Height(),
		pImage->GetBPP()); // 이미지 정보 출력 


	CDC* pDC = CDC::FromHandle(resizedImage.GetDC());
	CDC* pSrcDC = CDC::FromHandle(pImage->GetDC());


	// 이미지 스트레칭
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


	// Picture Control에 이미지 그리기

	CDC* pPictureDC = m_Picture_Resource.GetDC();
	CRect pictureRect;

	m_Picture_Resource.GetClientRect(&pictureRect);

	pPictureDC->FillSolidRect(&pictureRect, RGB(255, 255, 255)); // Picture Control 배경을 흰색으로 채우기

	pImage->Draw(pPictureDC->m_hDC, pictureRect);

	m_Picture_Resource.ReleaseDC(pPictureDC);
	
	*pResult = 0;
}

// 애니메이션 리스트로 추가 하기 버튼 
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

	// 1. 원본 리스트 박스의 현재 선택된 항목 가져오기
	int nCurSel = pSrcListBox->GetCurSel();
	if (nCurSel == LB_ERR)
		return; // 선택된 항목이 없으면 종료

	CString strItemText;
	pSrcListBox->GetText(nCurSel, strItemText);

	// 2. 가져온 항목을 대상 리스트 박스로 옮기기
	pDestListBox->AddString(strItemText);

	// 3. 원본 리스트 박스에서 선택된 항목 제거
	//pSrcListBox->DeleteString(nCurSel);

}

// anim 리스트 내용 갱신 및 출력
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

	m_AnimPicture.SetBitmap(*(iter->second)); // 애니메이션에 쓸 이미지 출력하고 있음

	int i = 0;

	for (; i < strSelectName.GetLength(); ++i)
	{
		// isdigit : 매개 변수로 전달받은 문자가 숫자형태의 글자인지 아니면 글자형태의 글자인지 판별하는 함수
		// 숫자형태의 글자로 판별하면 0이 아닌 값을 반환

		if (0 != isdigit(strSelectName[i]))
			break;
	}

	// Delete(index, count) : 인덱스 위치로부터 카운트만큼 문자를 삭제하는 함수
	strSelectName.Delete(0, i);

	//_tstoi : 문자를 정수형으로 변환하는 함수
	m_iDrawID = _tstoi(strSelectName);

	CImage* pImage = iter->second; //  이미지 포인터에 이미지 값을 저장

	CRect PictureControlRect; // Picture control 렉트 정보를 저장 ->렉트 크기
	m_AnimPicture.GetWindowRect(&PictureControlRect);
	ScreenToClient(&PictureControlRect);

	CImage resizedImage; // 크기에 맞게 조정하기 위해 저장할 이미지
	resizedImage.Create(PictureControlRect.Width(),
		PictureControlRect.Height(),
		pImage->GetBPP()); // 이미지 정보 출력 

	CDC* pDC = CDC::FromHandle(resizedImage.GetDC());
	CDC* pSrcDC = CDC::FromHandle(pImage->GetDC());

	// 이미지 스트레칭
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

	// Picture Control에 이미지 그리기

	CDC* pPictureDC = m_AnimPicture.GetDC();
	CRect pictureRect;

	m_AnimPicture.GetClientRect(&pictureRect);

	pPictureDC->FillSolidRect(&pictureRect, RGB(255, 255, 255)); // Picture Control 배경을 흰색으로 채우기

	pImage->Draw(pPictureDC->m_hDC, pictureRect);

	m_AnimPicture.ReleaseDC(pPictureDC);

	UpdateData(FALSE);

}

// 플레이 버튼 
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

	m_nTimerID = SetTimer(1, 100, nullptr); // 시간
	
	CString strSelectName;
	m_AnimListBox.GetText(iSelect, strSelectName);

	auto iter = m_mapPngImg.find(strSelectName);
	
	if (iter == m_mapPngImg.end())
		return;

	m_AnimPicture.SetBitmap(*(iter->second));

}

// 타이머 -> 타이머에 따라서 커서 인덱스 이동 
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
	

// 스탑 버튼 -> 타이머 멈추게 만듬
void CDlgTab2::OnBnClickedButtonStop()
{
	KillTimer(m_nTimerID);
	m_nTimerID = 0;
}

// 애니메이션 리스트 지우기 버튼
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


// 이미지 리스트 초기화 버튼 
void CDlgTab2::OnBnClickedButton_Init_PictureList()
{
	UpdateData(TRUE);

	m_PictureListBox.ResetContent();
	

	UpdateData(FALSE);
}


// 애니메이션 리스트 초기화 버튼
void CDlgTab2::OnBnClickedButton_Init_AnimList_KJM()
{
	UpdateData(TRUE);

	m_AnimListBox.ResetContent();
	KillTimer(m_nTimerID);
	m_nTimerID = 0;
	m_AnimPicture.SetBitmap(NULL);

	UpdateData(FALSE);	
}
