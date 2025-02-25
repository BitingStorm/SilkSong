/**
 * @file	FileManager.h
 * @brief   文件管理器类定义，用于构建项目文件结构，管理文件的创建、销毁、读写
 * @author	Arty
 **/

#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;



/*----------------------------------
			  文件管理器
  ----------------------------------*/
class FileManager
{
public:
    static void CreateFolder(std::string folderPath);

	static bool ReadFile(std::string fileName, std::string& line);

	static bool WriteFile(std::string fileName, std::string text);
};