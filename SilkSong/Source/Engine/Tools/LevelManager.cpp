#include "LevelManager.h"
#include "Core/World.h"

//************************************************
//*********       �û���༭����          **********

/**
 * �Զ��峡����ͼͷ�ļ�
 **/
#include "TearCityLevel.h"
#include "MenuLevel.h"


void LevelManager::Initialize()
{
	/**
	 * ��ӳ���
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
