#pragma once
#include <unordered_set>

//************************************************
//*********       用户需编辑部分          **********

/* 碰撞类型 */
enum class CollisionType:uint8_t
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
