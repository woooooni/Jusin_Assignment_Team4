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
, m_AnimSpeed(0)
, m_CurrentSpeed(0)
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
END_MESSAGE_MAP()


// CDlgTab2 메시지 처리기입니다.

// Initilalize
BOOL CDlgTab2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	::DragAcceptFiles(g_hWnd, TRUE);
	m_PictureListBox.DragAcceptFiles(TRUE);


	ChangeWindowMessageFilterEx(m_PictureListBox.GetSafeHwnd(), WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_PictureListBox.GetSafeHwnd(), WM_COPYDATA, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_PictureListBox.GetSafeHwnd(), 0x0049, MSGFLT_ALLOW, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


// 드롭 파일 
void CDlgTab2::OnDropFiles(HDROP hDropInfo)
{
	
	UpdateData(TRUE);
	//CDialogEx::OnDropFiles(hDropInfo);

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

	CRect rect;//픽쳐 컨트롤의 크기를 저장할 CRect 객체
	m_Picture_Resource.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.
	CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
	dc = m_Picture_Resource.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.
	(*iter->second).StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정
	//m_Picture_Resource.SetBitmap(*(iter->second)); // 애니메이션에 쓸 이미지 출력하고 있음



	// 나의 뻘짓
	//int i = 0;
	//for (; i < strSelectName.GetLength(); ++i)
	//{
	//	// isdigit : 매개 변수로 전달받은 문자가 숫자형태의 글자인지 아니면 글자형태의 글자인지 판별하는 함수
	//	// 숫자형태의 글자로 판별하면 0이 아닌 값을 반환

	//	if (0 != isdigit(strSelectName[i]))
	//		break;
	//}

	//// Delete(index, count) : 인덱스 위치로부터 카운트만큼 문자를 삭제하는 함수
	//strSelectName.Delete(0, i);

	////_tstoi : 문자를 정수형으로 변환하는 함수
	//m_iDrawID = _tstoi(strSelectName);

	//CImage* pImage = iter->second; //  이미지 포인터에 이미지 값을 저장

	//CRect PictureControlRect; // Picture control 렉트 정보를 저장 ->렉트 크기
	//
	//m_Picture_Resource.GetWindowRect(&PictureControlRect);
	//
	//ScreenToClient(&PictureControlRect);

	//CImage resizedImage; // 크기에 맞게 조정하기 위해 저장할 이미지
	//
	//resizedImage.Create(
	//PictureControlRect.Width(),
	//PictureControlRect.Height(),
	//pImage->GetBPP()); // 이미지 정보 출력 

	//CDC* pDC = CDC::FromHandle(resizedImage.GetDC());
	//CDC* pSrcDC = CDC::FromHandle(pImage->GetDC());


	//// 이미지 스트레칭
	//pDC->SetStretchBltMode(HALFTONE);
	//pDC->StretchBlt(0, 0,				// 이미지 출력 위치 
	//	PictureControlRect.Width(),		// 출력할 이미지 너비 
	//	PictureControlRect.Height(),	// 출력할 이미지 높이
	//	pSrcDC,							// 이미지 핸들
	//	0, 0,							// 가져올 이미지의 시작 지점
	//	pImage->GetWidth(),				// 원본 이미지로 부터 잘라낼 너비
	//	pImage->GetHeight(),			// 원본 이미지로 부터 잘라낼 높이 
	//	SRCCOPY);						// 이미지 출력 방법 (복사)

	//resizedImage.ReleaseDC();
	//pImage->ReleaseDC();  // dc정보는 가져왔으면 다시 가져가야 한다 삭제 해야한다 안그럼 오류 난다 
	//pImage->Destroy();
	//pImage->Attach(resizedImage.Detach());
	//
	//// Picture Control에 이미지 그리기
	//
	//CDC* pPictureDC = m_Picture_Resource.GetDC();
	//CRect pictureRect;

	//m_Picture_Resource.GetClientRect(&pictureRect);
	//
	//pPictureDC->FillSolidRect(&pictureRect, RGB(0, 0, 0)); // Picture Control 배경을 흰색으로 채우기
	//pImage->Draw(pPictureDC->m_hDC, pictureRect);
	//m_Picture_Resource.ReleaseDC(pPictureDC);
	//

	UpdateData(FALSE);

}

// 리소스 리스트 지우기 버튼
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



	CRect rect;//픽쳐 컨트롤의 크기를 저장할 CRect 객체
	m_Picture_Resource.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.
	CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
	dc = m_Picture_Resource.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.
	(*iter->second).StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정
																					  //m_Picture_Resource.SetBitmap(*(iter->second)); // 애니메이션에 쓸 이미지 출력하고 있음



	 //CImage* pImage = iter->second; //  이미지 포인터에 이미지 값을 저장
	//CRect PictureControlRect; // Picture control 렉트 정보를 저장 ->렉트 크기
	
	//m_Picture_Resource.GetWindowRect(&PictureControlRect);
	//ScreenToClient(&PictureControlRect);

	//CImage resizedImage; // 크기에 맞게 조정하기 위해 저장할 이미지
	//resizedImage.Create(PictureControlRect.Width(),
	//	PictureControlRect.Height(),
	//	pImage->GetBPP()); // 이미지 정보 출력 


	//CDC* pDC = CDC::FromHandle(resizedImage.GetDC());
	//CDC* pSrcDC = CDC::FromHandle(pImage->GetDC());


	//// 이미지 스트레칭
	//pDC->SetStretchBltMode(HALFTONE);
	//pDC->StretchBlt(0, 0,
	//	PictureControlRect.Width(),
	//	PictureControlRect.Height(),
	//	pSrcDC, 0, 0, pImage->GetWidth(),
	//	pImage->GetHeight(),
	//	SRCCOPY);

	//resizedImage.ReleaseDC();
	//pImage->ReleaseDC();
	//pImage->Destroy();
	//pImage->Attach(resizedImage.Detach());


	//// Picture Control에 이미지 그리기

	//CDC* pPictureDC = m_Picture_Resource.GetDC();
	//CRect pictureRect;

	//m_Picture_Resource.GetClientRect(&pictureRect);

	//pPictureDC->FillSolidRect(&pictureRect, RGB(0, 0, 0)); // Picture Control 배경을 흰색으로 채우기

	//pImage->Draw(pPictureDC->m_hDC, pictureRect);

	//m_Picture_Resource.ReleaseDC(pPictureDC);
	
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

	CListBox* pSrcListBox = (CListBox*)GetDlgItem(IDC_PICTURELIST_KJM);
	CListBox* pDestListBox = (CListBox*)GetDlgItem(IDC_ANIMLIST_KJM);

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



	CRect rect;//픽쳐 컨트롤의 크기를 저장할 CRect 객체
	m_AnimPicture.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.
	CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
	dc = m_AnimPicture.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.
	(*iter->second).StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정
																					  //m_Picture_Resource.SetBitmap(*(iter->second)); // 애니메이션에 쓸 이미지 출력하고 있음



	//m_AnimPicture.SetBitmap(*(iter->second)); // 애니메이션에 쓸 이미지 출력하고 있음

	//int i = 0;

	//for (; i < strSelectName.GetLength(); ++i)
	//{
	//	// isdigit : 매개 변수로 전달받은 문자가 숫자형태의 글자인지 아니면 글자형태의 글자인지 판별하는 함수
	//	// 숫자형태의 글자로 판별하면 0이 아닌 값을 반환

	//	if (0 != isdigit(strSelectName[i]))
	//		break;
	//}

	//// Delete(index, count) : 인덱스 위치로부터 카운트만큼 문자를 삭제하는 함수
	//strSelectName.Delete(0, i);

	////_tstoi : 문자를 정수형으로 변환하는 함수
	//m_iDrawID = _tstoi(strSelectName);

	//CImage* pImage = iter->second; //  이미지 포인터에 이미지 값을 저장

	//CRect PictureControlRect; // Picture control 렉트 정보를 저장 ->렉트 크기
	//m_AnimPicture.GetWindowRect(&PictureControlRect);
	//ScreenToClient(&PictureControlRect);

	//CImage resizedImage; // 크기에 맞게 조정하기 위해 저장할 이미지
	//resizedImage.Create(PictureControlRect.Width(),
	//	PictureControlRect.Height(),
	//	pImage->GetBPP()); // 이미지 정보 출력 

	//CDC* pDC = CDC::FromHandle(resizedImage.GetDC());
	//CDC* pSrcDC = CDC::FromHandle(pImage->GetDC());

	//// 이미지 스트레칭
	//pDC->SetStretchBltMode(HALFTONE);
	//pDC->StretchBlt(0, 0,
	//	PictureControlRect.Width(),
	//	PictureControlRect.Height(),
	//	pSrcDC, 0, 0, pImage->GetWidth(),
	//	pImage->GetHeight(),
	//	SRCCOPY);

	//resizedImage.ReleaseDC();
	//pImage->ReleaseDC();
	//pImage->Destroy();
	//pImage->Attach(resizedImage.Detach());

	//// Picture Control에 이미지 그리기

	//CDC* pPictureDC = m_AnimPicture.GetDC();
	//CRect pictureRect;

	//m_AnimPicture.GetClientRect(&pictureRect);

	//pPictureDC->FillSolidRect(&pictureRect, RGB(0, 0, 0)); // Picture Control 배경을 흰색으로 채우기

	//pImage->Draw(pPictureDC->m_hDC, pictureRect);

	//m_AnimPicture.ReleaseDC(pPictureDC);

	UpdateData(FALSE);

}

