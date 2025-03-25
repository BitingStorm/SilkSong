/**
 * @file	ActorComponent.h
 * @brief	所有组件的基类ActorComponent定义
 * @author	Arty
 **/

#pragma once
#include"CoreMinimal.h"
#include"Tools/Timer.h"

class Actor;

DECLARE_NO_PARAM_MULTICAST_DELEGATE_CLASS(ComponentEvent)

/*----------------------------------
			  组件基类
  ----------------------------------*/
class ActorComponent : public Object, public TimerHandler
{
public:
	virtual ~ActorComponent() {}

	//获取绑定游戏对象
	Actor* GetOwner();

	//设置绑定游戏对象
	void SetOwner(Actor* owner);

	//销毁组件
	virtual void Destruct();

	//激活组件
	virtual void Activate();
	
	//停用组件
	virtual void Deactivate();

	//组件是否启用
	bool GetEnabled()const { return bIsEnabled; }

	/* 组件使用事件 */
	ComponentEvent OnActivated;
	ComponentEvent OnDeactivated;

	//组件所属全局对象注册
	virtual void RegisterDontDestroy() {}

protected:
	Actor* pOwner = nullptr;//处理控制逻辑

	bool bIsEnabled = true;//是否激活
};