#include "SkaarHardcoreMode.h"
#include <Windows.h>
#include <iostream>
#include <shlobj.h>
#include <strsafe.h>

namespace SkaarHardcoreMode {
	void SkaarDeleteAllSaveFiles(StaticFunctionTag *base) {
		_MESSAGE("SkaarDeleteAllSaveFiles() called");

		TCHAR szDir[MAX_PATH];
		WIN32_FIND_DATA ffd;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		DWORD dwError = 0;
		LPCTSTR saveFilePath;

		CHAR my_saves[MAX_PATH];
		HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_saves);

		if (result != S_OK) {
			_MESSAGE("ERROR: Could not get the My Documents directory");
			return;
		}
		else {
			strcat(my_saves, "\\my games\\skyrim\\saves\\");
			_MESSAGE("Skyrim saves located in %s", my_saves);
		}

		// Prepare the directory
		StringCchCopy(szDir, MAX_PATH, my_saves);
		StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

		// Find the first file in the directory.
		hFind = FindFirstFile(szDir, &ffd);
		if (INVALID_HANDLE_VALUE == hFind) {
			_MESSAGE("Couldn't find any save files");
			return;
		}

		// Iterate all the files in the directory
		do {
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				_MESSAGE(TEXT("  %s   <DIR>\n"), ffd.cFileName);
			}
			else {
				TCHAR saveFilePath[MAX_PATH];
				strcpy(saveFilePath, my_saves);
				strcat(saveFilePath, ffd.cFileName);
				_MESSAGE("About to delete %s", saveFilePath);
				DeleteFile(saveFilePath);
			}
		} while (FindNextFile(hFind, &ffd) != 0);

		dwError = GetLastError();
		if (dwError != ERROR_NO_MORE_FILES) {
			_MESSAGE("ERROR reading contents of saved games directory");
			return;
		}

		FindClose(hFind);
	}

	bool RegisterFuncs(VMClassRegistry* registry) {
		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, void>("SkaarDeleteAllSaveFiles", "SkaarHardcoreMode", SkaarHardcoreMode::SkaarDeleteAllSaveFiles, registry));
		return true;
	}
} 
