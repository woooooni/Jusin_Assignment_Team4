#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(CString strFullPath)
{
	TCHAR		szRelativePath[MAX_PATH] = L""; // 상대 경로를 저장할 문자배열
	TCHAR		szCurDirPath[MAX_PATH] = L""; // 현재 절대 경로를 저장할 문자배열

	GetCurrentDirectory(MAX_PATH, szCurDirPath);

	// szCurDirPath 에서 strFullPath.GetString()으로 가는 상대 경로를 구해서 szRelativePath 저장해주는 함수(단, 같은 드라이브 내에서만 변경 가능)

	PathRelativePathTo(szRelativePath,
		szCurDirPath,
		FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(),
		FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

void CFileInfo::DirInfoExtraction(const wstring & wstrPath, list<IMGPATH*>& rPathList)
{

	//CFileFind : mfc에서 제공하는 파일 및 경로 제어 관련 클래스
	CFileFind			Find;

	wstring		wstrFilePath = wstrPath + L"\\*.*";

	// FindFile : 주어진 경로에 파일의 유무를 판단하는 함수, 파일이 없으면 FALSE를 리턴
	// 유무 파악 후, 실제 그 파일에 접근하고 싶은 경우엔 반드시 FindNextfile 함수를 무조건 호출해야 한다.

	BOOL		bContinue = Find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		// FindNextFile : 동일 경로 안에서 다음 파일을 찾는 함수, 더이상 찾을게 없거나 찾은 파일이 폴더 중 마지막 파일이면 0을 반환

		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		else if (Find.IsDirectory())
		{
			//GetFilePath : 현재 찾은 경로를 얻어오는 함수
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathList);
		}
		else
		{
			if (Find.IsSystem())
				continue;


			IMGPATH*	pImgPath = new IMGPATH;
			TCHAR		szPathBuf[MAX_STR] = L"";

			lstrcpy(szPathBuf, Find.GetFilePath().GetString());
			//szPathBuf = 0x001de0e4 L"D:\\유준환\\137기\\Frame137\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack0.png"
			PathRemoveFileSpec(szPathBuf);
			//szPathBuf = 0x001de0e4 L"D:\\유준환\\137기\\Frame137\\Texture\\Stage\\Effect\\BossMultiAttack"

			pImgPath->iCount = CFileInfo::DirFileCnt(szPathBuf);

			// GetFileTitle : 파일 이름만 얻어오는 함수
			wstring		wstrTextureName = Find.GetFileTitle().GetString();
			//wstrTextureName = L"BossMultiAttack0"

			// substr(시작, 끝) : 시작과 끝에 해당하는 문자열을 얻어옴
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";
			// wstrTextureName = L"BossMultiAttack%d.png"

			// PathCombine(결과물, 원본 문자열(src1), 결합할 문자열(src2)) // src1과 src2사이에 '\\'자동으로 삽입된 채 결과물에 저장
			PathCombine(szPathBuf, szPathBuf, wstrTextureName.c_str());
			//szPathBuf = 0x001de0e4 L"D:\\유준환\\137기\\Frame137\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"

			pImgPath->wstrPath = ConvertRelativePath(szPathBuf);
			// pImgPath->wstrPath = L"..\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"

			PathRemoveFileSpec(szPathBuf);
			// pImgPath->wstrPath = L"..\\Texture\\Stage\\Effect\\BossMultiAttack\\

			// PathFindFileName : 파일 명을 찾거나 가장 마지막 폴더명을 찾아내는 함수
			pImgPath->wstrStateKey = PathFindFileName(szPathBuf);

			PathRemoveFileSpec(szPathBuf);
			//pImgPath->wstrPath = L"..\\Texture\\Stage\\Effect\\

			pImgPath->wstrObjKey = PathFindFileName(szPathBuf);

			rPathList.push_back(pImgPath);
			bContinue = FALSE;
		}
	}
}

int CFileInfo::DirFileCnt(const wstring & wstrPath)
{
	CFileFind			Find;

	wstring		wstrFilePath = wstrPath + L"\\*.*";

	// FindFile : 주어진 경로에 파일의 유무를 판단하는 함수, 파일이 없으면 FALSE를 리턴
	// 유무 파악 후, 실제 그 파일에 접근하고 싶은 경우엔 반드시 FindNextfile 함수를 무조건 호출해야 한다.

	BOOL		bContinue = Find.FindFile(wstrFilePath.c_str());

	int iFileCnt = 0;

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		if (Find.IsSystem())
			continue;

		++iFileCnt;

	}

	return iFileCnt;
}
