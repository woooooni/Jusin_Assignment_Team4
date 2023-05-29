#include "stdafx.h"
#include "FileInfo_KJM.h"


CFileInfo_KJM::CFileInfo_KJM()
{
}


CFileInfo_KJM::~CFileInfo_KJM()
{
}


CString CFileInfo_KJM::ConvertRelativePath(CString strFullPath)
{
	TCHAR		szRelativePath[MAX_PATH] = L""; // ��� ��θ� ������ ���ڹ迭
	TCHAR		szCurDirPath[MAX_PATH] = L""; // ���� ���� ��θ� ������ ���ڹ迭

	GetCurrentDirectory(MAX_PATH, szCurDirPath);

	// szCurDirPath ���� strFullPath.GetString()���� ���� ��� ��θ� ���ؼ� szRelativePath �������ִ� �Լ�(��, ���� ����̺� �������� ���� ����)

	PathRelativePathTo(szRelativePath,
		szCurDirPath,
		FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(),
		FILE_ATTRIBUTE_DIRECTORY
	);

	return CString(szRelativePath);
}
