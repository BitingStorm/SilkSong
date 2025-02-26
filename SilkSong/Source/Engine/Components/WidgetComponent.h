/**
 * @file	WidgetComponent.h
 * @brief	小部件组件，用于将屏幕UI呈现在世界中
 * @author	Arty
 **/

#pragma once
#include"Components/SceneComponent.h"



class UserInterface;

 /*----------------------------------
			   小部件组件
   ----------------------------------*/
class WidgetComponent final : public SceneComponent
{
	DEFINE_SUPER(SceneComponent)

public:
	WidgetComponent();

	virtual void Update(float deltaTime)override;

	virtual void Activate()override;

	virtual void Deactivate()override;

	void SetUI(UserInterface* ui);

	UserInterface* GetUI() { return userInterface; }

	void SetSize(FVector2D size);

	virtual void RegisterDontDestroy()override;

private:
	UserInterface* userInterface;
};