/**
 * @file	InputComponent.h
 * @brief	玩家鼠标键盘输入逻辑处理
 * @author	Arty
 **/


#pragma once
#include "ActorComponent.h"
#include <unordered_map>
#include <functional>
#include <easyx.h>


/* 按键信号类型 */
enum class InputType :uint8
{
	Pressed,
	Released,
	Holding,
	DoubleClick
};

/* 键码 */
enum KeyCode :int32
{
	VK_LButton = 1,
	VK_RButton,
	VK_MButton = 4,
	VK_Tab = 9,
	VK_Enter = 13,
	VK_Shift = 16,
	VK_Ctrl,
	VK_Esc = 27,
	VK_Space = 32,
	VK_A = 0x41,
	VK_B,VK_C,VK_D,
	VK_E,VK_F,VK_G,
	VK_H,VK_I,VK_J,
	VK_K,VK_L,VK_M,
	VK_N,VK_O,VK_P,
	VK_Q,VK_R,VK_S,
	VK_T,VK_U,VK_V,
	VK_W,VK_X,VK_Y,
	VK_Z,
};

/* 按键映射信息 */
struct KeyBindInfo
{
	std::function<void()> func;
	InputType type = InputType::Pressed;
	int pressFlag = 0;
	float lastTime = 0;
};


/*----------------------------------
			  输入交互组件
  ----------------------------------*/
class InputComponent final: public ActorComponent
{
public:
	//设置按键映射
	void SetMapping(std::string mappingName, KeyCode value);

	//绑定按键事件映射（瞬时触发，触发频率受帧率影响）
	template<typename T>
	void BindAction(std::string actionName,InputType type, T* obj, void(T::*func)())
	{
	    if(mappings.find(actionName)!= mappings.end())
		actionCallbacks.insert({ actionName, { std::bind(func, obj),type,false } });
	}

	//绑定按键事件映射（瞬时触发，触发频率受帧率影响）
	void BindAction(std::string actionName, InputType type, std::function<void()>func);


	//获取鼠标屏幕坐标
	static Vector2D GetMousePosition();
	
	//鼠标左键是否按下
	static bool IsMouseButtonPressed();

	//激活输入输出
	static void EnableInput(bool enable);

	//action
	void PeekInfo();
	
	//axis
	void PeekInfo_();

	void MouseTick();

private:
	std::unordered_map<std::string, KeyCode>mappings;
	std::unordered_map<std::string, KeyBindInfo>actionCallbacks;
	std::unordered_map<std::string, KeyBindInfo>axisCallbacks;

	static Vector2D mousePos;//鼠标在绘图窗口坐标
	static bool bActive;//是否启用交互
	ExMessage msg;
};
