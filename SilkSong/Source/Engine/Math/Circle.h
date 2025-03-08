/**
 * @file	Circle.h
 * @brief   圆模板定义
 * @author	Arty
 **/


#pragma once
#include"Box2D.h" 



namespace Math
{
	/* 圆 */
	template<typename T>
	struct TCircle
	{
		TVector2<T> center;

		T radius;

		T initRadius;

		T rotationalInertia;

		TCircle<T>()
		{
			center = TVector2<T>::ZeroVector;
			initRadius = T(0);
			radius = initRadius;
		}

		TCircle<T>(const TVector2<T>& center, T radius)
		{
			this->center = center;
			initRadius = radius;
			this->radius = initRadius;
		}

		//获取边界范围
		TBox2<T> GetExtents() const
		{
			TVector2<T>min = center - TVector2<T>(radius, radius);
			TVector2<T>max = center + TVector2<T>(radius, radius);
			return TBox2<T>(min, max);
		}

		//更新坐标转换
		void UpdateTransform(const TVector2<T>& newCenter, const TVector2<T>& newScale = TVector2<T>::UnitVector)
		{
			center = newCenter;
			if (newScale != TVector2<T>::UnitVector)
			{
				radius = initRadius * FMath::Sqrt(newScale.x * newScale.y);
				rotationalInertia;
			}
		}
	};
}