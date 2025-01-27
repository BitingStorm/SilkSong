/**
 * @file	FileManager.h
 * @brief   �ļ��������ඨ�壬���ڹ�����Ŀ�ļ��ṹ�������ļ��Ĵ���������
 * @author	Arty
 **/

#pragma once
#include <iostream>
#include <filesystem>


namespace fs = std::filesystem;



/*----------------------------------
			  �ļ�������
  ----------------------------------*/
class FileManager
{
public:
    static void CreateFolder(std::string folderPath);

};


