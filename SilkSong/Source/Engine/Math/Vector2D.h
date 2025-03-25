/**
 * @file	Vector2D.h
 * @brief   二维向量模板定义
 * @author	Arty
 **/


#pragma once
#include<iostream>
#include<string>
#include"Core/Math.h"


namespace Math
{
	/* 二维向量 */
	template<typename T>
	struct TVector2
	{
		static_assert(std::disjunction_v<std::is_integral<T>, std::is_floating_point<T>>,
			"T must be either an integral type or a floating point type");

		T x, y;
		FORCEINLINE TVector2();
		FORCEINLINE TVector2(T a, T b);
		explicit FORCEINLINE TVector2(T v);

		static const TVector2<T> ZeroVector;
		static const TVector2<T> UnitVector;

		FORCEINLINE TVector2<T> Zero() const { return ZeroVector; }
		FORCEINLINE TVector2<T> One() const { return UnitVector; }

		//获取平方和
		FORCEINLINE T SizeSquared() const;

		//获取向量长度
		FORCEINLINE T Size() const;

		//获取绝对值
		FORCEINLINE TVector2<T> GetAbs() const;

		//绝对值化
		FORCEINLINE void MakeAbs();

		//归零化, 将绝对值过小的值视作零
		FORCEINLINE void MakeZero(T tolerance = AE_KINDA_SMALL_NUMBER);

		/**
		 * @brief 获取单位向量
		 * @param[in] tolerance 对于待归一化的向量，所能接受其平方和的最小阈值
		 **/
		FORCEINLINE TVector2<T> GetSafeNormal(T tolerance = AE_SMALL_NUMBER) const;

		/**
		 * @brief 归一化向量
		 * @param[in] tolerance 对于待归一化的向量，所能接受其平方和的最小阈值
		 * @return 是否归一化成功
		 **/
		FORCEINLINE bool Normalize(T tolerance = AE_SMALL_NUMBER);

		/**
		 * @brief 将向量的所有分量限制在某个范围内
		 **/
		FORCEINLINE TVector2<T> ClampAxes(T minAxisVal, T maxAxisVal) const;

		/**
	     * @brief 检查一个向量是否可近似为零向量
	     * @param[in] tolerance 可近似为零的阈值
	     * @return 是否逼近零向量
	     */
		FORCEINLINE bool IsNearlyZero(T tolerance = AE_KINDA_SMALL_NUMBER) const;

		/**
	     * @brief 检查一个向量是否与另一个向量近似相等
	     * @param[in] tolerance 可近似看作相等的误差阈值
	     * @return 是否近似相等
	     */
		FORCEINLINE bool Equals(const TVector2<T>& v, T tolerance = AE_KINDA_SMALL_NUMBER) const;

		//将向量转化为字符串
		FORCEINLINE std::string ToString() const;


		TVector2<T> operator+(const TVector2<T>& another) const
		{
			return TVector2<T>(x + another.x, y + another.y);
		}

		TVector2<T> operator-(const TVector2<T>& another) const
		{
			return TVector2<T>(x - another.x, y - another.y);
		}

		TVector2<T>& operator+=(const TVector2<T>& another)
		{
			x += another.x; y += another.y;
			return *this;
		}

		TVector2<T>& operator-=(const TVector2<T>& another)
		{
			x -= another.x; y -= another.y;
			return *this;
		}

		TVector2<T> operator-() const
		{
			return TVector2<T>(-x, -y);
		}

	    bool operator==(const TVector2<T>& another) const
		{
			return x == another.x && y == another.y;
		}

		bool operator!=(const TVector2<T>& another) const
		{
			return x != another.x || y != another.y;
		}

		bool operator>(const TVector2<T>& another) const
		{
			return x > another.x && y > another.y;
		}

		bool operator<(const TVector2<T>& another) const
		{
			return x < another.x && y < another.y;
		}

		bool operator>=(const TVector2<T>& another) const
		{
			return x >= another.x && y >= another.y;
		}

		bool operator<=(const TVector2<T>& another) const
		{
			return x <= another.x && y <= another.y;
		}

		TVector2<T> operator*(float scale) const
		{
			return TVector2<T>(x * scale, y * scale);
		}

		TVector2<T>& operator*=(float scale)
		{
			x *= scale; y *= scale;
			return *this;
		}

