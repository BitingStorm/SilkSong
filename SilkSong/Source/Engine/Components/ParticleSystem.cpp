#include "ParticleSystem.h"
#include"Camera.h"
#include"Core/World.h"
#include"Tools/ResourceManager.h"
#include"Tools/Math.h"





void ParticleSystem::Produce()
{
	ParticleInfo temp;
	temp.position = GetWorldPosition();
	temp.alpha =  alpha;
	temp.maxSize = Math::RandReal(sizeRange.x, sizeRange.y);
	temp.size = temp.maxSize;
	if (fadingInTime > 0)
	{
		fadingType == ParticleFadingType::FadeInAndOut ? temp.alpha = 0 : temp.size = 0;
	}
	temp.index = Math::RandInt(0, number - 1);
	
	if (pattern == ParticlePattern::Center)
	{
		double radian = -Math::RandReal(scoop.x, scoop.y) * PI / 180;
		unitVector = Vector2D((float)cos(radian), (float)sin(radian));
		if(radius != Vector2D(0,0))temp.position += unitVector *  
			float((min(radius.x,radius.y) + std::abs(radius.y-radius.x) * Math::RandPerc()));
		temp.velocity = Math::RandReal(minSpeed, maxSpeed) * unitVector;
	}
	else if(pattern == ParticlePattern::Line)
	{
		double radian = -angle * PI / 180;
		unitVector = Vector2D((float)cos(radian), (float)sin(radian));
		Vector2D lineVector = Vector2D::RotateVector(90, unitVector);
		if(length)temp.position += lineVector * (float)(-length/2 + length * Math::RandPerc());
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

	if (particles.size() < capacity && capacity != -1)
	{
		if(interval == 0)for (int i = 0; i < capacity; i++)Produce();
		else if(duration<float>(steady_clock::now() - lastTime).count() >= interval)
		{
			Produce();lastTime = steady_clock::now();
		}

		if (!isLoop && particles.size() >= capacity) capacity = -1;  
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
			if (fadingType == ParticleFadingType::FadeInAndOut)
			{
				each.alpha -= alpha * deltaTime / fadingOutTime;
			}
			else if (fadingType == ParticleFadingType::ExpandAndShrink)
			{
				each.size -= each.maxSize * deltaTime / fadingOutTime;
			}
		}
		if (fadingInTime > 0 && duration<float>(steady_clock::now() - each.lastTime).count()
			<= fadingInTime)
		{
			if (fadingType == ParticleFadingType::FadeInAndOut)
			{
				each.alpha += alpha * deltaTime / fadingInTime;
			}
			else if (fadingType == ParticleFadingType::ExpandAndShrink)
			{
				each.size += each.maxSize * deltaTime / fadingInTime;
			}
		}
	}//粒子参数管理
}

void ParticleSystem::Render()
{
	if (!bIsEnabled||!images)return;

	HDC dstDC = GetImageHDC();
	int w = images[0]->getwidth();
	int h = images[0]->getheight();
	float dw = std::abs(float(w * GetWorldScale().x * 20.f / mainWorld.mainCamera->GetVirtualSpringArmLength()));
	float dh = std::abs(float(h * GetWorldScale().y * 20.f / mainWorld.mainCamera->GetVirtualSpringArmLength()));

	for (auto& each : particles)
	{
		float alpha = Math::Clamp(each.alpha, 0.f, 255.f);
		BLENDFUNCTION bf = { AC_SRC_OVER,0,(BYTE)alpha,AC_SRC_ALPHA };
		Vector2D pos = (each.position - mainWorld.mainCamera->GetVirtualPosition()) * 20.f / mainWorld.mainCamera->GetVirtualSpringArmLength()
			+ Vector2D(WIN_WIDTH, WIN_HEIGHT) * 0.5f - Vector2D(dw, dh) * each.size * 0.5f;
		HDC srcDC = GetImageHDC(images[each.index]);
		AlphaBlend(dstDC, (int)pos.x, (int)pos.y, (int)(dw * each.size), (int)(dh * each.size), srcDC, 0, 0, w, h, bf);
	}
}