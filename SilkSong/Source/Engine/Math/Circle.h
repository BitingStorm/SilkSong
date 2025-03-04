/**
 * @file	Circle.h
 * @brief   Ô²Ä£°å¶¨Òå
 * @author	Arty
 **/


#pragma once
#include"Vector2D.h" 



namespace Math
{
	/* Ô² */
	template<typename T>
	struct TCircle
	{
		TVector2<T> center;

		T radius;

		TCircle<T>()
		{
			center = TVector2<T>::ZeroVector;
			radius = T(0);
		}

		TCircle<T>(const TVector2<T>& center, T radius)
		{
			this->center = center;
			this->radius = radius;
		}
	};
}