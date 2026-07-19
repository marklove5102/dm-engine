#pragma once
#include <array>

class CFindFile
{
public:
	CFindFile(VOID) {}
	virtual ~CFindFile(VOID) {}
	BOOL StartFind(const char* pszPath, const char* pszFindName, BOOL bFindSubFolder = FALSE, UINT nParam = 0)
	{
		WIN32_FIND_DATA	wfd;
		std::array<CHAR, 1024> szFileName{};
		std::array<CHAR, 32> szExt{};
		std::array<CHAR, 32> szExtTest{};
		_makepath(szFileName.data(), nullptr, pszPath, "*.*", nullptr);
		_splitpath(pszFindName, nullptr, nullptr, nullptr, szExt.data());
		// RAII: FindHandleGuard 析构自动 FindClose, 即使 OnFoundFile/递归 StartFind 中途返回或异常也不泄漏
		FindHandleGuard hFindFile(FindFirstFile(szFileName.data(), &wfd));
		if (hFindFile)
		{
			do {
				if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
				{
					if (wfd.cFileName[0] != '.')
					{
						if (bFindSubFolder)
						{
							_makepath(szFileName.data(), nullptr, pszPath, wfd.cFileName, nullptr);
							StartFind(szFileName.data(), pszFindName, TRUE, nParam);
						}
					}
				}
				else
				{
					_splitpath(wfd.cFileName, nullptr, nullptr, nullptr, szExtTest.data());
					if (_stricmp(szExtTest.data(), szExt.data()) == 0)
					{
						_makepath(szFileName.data(), nullptr, pszPath, wfd.cFileName, nullptr);
						OnFoundFile(szFileName.data(), nParam);
					}
				}
			} while (FindNextFile(hFindFile, &wfd));
		}
		return TRUE;
	}
protected:
	virtual VOID OnFoundFile(const char* pszFilename, UINT nParam = 0) {}
};