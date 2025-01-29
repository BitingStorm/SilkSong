/**
 * @file	Math.h
 * @brief   全局数学函数库定义
 * @author	Arty
 **/

#pragma once
#include <random>


#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))


/*----------------------------------
			   数学工具 
  ----------------------------------*/
class Math
{
	static std::random_device rd;
	static std::mt19937 gen;
public:
	//获取整数类型随机数
	static int RandInt(int lower,int upper);

	//获取实数类型随机数
	static double RandReal(double lower,double upper);

	//获取0-1之间的随机数
	static double RandPerc();

	//将某数限制在某个范围内
	template<typename T>
	static T Clamp(T value, T lower, T upper)
	{
		return MIN(MAX(value, MIN(lower, upper)), MAX(lower, upper));
	}

	//将弧度转换为角度
	static float RadianToDegree(float radian);

	//将角度规范化(0~360区间)
	static float NormalizeDegree(float angle);
};