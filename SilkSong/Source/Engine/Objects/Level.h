/**
 * @file	Level.h
 * @brief   �ؿ��ඨ�塣һ��Worldͬһʱ�����ҽ���һ���ؿ���һ���ؿ�ͬһʱ��ֻ�ٿ�һ��Controller��
 * @author	Arty
 **/


#pragma once
#include"CoreMinimal.h"
#include"GameplayStatics.h"


/*----------------------------------
			  ��Ϸ�ؿ�
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