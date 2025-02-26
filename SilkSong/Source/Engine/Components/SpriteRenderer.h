/**
 * @file	SpriteRenderer.h
 * @brief	2D渲染器
 * @author	Arty
 **/

#pragma once
#include"Components/SceneComponent.h"
#include"Tools/VisualInterface.h"




/*----------------------------------
			  渲染器（2D）
  ----------------------------------*/
class SpriteRenderer final: public SceneComponent, public LayerInterface, public ImageInterface
{
	DEFINE_SUPER(SceneComponent)

	void DealImage()override;

public:
	virtual ~SpriteRenderer();
	virtual void Render()override;
	virtual void Update(float deltaTime)override;

	/**
	 * @brief 使得图像闪光一次
	 * @param[in] duration			一次频闪持续时间
	 * @param[in] color	            频闪发光的滤镜颜色
	 * @param[in] level             发光滤镜施加程度（最大为100，最小为0）
	 **/
	void Blink(float duration = 0.2f, COLORREF color = BLACK, int32 level = 60);

	//保存图像资源到本地
	void SaveImage();

	virtual void RegisterDontDestroy()override;

private:
	bool isBlinking = false;
	bool blinkFlag = false;
	float transistionLevel = 0;
	float transistionCoefficient = 0;
	ArtyEngine::FilterInfo blinkFilter;
	Timer* recoverTimerHandle{};
};