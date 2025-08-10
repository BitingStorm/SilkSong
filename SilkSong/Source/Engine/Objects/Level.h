/**
 * @file	Level.h
 * @brief   �ؿ��ඨ�塣һ��Worldͬһʱ�����ҽ���һ���ؿ���һ���ؿ�ͬһʱ��ֻ�ٿ�һ��Controller��
 * @author	Arty
 **/


#pragma once
#include"CoreMinimal.h"
#include"GameplayStatics.h"
#include"Tools/Timer.h"


DECLARE_NO_PARAM_MULTICAST_DELEGATE_CLASS(LevelEvent)

/*----------------------------------
			  ��Ϸ�ؿ�
  ----------------------------------*/
class Level :public Object, public TimerHandler
{
	friend GameplayStatics;
	friend class LevelManager;
	friend class Controller;

	Controller* mainController = nullptr;

	std::function<void()> callback;

	std::string levelName;
public:
	Level();

	virtual void BeginPlay()override;

	std::string GetLevelName() const { return levelName; }

	template<typename T>
	void SetDefaultController()
	{
		callback = [this]() {return mainController = GameplayStatics::CreateObject<T>(); };
	}

	LevelEvent OnLevelLoad;
	LevelEvent OnLevelDelete;
};