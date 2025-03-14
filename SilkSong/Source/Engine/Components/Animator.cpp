#include"Animator.h"
#include"Core/World.h"
#include"Tools/ResourceManager.h"
#include"Tools/VisualInterface.h"


void Animation::Tick()
{
	if (!animController)
	{
		return;
	}

	//如果不可循环则会静止在最后一帧
	if (!bMontage && !bLooping)
	{
		if (index == num - 1 && !IsReverse())
		{
			return;
		}
		if (index == 0 && IsReverse())
		{
			return;
		}
	}

	//更新动画帧
	index = ((index + (IsReverse() ? -1 : 1)) + num) % num;

	//若为蒙太奇动画，则播放一遍后立马跳出
	if (((index == 0 && !IsReverse()) || (index == num - 1 && IsReverse())) && bMontage)
	{
		bMontage = false;
		if (OnMontageExit.IsBinded())
		{
			OnMontageExit.Execute();
			return;
		}
		else
		{
			for (auto edge : nexts)
			{
				if (animController->CheckConditions(edge))
				{
					animController->SetNode(edge->end);
					return;
				}
			}
		}
	}

	//若无过渡条件，则播放一遍结束立马到下一个节点
	for (auto edge : nexts)
	{
		if (edge->IsUnconditional() && ((index == 0 && !IsReverse()) || (index == num - 1 && IsReverse())))
		{
			animController->SetNode(edge->end);
			return;
		}
	}

	//执行动画通知
	if (notifications.find(index) != notifications.end())
	{
		notifications[index].Execute();
	}
}

void Animation::Load(std::string name, FVector2D delta)
{
	ArtyEngine::FAnimationResource aniRes = mainWorld.resourcePool->FetchAnimation(name);
	num = aniRes.num;
	images = aniRes.images;
	offset = delta;
}






Animator::~Animator()
{
	if (rendererAttached)rendererAttached->animatorAttached = nullptr;
}

void Animator::BeginPlay()
{
	if (rendererAttached)
	{
		return;
	}

	rendererAttached = pOwner->GetComponentByClass<ImageInterface>();
	if (rendererAttached)
	{
		rendererAttached->animatorAttached = this;
	}
}

void Animator::Update(float deltaTime)
{
	if (!rendererAttached || !aniNode)
	{
		return;
	}

	//更新渲染器图像
	IMAGE* sprite = aniNode->images[aniNode->index];
	if (currentSprite != sprite)
	{
		if (currentSprite && !bIsEnabled)
		{
			return;
		}
		currentSprite = sprite;
		rendererAttached->sprite = sprite;

		rendererAttached->spriteInfo.offset = aniNode->offset;
		rendererAttached->spriteInfo.endLoc = { sprite->getwidth(), sprite->getheight() };
		rendererAttached->DealImage();
	}

	//检查过渡条件
	if (aniNode->bMontage)
	{
		return;
	}
	for (auto edge : aniNode->nexts) 
	{
		if (CheckConditions(edge))
		{
			SetNode(edge->end);
			break;
		}
	}
}

void Animator::Insert(std::string name, Animation& ani)
{
	if (!ani.images)
	{
		return;
	}
	animations.insert({ name,ani });
	ani.animController = this;
}


void Animator::SetNode(std::string nodeName)
{
	if (aniNode && !aniNode->exitLock)
	{
		Animation* lastNode = aniNode;
		lastNode->clock.Stop();
		lastNode->exitLock = true;
		lastNode->OnAnimExit.Execute();
		lastNode->exitLock = false;
		if (aniNode != lastNode)
		{
			return;
		}
	}

	aniNode = &(animations.find(nodeName)->second);
	if (!aniNode)
	{
		return;
	}
	aniNode->index = aniNode->bReverse ? (aniNode->num - 1) : 0;
	aniNode->clock.Continue();
	aniNode->OnAnimEnter.Execute();
}

void Animator::SetNode(Animation* node)
{
	if (aniNode && !aniNode->exitLock)
	{
		Animation* lastNode = aniNode;
		lastNode->clock.Stop(); 
		lastNode->exitLock = true;
		lastNode->OnAnimExit.Execute();
		lastNode->exitLock = false;
		if (aniNode != lastNode)
		{
			return;
		}
	}

	aniNode = node;
	aniNode->index = aniNode->bReverse ? (aniNode->num - 1) : 0;
	aniNode->clock.Continue(); 
	aniNode->OnAnimEnter.Execute();
}

bool Animator::IsPlaying(std::string nodeName)
{
	return aniNode == &(animations.find(nodeName)->second);
}


void Animator::SetupAttachment(ImageInterface* renderer)
{
	rendererAttached = renderer;
	renderer->animatorAttached = this;
}

