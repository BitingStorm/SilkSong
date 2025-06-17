#include "CollisionManager.h"


//************************************************
//*********       用户需编辑部分          **********

void CollisionManager::Initialize()
{
	/** 
	 * 碰撞表建立(默认情况下的碰撞响应关系) 
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
	AddMapping(CollisionType::Bullet, CollisionType::Player);
	AddMapping(CollisionType::Bullet, CollisionType::Block);
}

bool CollisionManager::LayerMaskJudge(int layerMask1, CollisionType type2)
{
	return layerMask1 & (1 << int(type2));
}

int CollisionManager::FindMapping(CollisionType type)
{
	return layerMaskMap.find(type)->second;
}

//*********                             **********
//************************************************




CollisionManager::CollisionManager()
{
	layerMaskMap.insert({ CollisionType::Default, 1 });
}

void CollisionManager::AddMapping(CollisionType type1, CollisionType type2)
{
	if (layerMaskMap.find(type1) == layerMaskMap.end())
	{
		layerMaskMap.insert({ type1, 0 });
	}
	if (layerMaskMap.find(type2) == layerMaskMap.end())
	{
		layerMaskMap.insert({ type2, 0 });
	}

	layerMaskMap.find(type1)->second |= (1 << int(type2));
	layerMaskMap.find(type2)->second |= (1 << int(type1));
}
