#include "FileManager.h"


void FileManager::CreateFolder(std::string folderName)
{
    fs::path folderPath = fs::current_path() / folderName;

    if (fs::create_directory(folderPath)) std::cout << "�ļ��д����ɹ�: " << folderPath << std::endl;
}
