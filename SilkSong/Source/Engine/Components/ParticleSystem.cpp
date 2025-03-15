#include "ParticleSystem.h"
#include"Camera.h"
#include"Core/World.h"
#include"Tools/ResourceManager.h"






void ParticleSystem::Produce()
{
	ArtyEngine::FParticleInfo temp;
	temp.position = GetWorldPosition();
	temp.alpha =  alpha;
	temp.maxSize = FMath::RandReal(sizeRange.x, sizeRange.y);
	temp.size = temp.maxSize;
	if (fadingInTime > 0)
	{
		fadingType == EParticleFadingType::FadeInAndOut ? temp.alpha = 0 : temp.size = 0;
	}
	temp.index = FMath::RandInt(0, number - 1);
	
	if (pattern == EParticlePattern::Center)
	{
		temp.degree = FMath::RandReal(0, 360);
		unitVector = FVector2D::DegreeToVector(FMath::RandReal(scoop.x, scoop.y));;
		if (radius != FVector2D::ZeroVector)
		{
			temp.position += unitVector * float((FMath::Min(radius.x, radius.y) + FMath::Abs(radius.y - radius.x) * FMath::RandPerc()));
		}
		temp.velocity = FMath::RandReal(minSpeed, maxSpeed) * unitVector;
	}
	else if (pattern == EParticlePattern::Line)
	{
		unitVector = FVector2D::DegreeToVector(angle);
		FVector2D lineVector = FVector2D::RotateVector(-90, unitVector);
		if (length)temp.position += lineVector * (float)(-length * 0.5f + length * FMath::RandPerc());
		float multi = 1.f;
		if (twoWay)multi = FMath::RandPerc() > 0.5 ? 1.f : -1.f;
		if (offset)temp.position += FMath::RandPerc() * offset * multi * unitVector;
		temp.velocity = FMath::RandReal(minSpeed, maxSpeed) * multi * unitVector;
	}
	particles.push_back(temp);
}

void ParticleSystem::Load(std::string name)
{
	ArtyEngine::FAnimationResource aniRes = mainWorld.resourcePool->FetchAnimation(name);
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
	float multi_sa = 20.f / mainWorld.mainCamera->GetVirtualSpringArmLength();

	for (auto& each : particles)
	{
		int32 w = images[0]->getwidth(), h = images[0]->getheight();
		float dw = FMath::Abs(w * GetWorldScale().x * multi_sa), dh = FMath::Abs(h * GetWorldScale().y * multi_sa);
		float alpha = FMath::Clamp(each.alpha, 0.f, 255.f);
		FVector2D pos = (each.position - mainWorld.mainCamera->GetVirtualPosition()) * multi_sa
			+ FVector2D(WIN_WIDTH, WIN_HEIGHT) * 0.5f;
		
		FRect cameraRect(FVector2D::ZeroVector, FVector2D(WIN_WIDTH, WIN_HEIGHT));
		FRect imgRect(FVector2D(pos.x, pos.y), dw, dh);
		if (!cameraRect.Intersects(imgRect) || alpha <= 0.f)
		{
			continue;
		}

		IMAGE* img = images[each.index];
		if (each.degree != 0)
		{
			img = new IMAGE;
			FIntVector2 newPair = ImageToolkit::RotateImage(images[each.index], img, each.degree - 90);
			w = FMath::Abs(newPair.x), h = FMath::Abs(newPair.y);
			dw = dw * w / images[0]->getwidth(), dh = dh * h / images[0]->getheight();
		}
		HDC srcDC = GetImageHDC(img);
		BLENDFUNCTION bf = { AC_SRC_OVER,0,(BYTE)alpha,AC_SRC_ALPHA };
		AlphaBlend(dstDC, int(pos.x - dw * 0.5f * each.size), int(pos.y - dh * 0.5f * each.size), int(dw * each.size), int(dh * each.size), srcDC, 0, 0, w, h, bf);
		if (img != images[each.index])delete img;
	}
}

void ParticleSystem::SetCenter(FVector2D radius, FVector2D scoop)
{
	this->radius = radius; this->scoop = scoop;
}

void ParticleSystem::SetLine(float length, float angle, float offset, bool twoWay)
{
	this->length = length; this->angle = FMath::NormalizeDegree(angle);
	this->offset = offset; this->twoWay = twoWay;
}

void ParticleSystem::RegisterDontDestroy()
{
	mainWorld.OverallRenders.insert(this);
}