void Animator::PlayMontage(std::string nodeName)
{
	if (aniNode == &(animations.find(nodeName)->second))//若当前正在播放相同蒙太奇动画，则重置为第一帧
	{
		aniNode->index = aniNode->bReverse ? (aniNode->num - 1) : 0; return;
	}
	lastNode = aniNode;
	SetNode(nodeName);
	if (!aniNode)
	{
		return;
	}
	aniNode->bMontage = true;
	
	if (rendererAttached)
	{
		IMAGE* sprite = aniNode->images[aniNode->index];
		currentSprite = sprite;
		rendererAttached->sprite = sprite;

		rendererAttached->spriteInfo.offset = aniNode->offset;
		rendererAttached->spriteInfo.endLoc = { sprite->getwidth(), sprite->getheight() };
		rendererAttached->DealImage();
	}

	if (aniNode->nexts.empty())
	{
		//若蒙太奇节点独立(无后继节点），则播放完毕后默认恢复之前的动画节点 
		aniNode->OnMontageExit.Bind([this]()
			{
				aniNode->clock.Stop(); aniNode->OnAnimExit.Execute();
				aniNode = lastNode;
				aniNode->clock.Continue();
				aniNode->OnAnimEnter.Execute();
			});
	}
}

void Animator::AddParamater(std::string paramName, ParamType type)
{
	if (type == ParamType::Integer)
	{
		paramatersInteger.insert({ paramName,int()});
	}
	else if (type == ParamType::Float)
	{
		paramatersFloat.insert({paramName,float()});
	}
	else if (type == ParamType::Bool)
	{
		paramatersBool.insert({paramName,bool()});
	}
	else
	{
		paramatersTrigger.insert({paramName,false});
	}
}

void Animator::SetInteger(std::string paramName, int32 value)
{
	if (paramatersInteger.find(paramName) != paramatersInteger.end())
	{
		paramatersInteger[paramName] = value;
	}
}

void Animator::SetFloat(std::string paramName, float value)
{
	if (paramatersFloat.find(paramName) != paramatersFloat.end())
	{
		paramatersFloat[paramName] = value;
	}
}

void Animator::SetBool(std::string paramName, bool value)
{
	if (paramatersBool.find(paramName) != paramatersBool.end())
	{
		paramatersBool[paramName] = value;
	}
}

void Animator::SetTrigger(std::string paramName)
{
	if (paramatersTrigger.find(paramName) != paramatersTrigger.end())
	{
		paramatersTrigger[paramName] = true;
	}
}

int32 Animator::GetInteger(std::string paramName)
{
	if (paramatersInteger.find(paramName) != paramatersInteger.end())
	{
		return paramatersInteger[paramName];
	}
	return {};
}

float Animator::GetFloat(std::string paramName)
{
	if (paramatersFloat.find(paramName) != paramatersFloat.end())
	{
		return paramatersFloat[paramName];
	}
	return {};
}

bool Animator::GetBool(std::string paramName)
{
	if (paramatersBool.find(paramName) != paramatersBool.end())
	{
		return paramatersBool[paramName];
	}
	return {};
}

bool Animator::CheckConditions(AnimEdge* edge)
{
	bool result = false;
	for (const auto& condition : edge->integerConditions) {

		if (paramatersInteger.find(condition.paramName) == paramatersInteger.end())continue;
		result = AnimTransition::GetComparisonResult(paramatersInteger[condition.paramName], condition.value, condition.comparison);
		if (result && edge->comparisonMode == AnimTransition::OR)return true;
		if (!result && edge->comparisonMode == AnimTransition::AND)return false;
	}
	for (const auto& condition : edge->floatConditions) {

		if (paramatersFloat.find(condition.paramName) == paramatersFloat.end())continue;
		result = AnimTransition::GetComparisonResult(paramatersFloat[condition.paramName], condition.value, condition.comparison);
		if (result && edge->comparisonMode == AnimTransition::OR)return true;
		if (!result && edge->comparisonMode == AnimTransition::AND)return false;
	}
	for (const auto& condition : edge->boolConditions) {

		if (paramatersBool.find(condition.paramName) == paramatersBool.end())continue;
		result = (paramatersBool[condition.paramName] == condition.value);
		if (result && edge->comparisonMode == AnimTransition::OR)return true;
		if (!result && edge->comparisonMode == AnimTransition::AND)return false;
	}
	for (const auto& condition : edge->triggerConditions) {

		if (paramatersTrigger.find(condition.paramName) == paramatersTrigger.end())continue;
		result = paramatersTrigger[condition.paramName];
		paramatersTrigger[condition.paramName] = false;
		if (result && edge->comparisonMode == AnimTransition::OR)return true;
		if (!result && edge->comparisonMode == AnimTransition::AND)return false;
	}
	return result;
}

void Animator::Activate()
{
	Super::Activate();
	if (aniNode)
	{
		aniNode->clock.Continue();
	}
}

void Animator::Deactivate()
{
	Super::Deactivate();
	if (aniNode)
	{
		aniNode->clock.Stop();
	}
}