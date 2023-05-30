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
	TCHAR		szRelativePath[MAX_PATH] = L""; // ��� ��θ� ������ ���ڹ迭
	TCHAR		szCurDirPath[MAX_PATH] = L""; // ���� ���� ��θ� ������ ���ڹ迭

	GetCurrentDirectory(MAX_PATH, szCurDirPath);

	// szCurDirPath ���� strFullPath.GetString()���� ���� ��� ��θ� ���ؼ� szRelativePath �������ִ� �Լ�(��, ���� ����̺� �������� ���� ����)

	PathRelativePathTo(szRelativePath,
		szCurDirPath,
		FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(),
		FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

void CFileInfo::DirInfoExtraction(const wstring & wstrPath, list<IMGPATH*>& rPathList)
{

	//CFileFind : mfc���� �����ϴ� ���� �� ��� ���� ���� Ŭ����
	CFileFind			Find;

	wstring		wstrFilePath = wstrPath + L"\\*.*";

	// FindFile : �־��� ��ο� ������ ������ �Ǵ��ϴ� �Լ�, ������ ������ FALSE�� ����
	// ���� �ľ� ��, ���� �� ���Ͽ� �����ϰ� ���� ��쿣 �ݵ�� FindNextfile �Լ��� ������ ȣ���ؾ� �Ѵ�.

	BOOL		bContinue = Find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		// FindNextFile : ���� ��� �ȿ��� ���� ������ ã�� �Լ�, ���̻� ã���� ���ų� ã�� ������ ���� �� ������ �����̸� 0�� ��ȯ

		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		else if (Find.IsDirectory())
		{
			//GetFilePath : ���� ã�� ��θ� ������ �Լ�
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathList);
		}
		else
		{
			if (Find.IsSystem())
				continue;


			IMGPATH*	pImgPath = new IMGPATH;
			TCHAR		szPathBuf[MAX_STR] = L"";

			lstrcpy(szPathBuf, Find.GetFilePath().GetString());
			//szPathBuf = 0x001de0e4 L"D:\\����ȯ\\137��\\Frame137\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack0.png"
			PathRemoveFileSpec(szPathBuf);
			//szPathBuf = 0x001de0e4 L"D:\\����ȯ\\137��\\Frame137\\Texture\\Stage\\Effect\\BossMultiAttack"

			pImgPath->iCount = CFileInfo::DirFileCnt(szPathBuf);

			// GetFileTitle : ���� �̸��� ������ �Լ�
			wstring		wstrTextureName = Find.GetFileTitle().GetString();
			//wstrTextureName = L"BossMultiAttack0"

			// substr(����, ��) : ���۰� ���� �ش��ϴ� ���ڿ��� ����
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";
			// wstrTextureName = L"BossMultiAttack%d.png"

			// PathCombine(�����, ���� ���ڿ�(src1), ������ ���ڿ�(src2)) // src1�� src2���̿� '\\'�ڵ����� ���Ե� ä ������� ����
			PathCombine(szPathBuf, szPathBuf, wstrTextureName.c_str());
			//szPathBuf = 0x001de0e4 L"D:\\����ȯ\\137��\\Frame137\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"

			pImgPath->wstrPath = ConvertRelativePath(szPathBuf);
			// pImgPath->wstrPath = L"..\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"

			PathRemoveFileSpec(szPathBuf);
			// pImgPath->wstrPath = L"..\\Texture\\Stage\\Effect\\BossMultiAttack\\

			// PathFindFileName : ���� ���� ã�ų� ���� ������ �������� ã�Ƴ��� �Լ�
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

	// FindFile : �־��� ��ο� ������ ������ �Ǵ��ϴ� �Լ�, ������ ������ FALSE�� ����
	// ���� �ľ� ��, ���� �� ���Ͽ� �����ϰ� ���� ��쿣 �ݵ�� FindNextfile �Լ��� ������ ȣ���ؾ� �Ѵ�.

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
