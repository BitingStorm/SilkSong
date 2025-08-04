/**
 * @file	InputComponent.h
 * @brief	玩家鼠标键盘输入逻辑处理
 * @author	Arty
 **/


#pragma once
#include "ActorComponent.h"
#include <easyx.h>


/* 按键信号类型 */
enum class EInputType :uint8
{
	Pressed,
	Released,
	Holding,
	DoubleClick
};

/* 键码 */
enum class EKeyCode :int32
{
	VK_LButton = 0x01,
	VK_RButton,
	VK_MButton = 0x04,
	VK_Backspace = 0x08,
	VK_Tab,
	VK_Enter = 0x0D,
	VK_Shift = 0x10,
	VK_Ctrl, 
	VK_Alt,
	VK_Esc = 0x1B,
	VK_Space = 0x20,
	VK_Delete = 0x2E,
	VK_1 = 0x30, VK_2, VK_3, VK_4, VK_5,
	VK_6, VK_7, VK_8, VK_9, VK_0,
	VK_A = 0x41,
	VK_B, VK_C, VK_D,
	VK_E, VK_F, VK_G,
	VK_H, VK_I, VK_J,
	VK_K, VK_L, VK_M,
	VK_N, VK_O, VK_P,
	VK_Q, VK_R, VK_S,
	VK_T, VK_U, VK_V,
	VK_W, VK_X, VK_Y,
	VK_Z,
	VK_Num0 = 0x60, VK_Num1, VK_Num2, VK_Num3, VK_Num4,
	VK_Num5, VK_Num6, VK_Num7, VK_Num8, VK_Num9,
	VK_Multiply = 0x6A,
	VK_Add,
	VK_Subtract = 0x6D,
	VK_Decimal,
	VK_Devide,
	VK_Fn1 = 0x70, VK_Fn2, VK_Fn3, VK_Fn4, VK_Fn5, VK_Fn6,
	VK_Fn7, VK_Fn8, VK_Fn9, VK_Fn10, VK_Fn11, VK_Fn12,
	VK_Colon = 0xBA,
	VK_Plus,
	VK_Comma,
	VK_Minus,
	VK_Period,
	VK_Question,
	VK_Tilde,
	VK_LBracket = 0xDB,
	VK_Backslash,
	VK_RBracket,
	VK_Quotation,
};

/* 按键映射信息 */
struct FKeyBindInfo
{
	std::function<void()> func;
	EInputType type = EInputType::Pressed;
	int32 pressFlag = 0;
	float lastTime = 0;
};


/*----------------------------------
			  输入交互组件
  ----------------------------------*/
class InputComponent final: public ActorComponent
{
public:
	/**
	 * @brief 设置按键映射
	 * @param[in] mappingName			  事件名
	 * @param[in] value                   键码
	 **/
	void SetMapping(std::string mappingName, EKeyCode value);

	/**
	 * @brief 绑定按键事件映射
	 * @param[in] actionName		      事件名
	 * @param[in] type                    按键输入类型
	 * @param[in] obj	                  类函数所属对象指针
	 * @param[in] function                类函数指针
	 **/
	template<typename T>
	void BindAction(std::string actionName, EInputType type, T* obj, void(T::* func)())
	{
		if (mappings.find(actionName) != mappings.end())
		{
			actionCallbacks.insert({ actionName, { std::bind(func, obj),type,false } });
		}
	}

	/**
     * @brief 绑定按键事件映射
     * @param[in] actionName		      事件名
     * @param[in] type                    按键输入类型
     * @param[in] function                函数包装
     **/
	void BindAction(std::string actionName, EInputType type, std::function<void()>func);

	//获取当前按键码
	BYTE GetCurrentKeyCode();

	//是否有任何按键被按下
	static bool IsAnyKeyPressed();

	//某按键是否被按下
	static bool IsKeyPressed(EKeyCode keycode);

	//获取鼠标屏幕坐标
	static FVector2D GetMousePosition();
	
	//鼠标左键是否按下
	static bool IsMouseButtonPressed();

	//激活输入输出
	static void EnableInput(bool enable);

	//Action(行为事件，瞬时触发，触发频率受帧率影响)
	void PeekInfo();
	
	//Axis(轴事件，连续触发，触发频率恒定)
	void PeekInfo_();

	void MouseTick();

private:
	std::unordered_map<std::string, EKeyCode>mappings;
	std::unordered_map<std::string, FKeyBindInfo>actionCallbacks;
	std::unordered_map<std::string, FKeyBindInfo>axisCallbacks;

	static FVector2D mousePos;//鼠标在绘图窗口坐标
	static bool bActive;//是否启用交互
	ExMessage msg;
};
