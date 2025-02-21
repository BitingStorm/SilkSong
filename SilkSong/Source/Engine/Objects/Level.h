/**
 * @file	Level.h
 * @brief   关卡类定义。一个World同一时刻有且仅有一个关卡，一个关卡同一时间只操控一个Controller类
 * @author	Arty
 **/


#pragma once
#include"CoreMinimal.h"
#include"GameplayStatics.h"


/*----------------------------------
			  游戏关卡
  ----------------------------------*/
class Level :public Object
{
	friend GameplayStatics;

	class Controller* mainController = nullptr;

	std::function<void()> callback;

public:
	virtual void BeginPlay()override;

	template<typename T>
	void SetDefaultController()
	{
		callback = [this]() {return mainController = GameplayStatics::CreateObject<T>(); };
	}
};