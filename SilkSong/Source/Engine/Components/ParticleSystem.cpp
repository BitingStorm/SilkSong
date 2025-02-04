#include "ParticleSystem.h"
#include"Camera.h"
#include"Core/World.h"
#include"Tools/ResourceManager.h"
#include"Tools/Math.h"





void ParticleSystem::Produce()
{
	FParticleInfo temp;
	temp.position = GetWorldPosition();
	temp.alpha =  alpha;
	temp.maxSize = Math::RandReal(sizeRange.x, sizeRange.y);
	temp.size = temp.maxSize;
	if (fadingInTime > 0)
	{
		fadingType == EParticleFadingType::FadeInAndOut ? temp.alpha = 0 : temp.size = 0;
	}
	temp.index = Math::RandInt(0, number - 1);
	
	if (pattern == EParticlePattern::Center)
	{
		temp.degree = Math::RandReal(0, 360);
		float radian = -Math::DegreeToRadian(Math::RandReal(scoop.x, scoop.y));
		unitVector = FVector2D(cos(radian), sin(radian));
		if (radius != FVector2D(0, 0))temp.position += unitVector *
			float((min(radius.x, radius.y) + std::abs(radius.y - radius.x) * Math::RandPerc()));
		temp.velocity = Math::RandReal(minSpeed, maxSpeed) * unitVector;
	}
	else if (pattern == EParticlePattern::Line)
	{
		float radian = -Math::DegreeToRadian(angle);
		unitVector = FVector2D(cos(radian), sin(radian));
		FVector2D lineVector = FVector2D::RotateVector(90, unitVector);
		if (length)temp.position += lineVector * (float)(-length * 0.5f + length * Math::RandPerc());
		temp.velocity = Math::RandReal(minSpeed, maxSpeed) * unitVector;
	}
	particles.push_back(temp);
}

void ParticleSystem::Load(std::string name)
{
	AnimationResource aniRes = mainWorld.resourcePool->FetchAnimation(name);
	number = aniRes.num;
	images = aniRes.images;
}


void ParticleSystem::Update(float deltaTime)
{
	SceneComponent::Update(deltaTime);

	if (!bIsEnabled)return;

	if ((isLoop && particles.size() < capacity) || (!isLoop && parNum < capacity))
	{
		if (interval == 0)
		{
			for (int i = 0; i < capacity; i++)Produce();
			parNum += capacity;
		}
		else if (duration<float>(steady_clock::now() - lastTime).count() >= interval)
		{
			Produce(); lastTime = steady_clock::now(); parNum++;
		}
	}//生产管理

	if (particles.size()) 
	{
		if (interval == 0) 
		{
			if (duration<float>(steady_clock::now() - particles.begin()->lastTime).count() >= lifeCycle)
			{
				particles.clear();
			}
		}
		else if (duration<float>(steady_clock::now() - particles.begin()->lastTime).count() >= lifeCycle) 
		{
			particles.pop_front();
		}
	}//销毁逻辑

	for (auto &each : particles)
	{
		if(graivity)each.velocity.y += graivity * deltaTime;
		each.position += each.velocity * deltaTime;
		if (fadingOutTime > 0 && duration<float>(steady_clock::now() - each.lastTime).count()
			>= lifeCycle - fadingOutTime)
		{
			if (fadingType == EParticleFadingType::FadeInAndOut)
			{
				each.alpha -= alpha * deltaTime / fadingOutTime;
			}
			else if (fadingType == EParticleFadingType::ExpandAndShrink)
			{
				each.size -= each.maxSize * deltaTime / fadingOutTime;
			}
		}
		if (fadingInTime > 0 && duration<float>(steady_clock::now() - each.lastTime).count()
			<= fadingInTime)
		{
			if (fadingType == EParticleFadingType::FadeInAndOut)
			{
				each.alpha += alpha * deltaTime / fadingInTime;
			}
			else if (fadingType == EParticleFadingType::ExpandAndShrink)
			{
				each.size += each.maxSize * deltaTime / fadingInTime;
			}
		}
	}//粒子参数管理
}

void ParticleSystem::Render()
{
	if (!bIsEnabled || !images || alpha == 0)return;

	HDC dstDC = GetImageHDC();

	for (auto& each : particles)
	{
		int w = images[0]->getwidth();
		int h = images[0]->getheight();
		float dw = std::abs(float(w * GetWorldScale().x * 20.f / mainWorld.mainCamera->GetVirtualSpringArmLength()));
		float dh = std::abs(float(h * GetWorldScale().y * 20.f / mainWorld.mainCamera->GetVirtualSpringArmLength()));
		float alpha = Math::Clamp(each.alpha, 0.f, 255.f);
		BLENDFUNCTION bf = { AC_SRC_OVER,0,(BYTE)alpha,AC_SRC_ALPHA };
		FVector2D pos = (each.position - mainWorld.mainCamera->GetVirtualPosition()) * 20.f / mainWorld.mainCamera->GetVirtualSpringArmLength()
			+ FVector2D(WIN_WIDTH, WIN_HEIGHT) * 0.5f - FVector2D(dw, dh) * each.size * 0.5f;
		IMAGE* img = images[each.index];
		if (each.degree != 0)
		{
			img = new IMAGE;
			FPair newPair = ImageToolkit::RotateImage(images[each.index], img, each.degree - 90);
			w = std::abs(newPair.x), h = std::abs(newPair.y);
			dw = dw * float(w) / images[0]->getwidth(), dh = dh * float(h) / images[0]->getheight();
		}
		HDC srcDC = GetImageHDC(img);
		AlphaBlend(dstDC, (int)pos.x, (int)pos.y, (int)(dw * each.size), (int)(dh * each.size), srcDC, 0, 0, w, h, bf);
		if (img != images[each.index])delete img;
	}
}

void ParticleSystem::SetCenter(FVector2D radius, FVector2D scoop)
{
	this->radius = radius; this->scoop = scoop;
}

void ParticleSystem::SetLine(float length, float angle)
{
	this->length = length; this->angle = Math::NormalizeDegree(angle);
}