		TVector2<T> operator/(float scale) const
		{
			return TVector2<T>(x / scale, y / scale);
		}

		TVector2<T>& operator/=(float scale)
		{
			x /= scale; y /= scale;
			return *this;
		}

		TVector2<T> operator*(const TVector2<T>& another) const
		{
			return TVector2<T>(x * another.x, y * another.y);
		}

		TVector2<T> operator/(const TVector2<T>& another) const
		{
			return TVector2<T>(x / another.x, y / another.y);
		}

		T operator[](int32 index) const
		{
			index = FMath::Clamp(index, 0, 1);
			return (index == 0) ? x : y;
		}

		//点乘
		T operator|(const TVector2<T>& v) const
		{
			return x * v.x + y * v.y;
		}

		//叉乘
		T operator^(const TVector2<T>& v) const
		{
			return x * v.y - v.x * y;
		}

		//向量点积
		static FORCEINLINE T DotProduct(const TVector2<T>& v1, const TVector2<T>& v2);

		//向量叉积（以z轴值为结果）
		static FORCEINLINE T CrossProduct(const TVector2<T>& v1, const TVector2<T>& v2);

		//获取两个坐标距离的平方
		static FORCEINLINE T DistSquared(const TVector2<T>& v1, const TVector2<T>& v2);

		//获取两个坐标之间的距离
		static FORCEINLINE T Distance(const TVector2<T>& v1, const TVector2<T>& v2);

		//将向量转换为角度
		static FORCEINLINE float VectorToDegree(const TVector2<T>& v);

		//将角度转换为单位向量
		static FORCEINLINE TVector2<T> DegreeToVector(float angle);

		/**
		 * @brief 计算某一向量旋转后的结果并返回
		 * @param[in] angle        待旋转角度
		 * @param[in] v            待旋转向量
		 * @return 旋转后的向量
		 */
		static FORCEINLINE TVector2<T> RotateVector(float angle, const TVector2<T>& v);

		/**
		 * @brief 计算某一点以另一点为中心旋转后的结果并返回
		 * @param[in] angle        待旋转角度
		 * @param[in] center       旋转中心
		 * @param[in] p            待旋转点
		 * @return 旋转后的点的坐标
		 */
		static FORCEINLINE TVector2<T> RotateAround(float angle, const TVector2<T>& center, const TVector2<T>& p);

		//将向量 u 投影到向量 v 所在的直线上
		static FORCEINLINE TVector2<T> ProjectVector(const TVector2<T>& u, const TVector2<T>& v);
	};

	template<typename T>
	FORCEINLINE std::ostream& operator<<(std::ostream& out, const TVector2<T>& vec)
	{
		return out << "(" << vec.x << "," << vec.y << ")";
	}

	template<typename T>
	FORCEINLINE TVector2<T> operator*(float scale, TVector2<T> vec)
	{
		return TVector2<T>(vec.x * scale, vec.y * scale);
	}

	template<typename T>
	FORCEINLINE TVector2<T>::TVector2()
		:x(0), y(0) {}

	template<typename T>
	FORCEINLINE TVector2<T>::TVector2(T a, T b)
		: x(a), y(b) {}

	template<typename T>
	FORCEINLINE TVector2<T>::TVector2(T v)
		: x(v), y(v) {}

	template<typename T>
	FORCEINLINE T TVector2<T>::SizeSquared() const
	{
		return x * x + y * y;
	}

	template<typename T>
	FORCEINLINE T TVector2<T>::Size() const
	{
		return FMath::Sqrt(x * x + y * y);
	}

	template<typename T>
	FORCEINLINE TVector2<T> TVector2<T>::GetAbs() const
	{
		return TVector2<T>(FMath::Abs(x), FMath::Abs(y));
	}

	template<typename T>
	FORCEINLINE void TVector2<T>::MakeAbs()
	{
		x = FMath::Abs(x);
		y = FMath::Abs(y);
	}

	template<typename T>
	FORCEINLINE void TVector2<T>::MakeZero(T tolerance)
	{
		x = FMath::Abs(x) < tolerance ? T(0) : x;
		y = FMath::Abs(y) < tolerance ? T(0) : y;
	}

