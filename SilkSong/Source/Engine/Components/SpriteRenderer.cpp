#include "SpriteRenderer.h"
#include "Camera.h"
#include "Animator.h"
#include "Core/World.h"
#include "Tools/FileManager.h"
#include "Tools/Math.h"
#include "Tools/Timer.h"
#pragma comment(lib,"Msimg32.lib")



void SpriteRenderer::DealImage()
{
	if (GetWorldRotation() != 0)
	{
		RotateImage(PI * Math::NormalizeDegree(angle) / 180);
	}
	if (filterLayers.size() > 0)
	{
		FilterImage();
	}
}

SpriteRenderer::~SpriteRenderer()
{
	if (animatorAttached)animatorAttached->rendererAttached = nullptr;
	if (recoverTimerHandle)delete recoverTimerHandle;
}

void SpriteRenderer::Render()
{
	if (!sprite || !bIsEnabled || alpha == 0)return;

	Vector2D pos = (GetWorldPosition() - mainWorld.mainCamera->GetVirtualPosition() + spriteInfo.offset)
		* 20.f / mainWorld.mainCamera->GetVirtualSpringArmLength() + Vector2D(WIN_WIDTH / 2, WIN_HEIGHT / 2);
	HDC dstDC = GetImageHDC();


	int w = spriteInfo.endLoc.x - spriteInfo.startLoc.x;
	int h = spriteInfo.endLoc.y - spriteInfo.startLoc.y;

	float multi_w = std::abs(GetWorldScale().x) * 20.f / mainWorld.mainCamera->GetVirtualSpringArmLength();
	float multi_h = std::abs(GetWorldScale().y) * 20.f / mainWorld.mainCamera->GetVirtualSpringArmLength();

	IMAGE* img = copy ? copy : sprite;
	if (filterLayers.size() > 0 && filter)img = filter;

	
	IMAGE* mirImg{}; 
	if(GetWorldScale().x * GetWorldScale().y < 0)
	{	
		mirImg = new IMAGE(img->getwidth(),img->getheight());
		ImageToolkit::FlipImage(img, mirImg, GetWorldScale().x < 0);
		img = mirImg;
	}

	HDC srcDC = GetImageHDC(img);
	AlphaBlend(dstDC, (int)pos.x - int(multi_w * img->getwidth() / 2), (int)pos.y - int(multi_h * img->getheight() / 2),
		int(w * multi_w), int(h * multi_h),srcDC,(int)spriteInfo.startLoc.x, (int)spriteInfo.startLoc.y, w, h, { AC_SRC_OVER,0,alpha,AC_SRC_ALPHA });

	if (mirImg)delete mirImg;
}

void SpriteRenderer::Update(float deltaTime)
{
	SceneComponent::Update(deltaTime);

	if (!sprite || !bIsEnabled)return;

	/**
	 * 图像变换处理逻辑
	 **/
	IMAGE* img = copy ? copy : sprite;
	spriteInfo.size = Pair(img->getwidth(), img->getheight());
	
	if (GetWorldRotation() != angle)
	{
		angle = GetWorldRotation();
		RotateImage(PI * Math::NormalizeDegree(angle) / 180);
		if (filterLayers.size() > 0)
		{
			FilterImage();
		}
	}


	/**
	 * 闪光处理逻辑
	 **/
	if (!isBlinking)return;
	if (!blinkFlag)
	{
		if (transistionLevel > 0)transistionLevel -= transistionCoefficient * deltaTime;
		if (transistionLevel <= 0)isBlinking = false;
	}
	RemoveFilter();
	if (isBlinking)AddFilter({ blinkFilter.color, int(transistionLevel),1 });
}

void SpriteRenderer::Blink(float duration, COLORREF color, int level)
{
	if (!isBlinking)
	{
		duration = Math::Clamp(duration, 0.1f, 1.f);
		blinkFlag = true; isBlinking = true;
		blinkFilter = { color, level,1 };
		transistionLevel = float(level);
		transistionCoefficient = 2 * transistionLevel / duration; 
		if (!recoverTimerHandle)recoverTimerHandle = new Timer;
		recoverTimerHandle->Bind(duration / 2, [this]() {blinkFlag = false; });
	}
}

void SpriteRenderer::SaveImage()
{
	std::string path = "SaveGame/ProcessedImage/" + GetName() + ".png";
	
	FileManager::CreateFolder("SaveGame");
	FileManager::CreateFolder("SaveGame/ProcessedImage");

	saveimage(path.c_str(), sprite);
}