// 플레이 버튼 
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

	m_nTimerID = SetTimer(1, (1000/m_AnimSpeed), nullptr); // 애니메이션 스피드로 조절 됨 // 시간 정보 
	


	CRect rect;//픽쳐 컨트롤의 크기를 저장할 CRect 객체
	m_AnimPicture.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.
	CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
	dc = m_AnimPicture.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.
	(*iter->second).StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정
																					  //m_Picture_Resource.SetBitmap(*(iter->second)); // 애니메이션에 쓸 이미지 출력하고 있음



	//나의 뻘짓
	//CImage* pImage = iter->second;

	//CRect AnimPictureRect; //  애니메이션 출력 할  렉트 크기 

	//m_AnimPicture.GetWindowRect(&AnimPictureRect);

	//ScreenToClient(&AnimPictureRect);

	//CImage resizedImage; // 크기에 맞게 조정하기 위해 저장할 이미지

	//resizedImage.Create(
	//	AnimPictureRect.Width(),
	//	AnimPictureRect.Height(),
	//	pImage->GetBPP()); // 이미지 정보 출력 

	//CDC* pDC = CDC::FromHandle(resizedImage.GetDC());
	//CDC* pSrcDC = CDC::FromHandle(pImage->GetDC());


	//// 이미지 스트레칭
	//pDC->SetStretchBltMode(HALFTONE);
	//pDC->StretchBlt(0, 0,				// 이미지 출력 위치 
	//	AnimPictureRect.Width(),		// 출력할 이미지 너비 
	//	AnimPictureRect.Height(),	// 출력할 이미지 높이
	//	pSrcDC,							// 이미지 핸들
	//	0, 0,							// 가져올 이미지의 시작 지점
	//	pImage->GetWidth(),				// 원본 이미지로 부터 잘라낼 너비
	//	pImage->GetHeight(),			// 원본 이미지로 부터 잘라낼 높이 
	//	SRCCOPY);						// 이미지 출력 방법 (복사)

	//resizedImage.ReleaseDC();
	//pImage->ReleaseDC();  // dc정보는 가져왔으면 다시 가져가야 한다 삭제 해야한다 안그럼 오류 난다 
	//pImage->Destroy();
	//pImage->Attach(resizedImage.Detach());

	//// Picture Control에 이미지 그리기

	//CDC* pPictureDC = m_Picture_Resource.GetDC();
	//CRect pictureRect;

	//m_Picture_Resource.GetClientRect(&pictureRect);

	//pPictureDC->FillSolidRect(&pictureRect, RGB(0, 0, 0)); // Picture Control 배경을 흰색으로 채우기
	//pImage->Draw(pPictureDC->m_hDC, pictureRect);
	//m_Picture_Resource.ReleaseDC(pPictureDC);



	// 원래 하던대로 - 첫 출력 이미지 크기가 다름 수정 필요 
	//CString strSelectName;
	//m_AnimListBox.GetText(iSelect, strSelectName);

	//auto iter = m_mapPngImg.find(strSelectName);
	//
	//if (iter == m_mapPngImg.end())
	//	return;

	//m_AnimPicture.SetBitmap(*(iter->second));
	

}

