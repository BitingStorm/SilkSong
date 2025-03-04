/**
 * @file	CollisionManager.h
 * @brief   碰撞管理器定义，用于管理碰撞表
 * @author	Arty
 **/


#pragma once
#include <unordered_set>

//************************************************
//*********       用户需编辑部分          **********

/* 碰撞类型 */
///作者注：枚举最大值不得超过100
enum class CollisionType :uint8_t
{
	Default,
	Player,
	HurtBox,
	Enemy,
	Block,
	Item,
	Chest,
	Dart
};

//*********                             **********
//************************************************



/*----------------------------------
			  碰撞管理器
  ----------------------------------*/
class CollisionManager
{
public:
	CollisionManager();

	void Initialize();

	bool FindMapping(CollisionType type1, CollisionType type2);

private:
	std::unordered_set<int>collisionMap;

	void AddMapping(CollisionType type1, CollisionType type2);
};
