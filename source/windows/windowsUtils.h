#pragma once

class WindowsUtils {
public:
	static std::set<std::string> getAllFiles(std::string parentDirectory, std::string fileName = "", bool recursive = false);
};