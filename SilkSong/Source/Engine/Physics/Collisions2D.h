/**
 * @file	Collisions2D.h
 * @brief   有关碰撞计算函数集合
 * @author	Arty
 **/


#pragma once
#include "Core/Struct.h"

class Collider;

namespace Physics2D
{
	class Collisions2D final
	{
	public:
		static bool CollisionTestCircleToCircle(Collider* c1, Collider* c2, FHitResult& hitResult);

		static bool CollisionTestPolygonToPolygon(Collider* c1, Collider* c2, FHitResult& hitResult);
	};
}