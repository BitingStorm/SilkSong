/**
 * @file	Math.h
 * @brief   全局数学函数库定义
 * @author	Arty
 **/

#pragma once
#include <random>
#include <xmmintrin.h>
#include "Macro.h"


constexpr float AE_PI = 3.14159265f;
constexpr float AE_SMALL_NUMBER = 1.e-8f;
constexpr float AE_KINDA_SMALL_NUMBER = 1.e-4f;
constexpr float AE_BIG_NUMBER = 3.4e+38f;
constexpr float AE_EULERS_NUMBER = 2.71828183f;
constexpr float AE_GOLDEN_RATIO = 1.61803399f;


/*----------------------------------
			  基础数学库 
  ----------------------------------*/
struct FMath final
{
	//获取整数类型随机数
	static int32 RandInt(int32 lower, int32 upper);

	//获取实数类型随机数
	static float RandReal(float lower, float upper);

	//获取0~1之间的随机数
	static double RandPerc();

	//获取两数之间较大数
	template<typename T>
	static FORCEINLINE T Max(const T a, const T b)
	{
		return (a > b) ? a : b;
	}

	//获取两数之间较小数
	template<typename T>
	static FORCEINLINE T Min(const T a, const T b)
	{
		return (a < b) ? a : b;
	}

	//获取三数之间最大数
	template<typename T>
	static FORCEINLINE T Max(const T a, const T b, const T c)
	{
		return Max(Max(a, b), Max(a, c));
	}

	//获取三数之间最小数
	template<typename T>
	static FORCEINLINE T Min(const T a, const T b, const T c)
	{
		return Min(Min(a, b), Min(a, c));
	}

	//获取三数之间中位数
	template<typename T>
	static FORCEINLINE T Mid(const T a, const T b, const T c)
	{
		return a + b + c - Max(a, b, c) - Min(a, b, c);
	}

	//将某数限制在某个范围内
	template<typename T>
	static constexpr FORCEINLINE T Clamp(const T value, const T lower, const T upper)
	{
		return Min(Max(value, Min(lower, upper)), Max(lower, upper));
	}

	//取绝对值
	template<typename T>
	static FORCEINLINE T Abs(const T value)
	{
		return (value < T(0)) ? -value : value;
	}

	//取平方
	template<typename T>
	static FORCEINLINE T Square(const T value)
	{
		return value * value;
	}

	//取立方
	template<typename T>
	static FORCEINLINE T Cube(const T value)
	{
		return value * value * value;
	}

	//快速取开根的倒数
	static FORCEINLINE float InvSqrt(float inValue)
	{
		if (inValue == 0.f)
		{
			return 0.f;
		}
		const __m128 One = _mm_set_ss(1.0f);
		const __m128 Y0 = _mm_set_ss(inValue);
		const __m128 X0 = _mm_sqrt_ss(Y0);
		const __m128 R0 = _mm_div_ss(One, X0);
		float temp;
		_mm_store_ss(&temp, R0);
		return temp;
	}

	//是否为小量
	static FORCEINLINE float IsSmallNumber(float value)
	{
		return Abs(value) <= AE_SMALL_NUMBER;
	}

	//计算开根
	static FORCEINLINE float Sqrt(float value) 
	{ 
		return sqrtf(value); 
	}

	//计算幂方
	static FORCEINLINE float Pow(float a, float b) 
	{ 
		return powf(a, b); 
	}

	//e的幂方
	static FORCEINLINE float Exp(float value) 
	{ 
		return expf(value);
	}

	//以e为底取对数
	static FORCEINLINE float Log(float value)
	{
		return log(value);
	}

	//计算正弦值
	static FORCEINLINE float Sin(float value) 
	{
		return sinf(value); 
	}

	//计算余弦值
	static FORCEINLINE float Cos(float value) 
	{
		return cosf(value); 
	}

	//计算正切值
	static FORCEINLINE float Tan(float value) 
	{ 
		return tanf(value);
	}

	//计算反正切值
	static float Atan2(float y, float x);

	//浮点数取模
	static FORCEINLINE float Fmod(float a, float b)
	{
		if (IsSmallNumber(b))
		{
			return 0.0;
		}
		return fmodf(a, b);
	}

	//将弧度转换为角度
	static FORCEINLINE float RadianToDegree(float radian)
	{
		return radian * 180 / AE_PI;
	}

	//将角度转换为弧度
	static FORCEINLINE float DegreeToRadian(float degree)
	{
		return degree * AE_PI / 180;
	}

	//将角度规范化(0~360区间)
	static FORCEINLINE float NormalizeDegree(float angle)
	{
		float ang = Fmod(angle, 360.0f);
		return ang >= 0 ? ang : ang + 360;
	}


	/**
	 * @brief 计算线性插值
	 * @param[in] a           原数值
	 * @param[in] b           目标数值
	 * @param[in] x           参数值
	 * @return 最终插值
	 */
	template <typename T, typename U>
	static constexpr FORCEINLINE T Lerp(const T& a, const T& b, const U& alpha)
	{
		return (T)(a + alpha * (b - a));
	}

	/**
	 * @brief 计算平滑 Hermite 插值
	 * @param[in] a           下界
	 * @param[in] b           上界
	 * @param[in] x           参数值
	 * @return 0 到 1 之间的平滑插值
	 */
	template<typename T>
	static constexpr T SmoothStep(const T& a, const T& b, const T& x)
	{
		if (x < a)
		{
			return 0;
		}
		else if (x >= b)
		{
			return 1;
		}
		const T interpFraction = (x - a) / (b - a);
		return interpFraction * interpFraction * (3.0f - 2.0f * interpFraction);
	}

private:
	static std::random_device rd;
	static std::mt19937 gen;
};