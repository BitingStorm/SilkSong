 /**
  * @file	CoreMinimal.h
  * @brief	���Ĵ����ļ��������������ͷ�ļ��Լ�ȫ�ֺ���
  * @author	Arty
  **/


#pragma once


#include"Core/Macro.h"
#include"Core/Struct.h"
#include"Core/Object.h"
#include<windows.h>
#include<set>
#include<unordered_set>
#include<vector>
#include<ctime>
#include<cmath>



 #ifndef COREMINIMAL
 #define COREMINIMAL

//����ת��
template<typename T, typename F>
inline T * Cast(F * pBase)
{
	if(pBase)return dynamic_cast<T*>(pBase);

#ifdef _DEBUG
	std::cout << "Cast Error!" << std::endl;
#endif
	return nullptr;
}


//��ȡ�������ַ���
inline std::string GetRealTime()
{
	std::time_t now_time_t = std::time(nullptr);

	// ����һ��tm�ṹ�����洢����ʱ��
	std::tm now_tm;

	// ʹ��localtime_s��ȡ����ʱ��
	if (localtime_s(&now_tm, &now_time_t) == 0) {
		// ��ȡ�ꡢ�¡��ա�ʱ
		int year = now_tm.tm_year + 1900; // tm_year�Ǵ�1900�꿪ʼ������
		int month = now_tm.tm_mon + 1;    // tm_mon�Ǵ�0��ʼ���·�
		int day = now_tm.tm_mday;

		std::string real_tm = std::to_string(year) + "-" + std::to_string(month) + "-"
			+ std::to_string(day);
		return real_tm;
	}
	else std::cerr << "Failed to get local time." << std::endl;
	return std::string{};
}

//�Ƿ���֡������
extern bool bFrameLimitOn;

//���֡��
extern int MaxFrame;

#endif 