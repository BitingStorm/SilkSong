/**
 * @file	Polygon.h
 * @brief   多边形模板定义
 * @author	Arty
 **/


#pragma once
#include"Box2D.h" 
#include"Circle.h" 
#include"Segment2D.h"
#include<vector>



namespace Math
{
	/* 多边形 */
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
		 * @brief 多边形有参构造(使用默认的算数中心)
		 * @param[in] vts 指定多边形顶点相对坐标(必须按照顺时针或者逆时针排序)
		 **/
		TPolygon<T>(const std::vector<TVector2<T>>& vts)
		{
			initVertices = SimplifyVertices(vts);
			vertices.resize(initVertices.size(), TVector2<T>::ZeroVector);
			bIsConvex = IsConvex(initVertices);
			bIsValid = (vts.size() >= 3);
		}

		/**
	     * @brief 多边形有参构造(手动强制指定中心)
	     * @param[in] vts 指定多边形顶点相对坐标(必须按照顺时针或者逆时针排序)
	     * @param[in] mean 指定多边形中心
	     **/
		TPolygon<T>(const std::vector<TVector2<T>>& vts, const TVector2<T>& mean)
		{
			initVertices = SimplifyVertices(vts);
			vertices.resize(initVertices.size(), TVector2<T>::ZeroVector);
			bIsConvex = IsConvex(initVertices);
			bIsValid = (vts.size() >= 3);
		}

		//更新坐标转换
		void UpdateTransform(const TVector2<T>& newMean, T newDegree = T(0), const TVector2<T>& newScale = TVector2<T>::UnitVector)
		{
			mean = newMean;

			if (newScale == TVector2<T>::UnitVector && newDegree == T(0))
			{
				for (int i = 0; i < initVertices.size(); i++)
				{
					vertices[i] = mean + initVertices[i];
				}
				return;
			}
			if (newScale != TVector2<T>::UnitVector)
			{
				for (int i = 0; i < initVertices.size(); i++)
				{
					vertices[i] = initVertices[i] * newScale;
				}
			}
			if (newDegree != T(0))
			{
				for (int i = 0; i < initVertices.size(); i++)
				{
					vertices[i] = TVector2<T>::RotateAround(newDegree, TVector2<T>::ZeroVector, vertices[i]);
				}
			}
			for (int i = 0; i < initVertices.size(); i++)
			{
				vertices[i] += mean;
			}
		}

		//获取边界范围
		TBox2<T> GetExtents() const
		{
			T maxX, maxY, minX, minY;
			maxX = minX = vertices[0].x;
			maxY = minY = vertices[0].y;
			for (auto& vertex : vertices)
			{
				maxX = vertex.x > maxX ? vertex.x : maxX;
				maxY = vertex.x > maxY ? vertex.x : maxY;
				minX = vertex.x < minX ? vertex.x : minX;
				minY = vertex.x < minY ? vertex.x : minY;
			}
			return TBox2<T>(TVector2<T>(minX, minY), TVector2<T>(maxX, maxY));
		}

		//判断一个点是否位于多边形之内
		bool IsInside(const TVector2<T>& testPoint) const
		{
			if (!bIsValid)
			{
				return false;
			}

			//从该点垂直向上做一条射线，若射线与边交点数为奇数则在多边形内
			int n = vertices.size();
			int num = 0;
			for (int i = 0, j = n - 1; i < n; j = i++)
			{
				if (FMath::IsSmallNumber(vertices[i].x - vertices[j].x))
				{
					continue;//若该边垂直则直接跳过
				}
				T y = (vertices[i].y - vertices[j].y) / (vertices[i].x - vertices[j].x) * (testPoint.x - vertices[i].x) + vertices[i].y;
				if (FMath::Min(vertices[i].y, vertices[j].y) < y && y <= FMath::Max(vertices[i].y, vertices[j].y) && y > testPoint.y)
				{
					num++;
				}
			}
			return num % 2 == 1;
		}

		//尝试与另一个多边形相交，并存储相关信息
		FORCEINLINE bool Intersects(const TPolygon<T>& other, float& depth, TVector2<T>& normal, TVector2<T>& point) const;

		//尝试与另一个圆形相交，并存储相关信息
		FORCEINLINE bool Intersects(const TCircle<T>& other, float& depth, TVector2<T>& normal, TVector2<T>& point) const;

		//判断多边形是否为凸多边形
		static FORCEINLINE bool IsConvex(const std::vector<TVector2<T>>& vts);

		//简化多边形顶点集合(去除位于边上的多余顶点，并将算数中心与原点对齐)
		static FORCEINLINE std::vector<TVector2<T>> SimplifyVertices(const std::vector<TVector2<T>>& vts);

		/**
		 * @brief 获取多边形算数中心
		 * @param[in] vts 多边形顶点集合
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
		else//是否存在一个顶点处于另一多边形内
		{
			int n = vertices.size();
			for (auto& p : other.vertices)
			{
				if (IsInside(p))
				{
					point = p;

					TSegment2<T> aimSegment;
					T aimDist = T(0);
					for (int i = 0, j = n - 1; i < n; j = i++)
					{
						TSegment2<T> segment = TSegment2<T>(vertices[i], vertices[j]);
						if (segment.Dist(point) < aimDist)
						{
							aimSegment = segment;
						}
					}
					depth = aimSegment.Dist(point);
					normal = TVector2<T>(point - aimSegment.ClosestPoint(point));
					return true;
				}
			}
			for (int i = 0, j = n - 1; i < n; j = i++)
			{
				TSegment2<T> segment1(vertices[i], vertices[j]);
				int m = other.vertices.size();
				for (int a = 0, b = m - 1; a < m; b = a++)
				{
					TSegment2<T> segment2(vertices[a], vertices[b]);
					if (segment1.Intersects(segment2, point))
					{
						return true;
					}
				}
			}
			return false;
		}
	}

	template<typename T>
	FORCEINLINE bool TPolygon<T>::Intersects(const TCircle<T>& other, float& depth, TVector2<T>& normal, TVector2<T>& point) const
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

			T minB = TVector2<T>::DotProduct(other.center, axis) - other.radius;
			T maxB = minB + other.radius * 2;

			if (minA >= maxB || minB >= maxA)
			{
				return false;
			}

			float axisDepth = FMath::Min(maxB - minA, maxA - minB);
			if (axisDepth < depth)
			{
				depth = axisDepth;
				normal = TVector2<T>::DotProduct(normal, other.center - mean) > 0 ? axis : -axis;
			}
		}
		return false;
	}

	template<typename T>
	FORCEINLINE bool TPolygon<T>::IsConvex(const std::vector<TVector2<T>>& vts)
	{ 
		if (vts.size() < 3)return false;

		bool isPositive = TVector2<T>::CrossProduct(vts.back() - vts[0], vts[0] - vts[1]) > 0;
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