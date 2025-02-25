#include "FileManager.h"



void FileManager::CreateFolder(std::string folderName)
{
    fs::path folderPath = fs::current_path() / folderName;

    if (fs::create_directory(folderPath)) std::cout << "文件夹创建成功: " << folderPath << std::endl;
}

bool FileManager::ReadFile(std::string fileName, std::string& line)
{
    std::ifstream inFile;
    
    inFile.open(fileName);

    if (inFile.is_open()) 
    {
        while (getline(inFile, line)) 
        {
            std::cout << line << std::endl;
        }
        inFile.close();
        return true;
    }
    else 
    {
        std::cerr << "无法打开文件进行读取。" << std::endl;
        return false;
    }
}

bool FileManager::WriteFile(std::string fileName, std::string text)
{
    std::ofstream outFile;

    outFile.open(fileName);

    if (outFile.is_open())
    {
        outFile << text;
        outFile.close();
        std::cout << "文件已成功写入。" << std::endl;
        return true;
    }
    else 
    {
        std::cerr << "无法打开文件进行写操作。" << std::endl;
        return false;
    }
}