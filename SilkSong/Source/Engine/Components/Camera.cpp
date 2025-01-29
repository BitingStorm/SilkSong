#include "Camera.h"
#include "Core/World.h"
#include "Tools/Math.h"
#include "easyx.h"


Vector2D Camera::Lerp(Vector2D start, Vector2D end, float t)
{
	t = Math::Clamp(t, 0.001f, 0.1f);
	return start + (end - start) * t;
}

float Camera::Lerp(float start, float end, float t)
{
	t = Math::Clamp(t, 0.001f, 0.1f);
	return start + (end - start) * t;
}

float Camera::SmoothStep(float x)
{
	x = x<1?1:x;
	return powf(x, 2);
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

void Camera::SetSmoothness(int smooth)
{
	this->smoothness = Math::Clamp(smooth, 0, 100);
}

void Camera::SetDistanceThreshold(float threshold)
{
	this->distanceThreshold = Math::Clamp(threshold, 0.f, 500.f);
}

void Camera::SetSpringArmLength(float length)
{
	this->springArmLength = Math::Clamp(length, 1.f, 10000.f);
}

void Camera::SetSpringArmSmoothness(int smooth)
{
	this->smoothnessForSpringArm = Math::Clamp(smooth, 0, 100);
}

void Camera::ShakeCamera(int intensity,int decay)
{
	intensity = Math::Clamp(intensity, 0, 100);
	decay = Math::Clamp(decay, 1, 100);
	shakeIntensity = (float)intensity;
	shakeDecay = decay;
	shakeFlag = true;
}

void Camera::SetRectFrame(const Rect& frame)
{
	this->frame = frame;
}

Vector2D Camera::GetVirtualPosition() const
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
	Vector2D pos = GetWorldPosition();
	if (frame.left != 0 && frame.right != 0 && frame.bottom != 0 && frame.top != 0)
		pos = { Math::Clamp(pos.x,frame.left,frame.right),Math::Clamp(pos.y, frame.bottom, frame.top) };
	if (smoothness)
	{
		transform_virtual.position = Lerp(transform_virtual.position, pos,
			0.1f / smoothness * SmoothStep(Vector2D::Distance(transform_virtual.position, pos) / distanceThreshold));
	}
	else transform_virtual.position = pos;
	

	/**
	 * 平滑视野缩放
	 **/
	if (smoothnessForSpringArm)springArmLength_virtual = Lerp(springArmLength_virtual, springArmLength, 0.1f/smoothnessForSpringArm);
	else springArmLength_virtual = springArmLength;

	/**
	 * 相机抖动
	 **/
	if (shakeFlag)
	{
		if (shakeIntensity <= 0) { shakeFlag = false; return; }
		double radian = Math::RandReal(0, 360) * PI / 180;
		transform_virtual.position -= lastShakeVector;
		transform_virtual.position += (lastShakeVector = shakeIntensity * Vector2D((float)cos(radian), (float)sin(radian)));
		shakeIntensity -= float(shakeDecay) * 0.005f;
	}
}
