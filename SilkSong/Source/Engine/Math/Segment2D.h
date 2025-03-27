/**
 * @file	Segment2D.h
 * @brief   二维线段模板定义
 * @author	Arty
 **/


#pragma once
#include"Ray2D.h" 


namespace Math
{
	template<typename T>
	struct TSegment2
	{
		TRay2<T> ray1;
		TRay2<T> ray2;

		TSegment2()
		{
			ray1 = TRay2<T>();
			ray2 = TRay2<T>();
		}

		TSegment2(const &TVector2<T>p1, const& TVector2<T>p2)
		{
			ray1 = TRay2<T>(p1, p2 - p1);
			ray2 = TRay2<T>(p2, p1 - p2);
		}

		bool operator==(const TSegment2<T>& other) const
		{
			return ((ray1.origin == other.ray1.origin) && (ray2.origin == other.ray2.origin))
				|| ((ray1.origin == other.ray2.origin) && (ray2.origin == other.ray1.origin));
		}

		bool operator!=(const TSegment2<T>& other) const
		{
			return !(*this == other);
		}

		//判断某点是否位于线段上
		bool IsOn(const TVector2<T>& point) const
		{
			return ray1.IsOn(point) && ray2.IsOn(point);
		}

		/**
		 * @brief 找到点到线段的最小距离的平方
		 * @param[in] 需要查询的点
		 * @return 点到线段最小距离的平方
		 */
		T DistSquared(const TVector2<T>& point) const
		{
			if (ray1.GetParameter(point) >= 0 && ray2.GetParameter(point) >= 0)
			{
				return ray1.DistSquared(point);
			}
			return FMath::Min(TVector2<T>::DistSquared(point, ray1.origin), TVector2<T>::DistSquared(point, ray2.origin));
		}

		/**
		 * @brief 找到点到线段的最小距离
		 * @param[in] 需要查询的点
		 * @return 点到线段最小距离
		 */
		T Dist(const TVector2<T>& point) const
		{
			return FMath::Sqrt(DistSquared(point));
		}

		/**
		 * @brief 找到线段上距离某点最近的点
		 * @param[in] 需要查询的点
		 * @return 距离某点最近的点
		 */
		TVector2<T> ClosestPoint(const TVector2<T>& point) const
		{
			if (ray1.GetParameter(point) >= 0 && ray2.GetParameter(point) >= 0)
			{
				return ray1.PointAt(ray1.GetParameter(point));
			}
			else if (ray1.GetParameter(point) < 0)
			{
				return ray1.origin;
			}
			else
			{
				return ray2.origin;
			}
		}

		//判断是否与另一个线段相交
		bool Intersects(const TSegment2<T>& segment) const
		{
			TVector2<T> axis1(-ray1.direction.y, ray1.direction.x);
			axis1.Normalize();
			T A = TVector2<T>::DotProduct(ray1.origin, axis);
			T minB = FMath::Min(TVector2<T>::DotProduct(segment.ray1.origin, axis), TVector2<T>::DotProduct(segment.ray2.origin, axis));
			T maxB = FMath::Max(TVector2<T>::DotProduct(segment.ray1.origin, axis), TVector2<T>::DotProduct(segment.ray2.origin, axis));
			if (A > maxB || minB > A)
			{
				return false;
			}

			TVector2<T> axis2(-segment.ray1.direction.y, segment.ray1.direction.x);
			axis2.Normalize();
			T minA = FMath::Min(TVector2<T>::DotProduct(ray1.origin, axis), TVector2<T>::DotProduct(ray2.origin, axis));
			T maxA = FMath::Max(TVector2<T>::DotProduct(ray1.origin, axis), TVector2<T>::DotProduct(ray2.origin, axis));
			T B = TVector2<T>::DotProduct(segment.ray1.origin, axis);
			if (minA > B || B > maxA)
			{
				return false;
			}

		}
	};
}