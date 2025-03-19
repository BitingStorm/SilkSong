/**
 * @file	UserInterface.h
 * @brief   UI类模板，用于容纳并管理ui小部件
 * @author	Arty
 **/

#pragma once
#include"Widgets.h"
#include"Tools/Timer.h"


DECLARE_NO_PARAM_MULTICAST_DELEGATE_CLASS(ViewPortEvent)

/*----------------------------------
			    UI类 
  ----------------------------------*/
class UserInterface : public Object, public TimerHandler
{
	friend Panel;
	friend class WidgetComponent;

	std::unordered_set<Widget*>widgets;
	std::unordered_set<UserInterface*>userInterfaces;

	bool bRemoveFlag = false;

	void BeginPlay()override {}
	void EndPlay()override {}

protected:
	Widget* const rootCanvas = new Widget;

public:
	UserInterface();
	virtual ~UserInterface() { for (auto& widget : widgets)delete widget; }

	virtual void Update(float deltaTime);

	void ShowInfoBoxs();

	void DrawDebugRect();

	//添加widget小部件到UI模板管理
	template<class T>
	T* AddWidget()
	{
		T* gameWid = new T;
		if (Widget* buf = static_cast<Widget*>(gameWid))
		{
			widgets.insert(buf);
			return gameWid;
		}
		delete gameWid;
		return nullptr;
	}

	//将UI添加到视口
	void AddToViewport();

	//将UI从视口暂时隐藏
	void HideFromViewport();

	//将UI从视口移除
	void RemoveFromViewport();

	//将UI绑定在另一UI上
	void AttachTo(UserInterface* aim);

	//将UI从另一UI解绑
	void DettachFrom(UserInterface* aim);

	/* 视口事件 */
	ViewPortEvent OnAddedToViewport;
	ViewPortEvent OnHidFromViewport;
	ViewPortEvent OnRemovedFromViewport;

	//全局对象注册
	void RegisterDontDestroy();
};