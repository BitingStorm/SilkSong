#include "CollisionManager.h"


//************************************************
//*********       用户需编辑部分          **********

void CollisionManager::Initialize()
{
	/** 
	 * 碰撞表建立 
	 **/
	AddMapping(CollisionType::HurtBox,CollisionType::Enemy);
	AddMapping(CollisionType::Player, CollisionType::Block);
	AddMapping(CollisionType::Enemy, CollisionType::Block);
	AddMapping(CollisionType::Item, CollisionType::Block);
	AddMapping(CollisionType::Item, CollisionType::HurtBox);
	AddMapping(CollisionType::Chest, CollisionType::HurtBox);
	AddMapping(CollisionType::Dart, CollisionType::Block);
	AddMapping(CollisionType::Dart, CollisionType::Enemy);
	AddMapping(CollisionType::Dart, CollisionType::HurtBox);
}

//*********                             **********
//************************************************




CollisionManager::CollisionManager()
{
	collisionMap.insert({ int(CollisionType::Default) * int(CollisionType::Default) + int(CollisionType::Default) * int(CollisionType::Default) });
}


bool CollisionManager::FindMapping(CollisionType type1, CollisionType type2)
{
	int result = int(type1) * int(type1) + int(type2) * int(type2);
	return collisionMap.find(result)!= collisionMap.end();
}

void CollisionManager::AddMapping(CollisionType type1, CollisionType type2)
{
	int result = int(type1) * int(type1) + int(type2) * int(type2);
	collisionMap.insert(result);
}