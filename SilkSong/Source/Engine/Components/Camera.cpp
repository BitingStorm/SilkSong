#include "Camera.h"
#include "Core/World.h"
#include "easyx.h"



float Camera::SmoothStep(float x)
{
	x = x < 1 ? 1 : x;
	return FMath::Pow(x, 2);
}


void Camera::BeginPlay()
{
	SceneComponent::BeginPlay();

	/**
	 * 初始化虚拟参数
	 **/
	transform_virtual.position = GetWorldPosition();
	transform_virtual.rotation = GetWorldRotation();
	springArmLength_virtual = springArmLength;
}

void Camera::SetMainCamera()
{
	mainWorld.mainCamera = this;
}

void Camera::SetSmoothness(int32 smooth)
{
	this->smoothness = FMath::Clamp(smooth, 0, 100);
}

void Camera::SetDistanceThreshold(float threshold)
{
	this->distanceThreshold = FMath::Clamp(threshold, 0.f, 500.f);
}

void Camera::SetSpringArmLength(float length)
{
	this->springArmLength = FMath::Clamp(length, 1.f, 10000.f);
}

void Camera::SetSpringArmSmoothness(int32 smooth)
{
	this->smoothnessForSpringArm = FMath::Clamp(smooth, 0, 100);
}

void Camera::ShakeCamera(int32 intensity, int32 decay)
{
	intensity = FMath::Clamp(intensity, 0, 100);
	decay = FMath::Clamp(decay, 1, 100);
	shakeIntensity = (float)intensity;
	shakeDecay = decay;
	shakeFlag = true;
}

void Camera::SetRectFrame(const FRect& frame)
{
	this->frame = frame;
}

FVector2D Camera::GetVirtualPosition() const
{
	return transform_virtual.position;
}

float Camera::GetVirtualSpringArmLength() const
{
	return springArmLength_virtual;
}

void Camera::Calculate()
{
	if (!bIsEnabled)return;

	/**
	 * 平滑位移
	 **/
	FVector2D position = GetWorldPosition();
	if (frame != FRect::EmptyBox)
	{
		position = { FMath::Clamp(position.x,frame.min.x,frame.max.x),FMath::Clamp(position.y, frame.min.y, frame.max.y) };
	}
	float distance = FVector2D::Distance(transform_virtual.position, position);
	if (smoothness && distance > AE_SMALL_NUMBER)
	{
		float alpha = 0.1f / smoothness * SmoothStep(distance / distanceThreshold);
		transform_virtual.position = FMath::Lerp(transform_virtual.position, position, FMath::Clamp(alpha, 0.001f, 0.1f));
	}
	else
	{
		transform_virtual.position = position;
	}
	
	/**
	 * 平滑视野缩放
	 **/
	if (smoothnessForSpringArm)springArmLength_virtual = FMath::Lerp(springArmLength_virtual, springArmLength, 0.1f/smoothnessForSpringArm);
	else springArmLength_virtual = springArmLength;

	/**
	 * 相机抖动
	 **/
	if (shakeFlag)
	{
		if (shakeIntensity <= 0) { shakeFlag = false; return; }
		float radian = FMath::DegreeToRadian(float(FMath::RandReal(0, 360)));
		transform_virtual.position -= lastShakeVector;
		transform_virtual.position += (lastShakeVector = shakeIntensity * FVector2D(FMath::Cos(radian), FMath::Sin(radian)));
		shakeIntensity -= float(shakeDecay) * 0.005f;
	}
}