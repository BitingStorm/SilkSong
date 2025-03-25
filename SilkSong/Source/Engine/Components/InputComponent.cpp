#include "InputComponent.h"
#include "GameplayStatics.h"



void InputComponent::SetMapping(std::string mappingName, EKeyCode value)
{
	mappings.insert({mappingName, value});
}


void InputComponent::BindAction(std::string actionName, EInputType type, std::function<void()> func)
{
	if (mappings.find(actionName) != mappings.end())
		actionCallbacks.insert({ actionName, { func,type,false } });
}

bool InputComponent::IsAnyKeyPressed()
{
	for (int i = 0; i < 256; ++i) 
	{
		if (GetAsyncKeyState(i) & 0x8000) return true;
	}
}

bool InputComponent::IsKeyPressed(EKeyCode keycode)
{
	return GetAsyncKeyState(int(keycode)) & 0x8000;
}

FVector2D InputComponent::GetMousePosition()
{
	return bActive ? mousePos : FVector2D{};
}

bool InputComponent::IsMouseButtonPressed()
{
	return bActive ? (GetAsyncKeyState(VK_LBUTTON) & 0x8000) : false;
}

void InputComponent::EnableInput(bool enable)
{
	bActive = enable;
}

void InputComponent::PeekInfo()
{
	if (!bIsEnabled || !bActive)return;

	for (auto& mapping : mappings)
	{
		FKeyBindInfo& info = actionCallbacks[mapping.first];
		if (info.type == EInputType::Holding)continue;
		if (GetAsyncKeyState((int)mapping.second) & 0x8000)
		{
			if (info.type == EInputType::Pressed && !info.pressFlag)info.func();
			if (info.type == EInputType::DoubleClick && info.lastTime > 0)
			{
				if (GameplayStatics::GetTimeSeconds() - info.lastTime < 0.5f) { info.func(); info.lastTime = -1; }
				else info.lastTime = 0;
			}
			info.pressFlag = true;
		}
		else if (info.pressFlag)
		{
			if (info.type == EInputType::Released)info.func();
			if (info.type == EInputType::DoubleClick && info.lastTime == 0)info.lastTime = float(GameplayStatics::GetTimeSeconds());
			if (info.type == EInputType::DoubleClick && info.lastTime == -1)info.lastTime = 0;
			info.pressFlag = false;
		}
	}
}

void InputComponent::PeekInfo_()
{
	if (!bIsEnabled || !bActive)return;

	for (auto& mapping : mappings)
	{
		FKeyBindInfo& info = actionCallbacks[mapping.first];
		if (info.type != EInputType::Holding)continue;
		if (GetAsyncKeyState((int)mapping.second) & 0x8000)
		{
			info.func();
		}
	}
}

void InputComponent::MouseTick()
{
	if (peekmessage(&msg))
	{
		mousePos = FVector2D(float(msg.x), float(msg.y));
	}
}



FVector2D InputComponent::mousePos = {};
bool InputComponent::bActive = true;