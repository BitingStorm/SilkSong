/**
 * @file	Camera.h
 * @brief	2D相机组件定义
 * @author	Arty
 **/

#pragma once
#include "SceneComponent.h"



/*----------------------------------
			  摄像机组件
  ----------------------------------*/
class Camera final :public SceneComponent
{
	friend class World;

public:
	virtual void BeginPlay()override;

	//设置主摄像机
	void SetMainCamera();

	//设置相机移动平滑度
	void SetSmoothness(int32 smooth);

	//设置距离阈值
	void SetDistanceThreshold(float threshold);

	//设置相机弹簧长度
	void SetSpringArmLength(float length);

	//设置相机弹簧平滑度
	void SetSpringArmSmoothness(int32 smooth);

	//相机震动
	void ShakeCamera(int32 intensity, int32 decay = 10);

	//设置相机可活动框架
	void SetRectFrame(const FRect& frame);

	//获取虚拟相机位置
	FVector2D GetVirtualPosition()const;

	//获取虚拟相机弹簧长度
	float GetVirtualSpringArmLength()const;

private:
	FTransform transform_virtual;//虚拟相机场景属性

	FRect frame;//相机可活动框

	float distanceThreshold = 100.f;//距离阈值0`500
	int32 smoothness = 30;//平滑度0~100

	float shakeIntensity = 0;//震动强度0~100
	bool shakeFlag = false;//震动标志位
	int32 shakeDecay = 5;//震动衰减系数1~100
	FVector2D lastShakeVector;//记录上次震动偏移向量

	float springArmLength = 20.f;//弹簧长度1~10000
	float springArmLength_virtual = 100.f;//虚拟弹簧长度
	int32 smoothnessForSpringArm = 20;//弹簧平滑度0~100


	//平滑处理
	float SmoothStep(float x);

	//数值计算
	void Calculate();
};