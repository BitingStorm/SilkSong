/**
 * @file	Box2D.h
 * @brief   二维矩形框模板定义
 * @author	Arty
 **/


#pragma once
#include"Vector2D.h"


namespace Math
{
	/* 二维矩形框 */
	template<typename T>
	struct TBox2
	{
		TVector2<T> min;

		TVector2<T> max;

		TBox2<T>()
			: min(0, 0), max(0, 0) {}

		TBox2<T>(const TVector2<T>& inMin, const TVector2<T>& inMax)
			: min(inMin), max(inMax)
		{
			if (inMin.x >= inMax.x || inMin.y >= inMax.y)
			{
				min = max = TVector2<T>::ZeroVector;
			}
		}

		FORCEINLINE TBox2<T>(const TVector2<T>& center, T width, T height);

		static const TBox2<T> EmptyBox;

		bool operator==(const TBox2<T>& another) const
		{
			return min == another.min && max == another.max;
		}

		bool operator!=(const TBox2<T>& another) const
		{
			return min != another.min || max != another.max;
		}

		//获取矩形中心点
		TVector2<T> GetCenter() const
		{
			return TVector2<T>((min + max) * 0.5f);
		}

		//获取矩形大小
		TVector2<T> GetSize() const
		{
			return TVector2<T>(max - min);
		}

		//获取矩形大小的一半
		TVector2<T> GetHalf() const
		{
			return 0.5f * TVector2<T>(max - min);
		}

		//获取矩形面积
		T GetArea() const
		{
			return (max.x - min.x) * (max.y - min.y);
		}

		//判断一个点是否位于矩形之中
		bool IsInside(const TVector2<T>& testPoint) const
		{
			return ((testPoint.x > min.x) && (testPoint.x < max.x) && (testPoint.y > min.y) && (testPoint.y < max.y));
		}

		//判断一个点是否位于矩形之中(包含边缘)
		bool IsInsideOrOn(const TVector2<T>& testPoint) const
		{
			return ((testPoint.x >= min.x) && (testPoint.x <= max.x) && (testPoint.y >= min.y) && (testPoint.y <= max.y));
		}

		//判断一个点是否位于矩形边缘上
		bool IsOn(const TVector2<T>& testPoint) const
		{
			return !IsInside(testPoint) && IsInsideOrOn(testPoint);
		}

		//检查两个矩形是否相交
		FORCEINLINE bool Intersects(const TBox2<T>& other) const;

		//获取重叠部分矩形
		FORCEINLINE TBox2<T> Overlaps(const TBox2<T>& other) const;

		//拿到矩形距离目标点最近的那个点
		FORCEINLINE TVector2<T> GetClosestPointTo(const TVector2<T>& point) const;
	};

	template<typename T>
	FORCEINLINE TBox2<T>::TBox2(const TVector2<T>& center, T width, T height)
	{
		min = center - TVector2<T>(width * 0.5f, height * 0.5f);
		max = center + TVector2<T>(width * 0.5f, height * 0.5f);
	}

	template<typename T>
	FORCEINLINE bool TBox2<T>::Intersects(const TBox2<T>& other) const
	{
		if ((min.x > other.max.x) || (other.min.x > max.x))
		{
			return false;
		}
		if ((min.y > other.max.y) || (other.min.y > max.y))
		{
			return false;
		}
		return true;
	}

	template<typename T>
	FORCEINLINE TBox2<T> TBox2<T>::Overlaps(const TBox2<T>& other) const
	{
		if (!Intersects(other))
		{
			return EmptyBox;
		}

		TVector2<T> minVector, maxVector;

		minVector.x = FMath::Max(min.x, other.min.x);
		maxVector.x = FMath::Min(max.x, other.max.x);

		minVector.y = FMath::Max(min.y, other.min.y);
		maxVector.y = FMath::Min(max.y, other.max.y);

		return TBox2<T>(minVector, maxVector);
	}

	template<typename T>
	FORCEINLINE TVector2<T> TBox2<T>::GetClosestPointTo(const TVector2<T>& point) const
	{
		TVector2<T> closestPoint = point;

		if (point.x < min.x)
		{
			closestPoint.x = min.x;
		}
		else if (point.x > max.x)
		{
			closestPoint.x = max.x;
		}

		if (point.y < min.y)
		{
			closestPoint.y = min.y;
		}
		else if (point.y > max.y)
		{
			closestPoint.y = max.y;
		}

		return closestPoint;
	}

	template<typename T>
	const TBox2<T> TBox2<T>::EmptyBox = TBox2<T>();
}