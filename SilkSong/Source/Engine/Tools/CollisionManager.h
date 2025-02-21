#pragma once
#include <unordered_set>

//************************************************
//*********       �û���༭����          **********

/* ��ײ���� */
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
			  ��ײ������
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
