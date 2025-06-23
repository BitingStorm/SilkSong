#include "LevelManager.h"
#include "Core/World.h"

//************************************************
//*********       用户需编辑部分          **********

/**
 * 自定义场景地图头文件
 **/
#include "TearCityLevel.h"
#include "MenuLevel.h"
#include "StartLevel.h"
#include "RuinHouseLevel.h"
#include "BossLevel.h"
#include "GrimmLevel.h"


void LevelManager::Initialize()
{
	/**
	 * 添加场景
	 **/
	AddLevel<MenuLevel>("Menu");
	AddLevel<StartLevel>("Start");
	AddLevel<TearCityLevel>("TearCity");
	AddLevel<RuinHouseLevel>("RuinHouse");
	AddLevel<BossLevel>("BossHouse");
	AddLevel<GrimmLevel>("GrimmTent");
	SetDefaultLevel("Menu");

//*********                             **********
//************************************************


	mainWorld.currentLevel = level_to_create();
	mainWorld.currentLevel->BeginPlay();
}

void LevelManager::SetDefaultLevel(std::string levelName)
{
	level_to_delete = mainWorld.currentLevel;
	if (levelMap.find(levelName) == levelMap.end())return;
	level_to_create = levelMap[levelName];
}

void LevelManager::RefreshLevel()
{
	if (level_to_delete)
	{
		level_to_delete->OnLevelDelete();
		mainWorld.WipeData();
		delete level_to_delete;
		level_to_delete = nullptr;
		mainWorld.currentLevel = level_to_create();
		mainWorld.currentLevel->OnLevelLoad();
		mainWorld.currentLevel->BeginPlay();
	}
}
