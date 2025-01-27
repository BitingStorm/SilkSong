/**
 * @file	LevelManager.h
 * @brief   地图管理器定义，用于管理游戏所有场景地图
 * @author	Arty
 **/


#pragma once
#include <unordered_map>
#include <string>


class Level;


/*----------------------------------
			   地图管理器
  ----------------------------------*/
class LevelManager
{
public:
	void Initialize();

	void SetDefaultLevel(std::string levelName);

	void RefreshLevel();
private:
	std::unordered_map<std::string, Level*>levelMap;

	template<class T>
	void AddLevel(std::string levelName);

	Level* level_to_delete = nullptr;
};

template<class T>
inline void LevelManager::AddLevel(std::string levelName)
{
	T* pLevel = new T;
	if (pLevel && static_cast<Level*>(pLevel))
	{
		levelMap.insert({levelName,pLevel});
		return;
	}
	delete pLevel;
}
