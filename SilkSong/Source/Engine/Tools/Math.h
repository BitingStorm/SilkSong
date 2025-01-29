/**
 * @file	Math.h
 * @brief   ȫ����ѧ�����ⶨ��
 * @author	Arty
 **/

#pragma once
#include <random>


#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))


/*----------------------------------
			   ��ѧ���� 
  ----------------------------------*/
class Math
{
	static std::random_device rd;
	static std::mt19937 gen;
public:
	//��ȡ�������������
	static int RandInt(int lower,int upper);

	//��ȡʵ�����������
	static double RandReal(double lower,double upper);

	//��ȡ0-1֮��������
	static double RandPerc();

	//��ĳ��������ĳ����Χ��
	template<typename T>
	static T Clamp(T value, T lower, T upper)
	{
		return MIN(MAX(value, MIN(lower, upper)), MAX(lower, upper));
	}

	//������ת��Ϊ�Ƕ�
	static float RadianToDegree(float radian);

	//���Ƕȹ淶��(0~360����)
	static float NormalizeDegree(float angle);
};