	template<typename T>
	FORCEINLINE TVector2<T> TVector2<T>::GetSafeNormal(T tolerance) const
	{
		const T squareSum = x * x + y * y;
		if (squareSum > tolerance)
		{
			const T scale = FMath::InvSqrt(squareSum);
			return TVector2<T>(x, y) * scale;
		}
		return ZeroVector;
	}

	template<typename T>
	FORCEINLINE bool TVector2<T>::Normalize(T tolerance)
	{
		const T squareSum = x * x + y * y;
		if (squareSum > tolerance)
		{
			const T scale = FMath::InvSqrt(squareSum);
			x *= scale, y *= scale;
			return true;
		}
		x = y = T(0);
		return false;
	}

	template<typename T>
	FORCEINLINE TVector2<T> TVector2<T>::ClampAxes(T minAxisVal, T maxAxisVal) const
	{
		return TVector2<T>(FMath::Clamp(x, minAxisVal, maxAxisVal), FMath::Clamp(y, minAxisVal, maxAxisVal));
	}

	template<typename T>
	FORCEINLINE bool TVector2<T>::IsNearlyZero(T tolerance) const
	{
		return FMath::Abs(x) <= tolerance && FMath::Abs(y) <= tolerance;
	}

	template<typename T>
	FORCEINLINE bool TVector2<T>::Equals(const TVector2<T>& v, T tolerance) const
	{
		return FMath::Abs(x - v.x) <= tolerance && FMath::Abs(y - v.y) <= tolerance;
	}

	template<typename T>
	FORCEINLINE std::string TVector2<T>::ToString() const
	{
		return "(" + std::to_string((int)x) + "," + std::to_string((int)y) + ")";
	}

	template<typename T>
	FORCEINLINE T TVector2<T>::DotProduct(const TVector2<T>& v1, const TVector2<T>& v2)
	{
		return v1 | v2;
	}

	template<typename T>
	FORCEINLINE T TVector2<T>::CrossProduct(const TVector2<T>& v1, const TVector2<T>& v2)
	{
		return v1 ^ v2;
	}

	template<typename T>
	FORCEINLINE T TVector2<T>::DistSquared(const TVector2<T>& v1, const TVector2<T>& v2)
	{
		return TVector2<T>(v1 - v2).SizeSquared();
	}

	template<typename T>
	FORCEINLINE T TVector2<T>::Distance(const TVector2<T>& v1, const TVector2<T>& v2)
	{
		return FMath::Sqrt(DistSquared(v1, v2));
	}

	template<typename T>
	FORCEINLINE float TVector2<T>::VectorToDegree(const TVector2<T>& v)
	{
		if (v == ZeroVector)
		{
			return 0.f;
		}
		float radian = FMath::Atan2(-v.y, v.x);
		return FMath::RadianToDegree(radian);
	}

	template<typename T>
	FORCEINLINE TVector2<T> TVector2<T>::DegreeToVector(float angle)
	{
		float radian = -FMath::DegreeToRadian(angle);
		float fsin = FMath::Sin(radian);
		float fcos = FMath::Cos(radian);
		return TVector2<T>(fcos, fsin);
	}

	template<typename T>
	FORCEINLINE TVector2<T> TVector2<T>::RotateVector(float angle, const TVector2<T>& v)
	{
		float radian = FMath::DegreeToRadian(angle);
		float fsin = FMath::Sin(radian);
		float fcos = FMath::Cos(radian);
		return TVector2<T>(v.x * fcos - v.y * fsin, v.x * fsin + v.y * fcos);
	}

	template<typename T>
	FORCEINLINE TVector2<T> TVector2<T>::RotateAround(float angle, const TVector2<T>& center, const TVector2<T>& p)
	{
		return RotateVector(angle, p - center) + center;
	}

	template<typename T>
	FORCEINLINE TVector2<T> TVector2<T>::ProjectVector(const TVector2<T>& u, const TVector2<T>& v)
	{
		float scalar = DotProduct(u, v) * FMath::InvSqrt(v.SizeSquared());  // 计算投影向量的缩放因子
		return v.GetSafeNormal() * scalar;
	}

	template<typename T>
	const TVector2<T> TVector2<T>::ZeroVector = TVector2<T>();

	template<typename T>
	const TVector2<T> TVector2<T>::UnitVector = TVector2<T>(1, 1);
}