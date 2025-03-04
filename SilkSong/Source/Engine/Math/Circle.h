/**
 * @file	Circle.h
 * @brief   Բģ�嶨��
 * @author	Arty
 **/


#pragma once
#include"Vector2D.h" 



namespace Math
{
	/* Բ */
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