/**
 * @file	Segment2D.h
 * @brief   ��ά�߶�ģ�嶨��
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

		//�ж�ĳ���Ƿ�λ���߶���
		bool IsOn(const TVector2<T>& point) const
		{
			return ray1.IsOn(point) && ray2.IsOn(point);
		}

		/**
		 * @brief �ҵ��㵽�߶ε���С�����ƽ��
		 * @param[in] ��Ҫ��ѯ�ĵ�
		 * @return �㵽�߶���С�����ƽ��
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
		 * @brief �ҵ��㵽�߶ε���С����
		 * @param[in] ��Ҫ��ѯ�ĵ�
		 * @return �㵽�߶���С����
		 */
		T Dist(const TVector2<T>& point) const
		{
			return FMath::Sqrt(DistSquared(point));
		}

		/**
		 * @brief �ҵ��߶��Ͼ���ĳ������ĵ�
		 * @param[in] ��Ҫ��ѯ�ĵ�
		 * @return ����ĳ������ĵ�
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

		//�ж��Ƿ�����һ���߶��ཻ
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