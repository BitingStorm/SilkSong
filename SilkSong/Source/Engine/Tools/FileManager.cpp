#include "FileManager.h"


void FileManager::CreateFolder(std::string folderName)
{
    fs::path folderPath = fs::current_path() / folderName;

    if (fs::create_directory(folderPath)) std::cout << "文件夹创建成功: " << folderPath << std::endl;
}
