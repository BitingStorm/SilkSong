/**
 * @file	ResourceManager.h
 * @brief   资源管理器定义，用于访问并管理全局游戏图像、媒体资源
 * @author	Arty
 **/

#pragma once
#include <unordered_map>
#include <vector>
#include "Core/Struct.h"



class IMAGE;

/* 动画资源类 */
namespace ArtyEngine
{
	struct FAnimationResource
	{
		IMAGE** images;
		int32 num;
	};
}

/*----------------------------------
			 资源管理器
  ----------------------------------*/
class ResourceManager
{
public:
	void Initialize();

	IMAGE* Fetch(std::string name);
	ArtyEngine::FAnimationResource FetchAnimation(std::string name);
    std::wstring FetchVideo(std::string name);

private:
	std::unordered_map<std::string, IMAGE*>imagePool;
	std::unordered_map<std::string, ArtyEngine::FAnimationResource>animationPool;
	std::unordered_map<std::string, std::string>audioPool;
	std::unordered_map<std::string, std::wstring>videoPool;

	/**
	 * @brief 加载单张图像资源
	 * @param[in] name			    指定资源名称
	 * @param[in] path              资源路径
	 * @param[in] wid			    图像宽度
	 * @param[in] hei	            图像高度 
	 **/
	void Load(std::string name, std::string path, int wid, int hei);

	/**
	 * @brief 加载多张动画图像资源
	 * @param[in] name			    指定资源名称
	 * @param[in] path              资源路径
	 * @param[in] wid			    图像宽度
	 * @param[in] hei	            图像高度
	 * @param[in] hei	            动画帧数量
	 **/
	void Load(std::string name, std::string path, int wid, int hei, int num);

	/**
	 * @brief 加载多张动画图像资源
	 * @param[in] name			    指定资源名称
	 * @param[in] path              资源路径
	 * @param[in] arr			    传入一个Pair数组，用于指定每一帧动画的宽度与高度
	 **/
	void Load(std::string name, std::string path, const std::vector<FIntVector2>& arr);

	/**
	 * @brief 加载SpriteSheet动画图像资源
	 * @param[in] name			    指定资源名称
	 * @param[in] path              资源路径
	 * @param[in] wid			    SpriteSheet宽度
	 * @param[in] hei	            SpriteSheet高度
	 * @param[in] row	            SpriteSheet行数
	 * @param[in] col	            SpriteSheet列数
	 **/
	void Load(std::string name, std::string path, int wid, int hei, int num, int row, int col);

	//加载音频媒体资源
	void Load(std::string name, std::string path);

	//加载视频资源
	void Load(std::string name, std::wstring pszFileName);

	//加载字体资源
	void LoadText(std::string path);

	//加载鼠标样式资源
	void LoadCustomCursor(std::string path);
};