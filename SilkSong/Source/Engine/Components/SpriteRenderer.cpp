#include "SpriteRenderer.h"
#include "Camera.h"
#include "Animator.h"
#include "Core/World.h"
#include "Tools/FileManager.h"
#pragma comment(lib,"Msimg32.lib")



void SpriteRenderer::DealImage()
{
	if (GetWorldRotation() != 0)
	{
		RotateImage(angle);
	}
	if (filterLayers.size() > 0)
	{
		FilterImage();
	}
}

SpriteRenderer::~SpriteRenderer()
{
	if (recoverTimerHandle)delete recoverTimerHandle; 
}

void SpriteRenderer::Render()
{
	if (!sprite || !bIsEnabled || alpha == 0)return;

	float multi_sa = 20.f / mainWorld.mainCamera->GetVirtualSpringArmLength();

	FVector2D pos = (GetWorldPosition() - mainWorld.mainCamera->GetVirtualPosition() + spriteInfo.offset * GetWorldScale())
		* multi_sa + FVector2D(WIN_WIDTH, WIN_HEIGHT) * 0.5f;

	int32 w = spriteInfo.endLoc.x - spriteInfo.startLoc.x, h = spriteInfo.endLoc.y - spriteInfo.startLoc.y;

	float multi_w = FMath::Abs(GetWorldScale().x) * multi_sa, multi_h = FMath::Abs(GetWorldScale().y) * multi_sa;

	IMAGE* img = (copy && angle) ? copy : sprite;
	if (filterLayers.size() > 0 && filter)img = filter;

	float draw_x = pos.x - multi_w * img->getwidth() * 0.5f, draw_y = pos.y - multi_h * img->getheight() * 0.5f;

	
	FRect cameraRect(FVector2D::ZeroVector, FVector2D(WIN_WIDTH, WIN_HEIGHT));
	FRect imgRect(FVector2D(pos.x, pos.y), img->getwidth() * multi_w, img->getheight() * multi_h);
	if (!cameraRect.Intersects(imgRect))//如若在视野外则不必进行接下来的渲染操作
	{
		return;
	}

	IMAGE* mirImg{};
	if (GetWorldScale().x * GetWorldScale().y < 0)//检查是否需要镜像颠倒
	{
		mirImg = new IMAGE(img->getwidth(), img->getheight());
		ImageToolkit::FlipImage(img, mirImg, GetWorldScale().x < 0);
		img = mirImg;
	}

	HDC dstDC = GetImageHDC();
	HDC srcDC = GetImageHDC(img);
	AlphaBlend(dstDC, int(draw_x), int(draw_y), int(w * multi_w), int(h * multi_h),
		srcDC, spriteInfo.startLoc.x, spriteInfo.startLoc.y, w, h, { AC_SRC_OVER,0,alpha,AC_SRC_ALPHA });
	if (mirImg)delete mirImg;
}

void SpriteRenderer::Update(float deltaTime)
{
	SceneComponent::Update(deltaTime);

	if (!sprite || !bIsEnabled)return;

	/**
	 * 图像变换处理逻辑
	 **/
	IMAGE* img = (copy && angle) ? copy : sprite;
	spriteInfo.size = FIntVector2(img->getwidth(), img->getheight());
	
	if (GetWorldRotation() != angle)
	{
		angle = GetWorldRotation();
		RotateImage(angle);
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

void SpriteRenderer::Blink(float duration, COLORREF color, int32 level)
{
	if (!isBlinking)
	{
		duration = FMath::Clamp(duration, 0.1f, 1.f);
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

void SpriteRenderer::RegisterDontDestroy()
{
	mainWorld.OverallRenders.insert(this);
}