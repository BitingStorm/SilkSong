#include "ButtonIconUI.h"

std::unordered_map<uint8, std::string>ButtonIconUI::IconStringMap =
{
	{ 0x01,"button_skin_mouse_l" },
	{ 0x02,"button_skin_mouse_r" },
	{ 0x04,"button_skin_mouse_m" },
	{ 0x25,"button_skin_arrow_l" },
	{ 0x26,"button_skin_arrow_u" },
	{ 0x27,"button_skin_arrow_r" },
	{ 0x28,"button_skin_arrow_d" },
	{ 0x08,"Backspace" },
	{ 0x09,"Tab" },
	{ 0x0D,"Enter" },
	{ 0x10,"Shift" },
	{ 0x11,"Ctrl" },
	{ 0x12,"Alt" },
	{ 0x20,"Space" },
	{ 0x2E,"Delete" },
	{ 0x60,"Num0" },{ 0x61,"Num1" },{ 0x62,"Num2" },{ 0x63,"Num3" },{ 0x64,"Num4" },
	{ 0x65,"Num5" },{ 0x66,"Num6" },{ 0x67,"Num7" },{ 0x68,"Num8" },{ 0x69,"Num9" },
	{ 0x70,"F1" },{ 0x71,"F2" },{ 0x72,"F3" },{ 0x73,"F4" },{ 0x74,"F5" },{ 0x75,"F6" },
	{ 0x76,"F7" },{ 0x77,"F8" },{ 0x78,"F9" },{ 0x79,"F10" },{ 0x7A,"F11" },{ 0x7B,"F12" },
};

std::unordered_map<uint8, char>ButtonIconUI::IconCharMap =
{
	{ 0x6A,'*'},
	{ 0x6B,'+'},
	{ 0x6D,'-'},
	{ 0x6E,'.'},
	{ 0x6F,'/'},
	{ 0xBA,':'},
	{ 0xBB,'='},
	{ 0xBC,'<'},
	{ 0xBD,'_'},
	{ 0xBE,'>'},
	{ 0xBF,'?'},
	{ 0xC0,'~'},
	{ 0xDB,'{'},
	{ 0xDC,'\\'},
	{ 0xDD,'}'},
	{ 0xDE,'"'},
};

ButtonIconUI::ButtonIconUI()
{
	Img = AddWidget<Image>();
	Img->AttachTo(rootCanvas);
	Img->SetLayoutPattern(LayoutPattern::Center);
	Img->SetLayer(30);
}

bool ButtonIconUI::IsStringLegal(uint8 keyCode)
{
	return IconStringMap.find(keyCode) != IconStringMap.end();
}

bool ButtonIconUI::IsCharLegal(uint8 keyCode)
{
	return IconCharMap.find(keyCode) != IconCharMap.end();
}


ButtonIconCharUI::ButtonIconCharUI()
{
	Char = AddWidget<Text>();
	Char->AttachTo(Img);
	Char->SetLayoutPattern(LayoutPattern::Center);
	Char->SetLayer(30);
	flag = false;
}

void ButtonIconCharUI::SetIcon(uint8 keyCode)
{
	Img->LoadSprite("button_skin_square");
	char ch = char(keyCode);
	if (flag)
	{
		ch = IconCharMap[keyCode];
	}
	Char->SetText(std::string(1, ch), 5);
}

ButtonIconStringUI::ButtonIconStringUI()
{
	String = AddWidget<Text>();
	String->AttachTo(Img);
	String->SetLayoutPattern(LayoutPattern::Center);
	String->SetLayer(30);
}

void ButtonIconStringUI::SetIcon(uint8 keyCode)
{
	Img->LoadSprite("button_skin_rectangle");
	String->SetText(IconStringMap[keyCode], 3);
}


void ButtonIconImageUI::SetIcon(uint8 keyCode)
{
	Img->LoadSprite(IconStringMap[keyCode]);
}
