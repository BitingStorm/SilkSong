/**
 * @file	Polygon.h
 * @brief   �����ģ�嶨��
 * @author	Arty
 **/


#pragma once
#include"Box2D.h" 
#include"Ray2D.h"
#include<vector>



namespace Math
{
	/* ����� */
	template<typename T>
	struct TPolygon
	{
		std::vector<TVector2<T>>initVertices;

		std::vector<TVector2<T>>vertices;

		TVector2<T>mean;

		bool bIsConvex;

		bool bIsValid;

		TPolygon<T>()
		{
			initVertices = {};
			vertices = {};
			bIsConvex = true;
			bIsValid = false;
		}

		/**
		 * @brief ������вι���(ʹ��Ĭ�ϵ���������)
		 * @param[in] vts ָ������ζ����������(���밴��˳ʱ�������ʱ������)
		 **/
		TPolygon<T>(const std::vector<TVector2<T>>& vts)
		{
			initVertices = SimplifyVertices(vts);
			vertices.resize(initVertices.size(), TVector2<T>::ZeroVector);
			bIsConvex = IsConvex(initVertices);
			bIsValid = (vts.size() >= 3);
		}

		/**
	     * @brief ������вι���(�ֶ�ǿ��ָ������)
	     * @param[in] vts ָ������ζ����������(���밴��˳ʱ�������ʱ������)
	     * @param[in] mean ָ�����������
	     **/
		TPolygon<T>(const std::vector<TVector2<T>>& vts, const TVector2<T>& mean)
		{
			initVertices = SimplifyVertices(vts);
			vertices.resize(initVertices.size(), TVector2<T>::ZeroVector);
			bIsConvex = IsConvex(initVertices);
			bIsValid = (vts.size() >= 3);
		}

		//�ж�һ�����Ƿ�λ�ڶ����֮��
		bool IsInside(const TVector2<T>& testPoint) const
		{
			if (!bIsValid)
			{
				return false;
			}

			//�Ӹõ㴹ֱ������һ�����ߣ���������߽�����Ϊ�������ڶ������
			int n = vertices.size();
			int num = 0;
			for (int i = 0, j = n - 1; i < n; j = i++)
			{
				if (FMath::IsSmallNumber(vertices[i].x - vertices[j].x))
				{
					continue;//���ñߴ�ֱ��ֱ������
				}
				T y = (vertices[i].y - vertices[j].y) / (vertices[i].x - vertices[j].x) * (testPoint.x - vertices[i].x) + vertices[i].y;
				if (FMath::Min(vertices[i].y, vertices[j].y) < y && y <= FMath::Max(vertices[i].y, vertices[j].y) && y > testPoint.y)
				{
					num++;
				}
			}
			return num % 2 == 1;
		}

		//��������
		void UpdateMean(const TVector2<T>& newMean)
		{
			mean = newMean;
			for (int i = 0; i < initVertices.size(); i++)
			{
				vertices[i] = mean + initVertices[i];
			}
		}

		//��������һ��������ཻ�����洢�����Ϣ
		FORCEINLINE bool Intersects(const TPolygon<T>& other, float& depth, TVector2<T>& normal, TVector2<T>& point) const;

		//�ж϶�����Ƿ�Ϊ͹�����
		static FORCEINLINE bool IsConvex(const std::vector<TVector2<T>>& vts);

		//�򻯶���ζ��㼯��(ȥ��λ�ڱ��ϵĶ��ඥ�㣬��������������ԭ�����)
		static FORCEINLINE std::vector<TVector2<T>> SimplifyVertices(const std::vector<TVector2<T>>& vts);

		/**
		 * @brief ��ȡ�������������
		 * @param[in] vts ����ζ��㼯��
		 **/
		static FORCEINLINE TVector2<T> FindArithmeticMean(const std::vector<TVector2<T>>& vts);
	};

