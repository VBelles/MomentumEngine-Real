#include "mcv_platform.h"
#include "windowsUtils.h"
#include <Windows.h>

std::set<std::string> WindowsUtils::getAllFiles(std::string parentDirectory, std::string fileName, bool recursive) {
	std::set<std::string> files;
	WIN32_FIND_DATA file;
	HANDLE search_handle = FindFirstFile((parentDirectory + fileName).c_str(), &file);

	//find files only 
	if (!(search_handle == INVALID_HANDLE_VALUE)) {
		do {
			if (strcmp(file.cFileName, ".") != 0 && strcmp(file.cFileName, "..") != 0) {
				std::string path = parentDirectory + file.cFileName;
				if (!(file.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)) {
					files.insert(path);
				}
			}
		} while (FindNextFile(search_handle, &file));
		FindClose(search_handle);
	}

	//check folders if recursive
	if (recursive) {
		search_handle = FindFirstFile((parentDirectory + "*").c_str(), &file);
		if (!(search_handle == INVALID_HANDLE_VALUE)) {
			do {
				if (strcmp(file.cFileName, ".") != 0 && strcmp(file.cFileName, "..") != 0) {
					std::string path = parentDirectory + file.cFileName;
					if (file.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) {
						path += "/";
						std::set<std::string> recursiveFiles = getAllFiles(path.c_str(), fileName, recursive);
						files.insert(recursiveFiles.begin(), recursiveFiles.end());
					}
				}
			} while (FindNextFile(search_handle, &file));
			FindClose(search_handle);
		}
	}

	return files;
}
