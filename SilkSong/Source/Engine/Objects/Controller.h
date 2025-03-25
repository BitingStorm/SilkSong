/**
 * @file	Controller.h
 * @brief   玩家控制器类定义，是每个场景中必须存在的对象（不唯一，但同一时刻只能处理一个控制器对象的逻辑），处理玩家交互逻辑
 * @author	Arty
 **/


#pragma once
#include"Actor.h"
#include"Components/InputComponent.h"



class Camera;


/*----------------------------------
			 玩家控制器类
  ----------------------------------*/
class Controller :public Actor
{
	DEFINE_SUPER(Actor)

	InputComponent* inputComponent;
	Camera* camera;

protected:
	virtual void SetupInputComponent(InputComponent* inputComponent) {}

public:
	Controller();

	virtual void BeginPlay() override;

	//更新点输入信息
	void PeekInfo();

	//更新轴输入信息
	void PeekInfo_();

	//更新鼠标信息
	void MouseTick();

	//获取鼠标世界坐标系位置
	FVector2D GetCursorPosition() const;

	//获取鼠标是否按下
	bool IsMouseClicked() const;

	//是否有任何按键被按下
	bool IsAnyKeyPressed() const;

	//是否有按键被按下
	bool IsKeyPressed(EKeyCode keycode) const;

	/**
	 * @brief 获取鼠标所处位置物体的信息（该物体必须拥有碰撞体）
	 * @return  FHitResult 世界坐标系下鼠标所处位置最顶层的物体信息
	 **/
	FHitResult GetHitResultUnderCursor() const;

	//激活输入输出
	void EnableInput(bool enable);
};