	template<typename T>
	FORCEINLINE bool TPolygon<T>::Intersects(const TPolygon<T>& other, float& depth, TVector2<T>& normal, TVector2<T>& point) const
	{
		if (bIsConvex && other.bIsConvex)//SAT
		{
			size_t n = vertices.size();
			for (int i = 0, j = n - 1; i < n; j = i++)
			{
				TVector2<T> edge = vertices[i] - vertices[j];
				TVector2<T> axis(-edge.y, edge.x);
				axis.Normalize();

				T minA = TVector2<T>::DotProduct(vertices[0], axis);
				T maxA = minA;
				for (int i = 1; i < n; i++)
				{
					T proj = TVector2<T>::DotProduct(vertices[i], axis);

					if (proj < minA) minA = proj;
					if (proj > maxA) maxA = proj;
				}

				T minB = TVector2<T>::DotProduct(other.vertices[0], axis);
				T maxB = minB;
				for (int i = 1; i < n; i++)
				{
					T proj = TVector2<T>::DotProduct(other.vertices[i], axis);

					if (proj < minB) minB = proj;
					if (proj > maxB) maxB = proj;
				}

				if (minA >= maxB || minB >= maxA)
				{
					return false;
				}

				float axisDepth = FMath::Min(maxB - minA, maxA - minB);
				if (axisDepth < depth)
				{
					depth = axisDepth;
					normal = TVector2<T>::DotProduct(normal, other.mean - mean) > 0 ? axis : -axis;
				}
			}

			n = other.vertices.size();
			for (int i = 0, j = n - 1; i < n; j = i++)
			{
				TVector2<T> edge = other.vertices[i] - other.vertices[j];
				TVector2<T> axis(-edge.y, edge.x);
				axis.Normalize();

				T minA = TVector2<T>::DotProduct(vertices[0], axis);
				T maxA = minA;
				for (int i = 1; i < n; i++)
				{
					T proj = TVector2<T>::DotProduct(vertices[i], axis);

					if (proj < minA) minA = proj;
					if (proj > maxA) maxA = proj;
				}

				T minB = TVector2<T>::DotProduct(other.vertices[0], axis);
				T maxB = minB;
				for (int i = 1; i < n; i++)
				{
					T proj = TVector2<T>::DotProduct(other.vertices[i], axis);

					if (proj < minB) minB = proj;
					if (proj > maxB) maxB = proj;
				}

				if (minA >= maxB || minB >= maxA)
				{
					return false;
				}

				float axisDepth = FMath::Min(maxB - minA, maxA - minB);
				if (axisDepth < depth)
				{
					depth = axisDepth;
					normal = TVector2<T>::DotProduct(normal, other.mean - mean) > 0 ? axis : -axis;
				}
			}

			return true;
		}
		else//�Ƿ����һ�����㴦����һ�������
		{
			for (auto& point : other.vertices)
			{
				if (IsInside(point))
				{
					return true;
				}
			}
			return false;
		}
	}

	template<typename T>
	FORCEINLINE bool TPolygon<T>::IsConvex(const std::vector<TVector2<T>>& vts)
	{
		if (vts.size() < 3)return false;

		bool isPositive;
		isPositive = TVector2<T>::CrossProduct(vts.back() - vts[0], vts[0] - vts[1]) > 0;
		for (size_t i = 1; i < vts.size() - 1; ++i)
		{
			if ((TVector2<T>::CrossProduct(vts[i - 1] - vts[i], vts[i] - vts[i + 1]) > 0) ^ isPositive)
			{
				return false;
			}
		}
		if ((TVector2<T>::CrossProduct(vts[vts.size() - 2] - vts.back(), vts.back() - vts[0]) > 0) ^ isPositive)
		{
			return false;
		}

		return true;
	}

	template<typename T>
	FORCEINLINE std::vector<TVector2<T>> TPolygon<T>::SimplifyVertices(const std::vector<TVector2<T>>& vts)
	{
		std::vector<TVector2<T>> simplifiedVertices;

		if (vts.size() < 3) return simplifiedVertices;

		if (!FMath::IsSmallNumber(TVector2<T>::CrossProduct(vts.back() - vts[0], vts[0] - vts[1])))
		{
			simplifiedVertices.push_back(vts[0]);
		}
		for (size_t i = 1; i < vts.size() - 1; ++i)
		{
			if (!FMath::IsSmallNumber(TVector2<T>::CrossProduct(vts[i - 1] - vts[i], vts[i] - vts[i + 1])))
			{
				simplifiedVertices.push_back(vts[i]);
			}
		}
		if (!FMath::IsSmallNumber(TVector2<T>::CrossProduct(vts[vts.size() - 2] - vts.back(), vts.back() - vts[0])))
		{
			simplifiedVertices.push_back(vts.back());
		}

		TVector2<T>offset = FindArithmeticMean(simplifiedVertices);
		for (auto& point : simplifiedVertices)
		{
			point -= offset;
		}

		return simplifiedVertices;
	}

	template<typename T>
	FORCEINLINE TVector2<T> TPolygon<T>::FindArithmeticMean(const std::vector<TVector2<T>>& vts)
	{
		TVector2<T> sum = TVector2<T>::ZeroVector;
		for (auto& v : vts)
		{
			sum += v;
		}
		return sum / T(vts.size());
	}
}