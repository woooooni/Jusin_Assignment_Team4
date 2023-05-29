#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::Convert_RelativePath(const CString strPath)
{
	TCHAR szCurrentPath[MAX_STR] = L"";
	TCHAR szRelativePath[MAX_STR] = L"";

	GetCurrentDirectory(MAX_STR, szCurrentPath);
	PathRelativePathTo(szRelativePath, szCurrentPath, FILE_ATTRIBUTE_DIRECTORY,
		strPath, FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}
