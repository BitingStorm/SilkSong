/**
 * @file	Ray2D.h
 * @brief   二维射线模板定义
 * @author	Arty
 **/


#pragma once
#include"Vector2D.h" 


namespace Math
{
	template<typename T>
	struct TRay2
	{
		TVector2<T> origin;

		TVector2<T> direction;

		TRay2()
		{
			origin = TVector2<T>::ZeroVector;
			direction = TVector2<T>::UnitVector;
		}

		TRay2(const TVector2<T>& origin, const TVector2<T>& direction)
		{
			this->origin = origin;
			this->direction = direction;
			this->direction.Normalize();
		}

		bool operator==(const TRay2<T>& other) const
		{
			return (origin == other.origin) && direction.Equals(other.direction);
		}

		bool operator!=(const TRay2<T>& other) const
		{
			return !(*this == other);
		}

		/**
		 * @brief 获取射线上某一点
		 * @param[in] rayParameter 沿射线方向的标量距离
		 * @return 射线上的点
		 */
		TVector2<T> PointAt(T rayParameter) const
		{
			return origin + rayParameter * direction;
		}

		/**
		 * @brief 获取目标点在射线上投影点与起点间的标量距离(以射线方向为正)
		 * @param[in] 需要查询的点
		 * @return 目标点在射线上投影点与起点间的标量距离
		 */
		T GetParameter(const TVector2<T>& point) const
		{
			return TVector2<T>::DotProduct((point - origin), direction);
		}

		//判断某点是否位于射线上
		bool IsOn(const TVector2<T>& point) const
		{
			if (point == origin)
			{
				return true;
			}
			return FMath::IsSmallNumber(DistSquared(point));
		}

		/**
		 * @brief 找到点到射线的最小距离的平方
		 * @param[in] 需要查询的点
		 * @return 点到射线最小距离的平方
		 */
		T DistSquared(const TVector2<T>& point) const
		{
			T rayParameter = GetParameter(point);
			if (rayParameter < 0)
			{
				return TVector2<T>::DistSquared(origin, point);
			}
			else
			{
				return TVector2<T>::DistSquared(PointAt(rayParameter), point);
			}
		}

		/**
		 * @brief 找到点到射线的最小距离
		 * @param[in] 需要查询的点
		 * @return 点到射线最小距离
		 */
		T Dist(const TVector2<T>& point) const
		{
			return FMath::Sqrt(DistSquared(point));
		}

		/**
		 * @brief 找到射线上距离某点最近的点
		 * @param[in] 需要查询的点
		 * @return 距离某点最近的点
		 */
		TVector2<T> ClosestPoint(const TVector2<T>& point) const
		{
			T rayParameter = GetParameter(point);
			if (rayParameter < 0)
			{
				return origin;
			}
			else
			{
				return PointAt(rayParameter);
			}
		}
	};

}	