// 타이머 -> 타이머에 따라서 커서 인덱스 이동 
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


	CRect rect;//픽쳐 컨트롤의 크기를 저장할 CRect 객체
	m_AnimPicture.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.
	CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
	dc = m_AnimPicture.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.
	(*iter->second).StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정
																					  //m_Picture_Resource.SetBitmap(*(iter->second)); // 애니메이션에 쓸 이미지 출력하고 있음

	// 나의 뻘짓 
	/*CImage* pImage = iter->second;
	CDC* pPictureDC = m_AnimPicture.GetDC();
	
	CRect pictureRect;
	m_AnimPicture.GetClientRect(&pictureRect);

	pPictureDC->FillSolidRect(&pictureRect, RGB(0, 0, 0));
	pImage->Draw(pPictureDC->m_hDC, pictureRect);

	m_AnimPicture.ReleaseDC(pPictureDC);

	CDialogEx::OnTimer(nIDEvent);*/

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
	m_Picture_Resource.SetBitmap(NULL);

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


// 속도 변경 ok 버튼
void CDlgTab2::OnBnClickedButtonAnimspeedOk() 
{

	UpdateData(TRUE);
	m_AnimSpeed = GetDlgItemInt(IDC_EDIT_ANIMSPEED_KJM);
	
	m_CurrentSpeed = m_AnimSpeed;

	UpdateData(FALSE);
	
}

// 한번에 추가 all 버튼
void CDlgTab2::OnBnClickedButtonAllKjm()
{
	
	CString		strFindName = L"";

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

		// 1. 원본 리스트 박스의 현재 선택된 항목 가져오기
		//int nCurSel = pSrcListBox->GetCurSel();
		//if (nCurSel == LB_ERR)
		//	return; // 선택된 항목이 없으면 종료

		CString strItemText;
		pSrcListBox->GetText(iSelect, strItemText);


		// 2. 가져온 항목을 대상 리스트 박스로 옮기기
		pDestListBox->AddString(strItemText);


		// 3. 원본 리스트 박스에서 선택된 항목 제거
		//pSrcListBox->DeleteString(nCurSel);
		iSelect++;
	}


}
