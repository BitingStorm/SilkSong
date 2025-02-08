#include "LevelManager.h"
#include "Core/World.h"

//************************************************
//*********       用户需编辑部分          **********

/**
 * 自定义场景地图头文件
 **/
#include "TearCityLevel.h"
#include "MenuLevel.h"


void LevelManager::Initialize()
{
	/**
	 * 添加场景
	 **/
	AddLevel<MenuLevel>("Menu");
	AddLevel<TearCityLevel>("TearCity");
	SetDefaultLevel("Menu");
}

//*********                             **********
//************************************************

void LevelManager::SetDefaultLevel(std::string levelName)
{
	level_to_delete = mainWorld.currentLevel;
	if (levelMap.find(levelName) == levelMap.end())return;
	mainWorld.currentLevel = levelMap[levelName];
}

void LevelManager::RefreshLevel()
{
	if (level_to_delete)
	{
		mainWorld.WipeData();
		level_to_delete = nullptr;
		mainWorld.currentLevel->BeginPlay();
	}
}
