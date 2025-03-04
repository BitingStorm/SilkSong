 /**
  * @file	 CoreMinimal.h
  * @brief	 核心代码文件，包含引擎基本头文件以及全局函数
  * @author	 Arty
  **/


#pragma once
#include"Core/Macro.h"
#include"Core/Math.h"
#include"Math/Vector2D.h"
#include"Math/Box2D.h"
#include"Math/Transform.h"
#include"Core/Struct.h"
#include"Core/Object.h"
#include"Core/Delegate.h"
#include<windows.h>
#include<set>
#include<unordered_set>
#include<unordered_map>
#include<vector>
#include<stack>
#include<queue>
#include<ctime>
#include<cmath>
#include<cassert>


 #ifndef COREMINIMAL
 #define COREMINIMAL

//类型转换
template<typename T, typename F>
FORCEINLINE T * Cast(F * pBase)
{
	if(pBase)return dynamic_cast<T*>(pBase);

#ifdef _DEBUG
	std::cout << "Cast Error!" << std::endl;
#endif
	return nullptr;
}


//获取年月日字符串
inline std::string GetRealTime()
{
	std::time_t now_time_t = std::time(nullptr);

	// 定义一个tm结构体来存储本地时间
	std::tm now_tm;

	// 使用localtime_s获取本地时间
	if (localtime_s(&now_tm, &now_time_t) == 0) {
		// 获取年、月、日、时
		int year = now_tm.tm_year + 1900; // tm_year是从1900年开始的年数
		int month = now_tm.tm_mon + 1;    // tm_mon是从0开始的月份
		int day = now_tm.tm_mday;

		std::string real_tm = std::to_string(year) + "-" + std::to_string(month) + "-"
			+ std::to_string(day);
		return real_tm;
	}
	else std::cerr << "Failed to get local time." << std::endl;
	return std::string{};
}


//检测指针是否有效
inline void checkPtr(void* ptr, const char* ptrName, const char* fileName, int lineNumber)
{
	if (!ptr)
	{
		std::cerr << "Assertion failed: " << ptrName << " is nullptr in " << fileName << " at line " << lineNumber << std::endl;
		assert(false);
	}
}

//是否开启帧率限制
extern bool bFrameLimitOn;

//最大帧率
extern int MaxFrame;

#endif 