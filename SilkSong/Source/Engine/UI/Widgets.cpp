#include "UserInterface.h"
#include "Tools/ResourceManager.h"
#include "GameplayStatics.h"
#include "Components/InputComponent.h"
#include "Objects/Controller.h"
#include "Tools/Math.h"




std::unordered_map<std::string, COLORREF>Characters::TextColorMap =
{
	{"$0",WHITE}/*白色*/,{"$1",RGB(245, 245, 245)}/*烟白色*/,
	{"$2",LIGHTGRAY}/*浅灰色*/,{"$3",DARKGRAY}/*深灰色*/, {"$4",BLACK}/*黑色*/,
	{"$5",GREEN}/*深绿色*/,{"$6",LIGHTGREEN}/*绿色*/, {"$7",YELLOW}/*黄色*/,
	{"$8",RGB(255, 165, 0)}/*橙色*/,{"$9",LIGHTRED}/*浅红色*/, {"$a",RED}/*深红色*/,
	{"$b",LIGHTMAGENTA}/*粉色*/,{"$c",MAGENTA}/*品红色*/, {"$d",RGB(148, 0, 211)}/*紫色*/,
	{"$e",BLUE}/*深蓝色*/,{"$f",LIGHTBLUE}/*海蓝色*/, {"$g",LIGHTCYAN}/*天蓝色*/,
	{"$h",CYAN}/*青色*/,{"$i",BROWN}/*褐色*/
};

void Characters::SetCharacters(std::string text, int size, LPCTSTR type)
{
	row = 1, column = 0;
	int temp = 0;
	for (int i = 0; i < text.length(); ++i)
	{
		if (text[i] == '\n') { row++,column = temp>column?temp:column,temp = 0; }
		else if (text[i] == '$' && (uint64)i + 1 < text.length())
		{
			std::string buf = "$" + text[i + 1];
			if (TextColorMap.find(buf) != TextColorMap.end())++i;
			else --temp;
		}
		else ++temp;
	}
	column = temp>column?temp:column;
	this->texts = text;
	this->size = size;
	this->type = type;
}

void Characters::PrintCharacters(Vector2D pos, CharactersPattern pattern)
{
	settextstyle(6*size,3*size,type);
	settextcolor(BLACK);
	int pat = 1;
	switch (pattern)
	{
	case CharactersPattern::Left:pat = 0; break;
	case CharactersPattern::Middle:pat = 1;break;
	case CharactersPattern::Right:pat = 2; break;
	default:pat = 1;break;
	}


	std::string temp;
	int r = 0;
	for (int i = 0; i < texts.length(); ++i)
	{
		if (texts[i] == '\n')
		{
			outtextxy((int)pos.x + (GetWidth() - int(temp.size()) * 3 * size) * pat / 2, (int)pos.y + r * 6 * size, temp.c_str());
			temp.clear(), ++r;
		}
		else if (texts[i] == '$' && (uint64)i+1 < texts.length())
		{
			std::string buf = "$"+ std::string(1, texts[++i]);
			if (TextColorMap.find(buf) != TextColorMap.end())
			{
				settextcolor(TextColorMap[buf]);
			}
			else temp.push_back(texts[i]);
		}
		else temp.push_back(texts[i]);
	}
	outtextxy((int)pos.x + (GetWidth() - int(temp.size()) * 3 * size) * pat / 2, (int)pos.y + r * 6 * size, temp.c_str());

	settextcolor(BLACK);
}







Vector2D Widget::GetLayoutOffset() const
{
	switch (layoutPattern)
	{
	case LayoutPattern::LeftTop:return Vector2D(-parent->GetSize().x / 2, -parent->GetSize().y / 2);
	case LayoutPattern::MiddleTop:return Vector2D(0, -parent->GetSize().y / 2);
	case LayoutPattern::RightTop:return Vector2D(parent->GetSize().x / 2, -parent->GetSize().y / 2);
	case LayoutPattern::LeftMiddle:return Vector2D(-parent->GetSize().x / 2, 0);
	case LayoutPattern::Center:return Vector2D(0, 0);
	case LayoutPattern::RightMiddle:return Vector2D(parent->GetSize().x / 2, 0);
	case LayoutPattern::LeftBottom:return Vector2D(-parent->GetSize().x / 2, parent->GetSize().y / 2);
	case LayoutPattern::MiddleBottom:return Vector2D(0, parent->GetSize().y / 2);
	case LayoutPattern::RightBottom:return Vector2D(parent->GetSize().x / 2, parent->GetSize().y / 2);
	default:return Vector2D(0, 0);
	}
	return Vector2D(0, 0);
}

bool Widget::IsUnderCursor() const
{
	float x = InputComponent::GetMousePosition().x;
	float y = InputComponent::GetMousePosition().y;

	int i = Math::Clamp((int)x / 200, 0, 5);
	int j = Math::Clamp((int)y / 200, 0, 3);

	for (auto it = mainWorld.UIDetectZones[i][j].rbegin(); it != mainWorld.UIDetectZones[i][j].rend(); ++it)
	{
		if (Widget* widget = Cast<Widget>(*it))
		{
			Vector2D loc = widget->GetScreenPosition();
			if (x < GetSize().x / 2 + loc.x && x > loc.x - GetSize().x / 2 
				&& y < GetSize().y / 2 + loc.y && y > loc.y - GetSize().y / 2 && widget == this)
			{
				return true;
			}
		}
	}
	return false;
}

Widget::~Widget()
{
	for (int i = point.x; i <= point_1.x; ++i)for (int j = point.y; j <= point_1.y; ++j)mainWorld.UIDetectZones[i][j].erase(this);
}

void Widget::Update()
{
	if (uiPattern != UIPattern::VisibleAndInteractive)return;

	Vector2D pos = GetScreenPosition() - GetSize()/2;
	Pair newPoint(Math::Clamp(int(pos.x) / 200, 0, 5), Math::Clamp(int(pos.y) / 200, 0, 3));
	pos += size;
	Pair newPoint_1(Math::Clamp(int(pos.x) / 200, 0, 5), Math::Clamp(int(pos.y) / 200, 0, 3));
	if (newPoint == point && newPoint_1 == point_1)return;

	if (point != Pair(-1, -1))for (int i = point.x; i <= point_1.x; ++i)for (int j = point.y; j <= point_1.y; ++j)mainWorld.UIDetectZones[i][j].erase(this);
	point = newPoint; point_1 = newPoint_1;
	for (int i = point.x; i <= point_1.x; ++i)for (int j = point.y; j <= point_1.y; ++j)mainWorld.UIDetectZones[i][j].insert(this);
}

void Widget::ShowInfoBox()
{
	if (bInfoBox && uiPattern == UIPattern::VisibleAndInteractive && IsUnderCursor())
	{
		setlinecolor(BLACK);
		setlinestyle(PS_SOLID | PS_JOIN_BEVEL);
		setfillcolor(RGB(255, 247, 213));

		Vector2D pos = InputComponent::GetMousePosition() + Vector2D(-15, 15);
		fillrectangle((int)pos.x, (int)pos.y, (int)pos.x + InfoText.GetWidth(), (int)pos.y + InfoText.GetHeight());
		InfoText.PrintCharacters(pos);
	}
}

void Widget::DrawDebugRect()
{
	setlinecolor(BLUE);
	Vector2D pos = GetScreenPosition() - GetSize() / 2;
	int left = int(pos.x), top = int(pos.y);
	pos += GetSize();
	int right = int(pos.x), bottom = int(pos.y);
	rectangle(left, top, right, bottom);
}

void Widget::SetUIPattern(UIPattern pattern)
{
	uiPattern = pattern;
	for (auto& child : children)
	{
		child->SetUIPattern(pattern);
	}
	if (uiPattern != UIPattern::VisibleAndInteractive)
	{
		if (point != Pair(-1, -1))for (int i = point.x; i <= point_1.x; ++i)for (int j = point.y; j <= point_1.y; ++j)mainWorld.UIDetectZones[i][j].erase(this);
		point = { -1, -1 }, point_1 = { -1, -1 };
	}
}

Vector2D Widget::GetSize() const
{
	return size * GetScreenScale();
}

Vector2D Widget::GetScreenPosition() const
{
	if (parent)return parent->GetScreenPosition() + GetRelativePosition() + GetLayoutOffset();
	else return GetRelativePosition(); 
}

float Widget::GetScreenRotation() const
{
	if (parent)return parent->GetScreenRotation() + GetRelativeRotation();
	else return GetRelativeRotation();
}

Vector2D Widget::GetScreenScale() const
{
	if (parent)return parent->GetScreenScale() * GetRelativeScale();
	else return GetRelativeScale();
}

void Widget::AttachTo(Widget* par)
{
	if (par) 
	{
		par->children.insert(this);
		parent = par;
	}
}

void Widget::DetachFrom(Widget* par)
{
	if (par) 
	{
		par->children.erase(this);
		parent = nullptr;
	}
}







Panel::~Panel()
{
	if(members_ui.empty())return;
	for (auto &ui:members_ui)
	{
		ui->RemoveFromViewport();
	}
}

void Panel::Update()
{
	Widget::Update();
	if (members.empty())return;
	int32 index = 0;
	for (auto& member : members)
	{
		AdjustMemberPosition(member,index++);
	}
}

void Panel::SetUnitSize(Vector2D size)
{
	unitSize = size;
	Panel::Update();
}

void Panel::AddMember(Widget* member, int32 index)
{
	member->AttachTo(this);
	if (index >= 0)members.emplace(members.begin() + index, member);
	else members.push_back(member);
	member->attachedPanel = this;
	AdjustMemberSizeToUnit(member);
	AdjustMemberPosition(member, index >= 0 ? index : members.size() - 1);
}
void Panel::RemoveMember(Widget* member)
{
	member->DetachFrom(this);
	members.erase(std::remove(members.begin(), members.end(), member), members.end());
	member->attachedPanel = nullptr;
}
void Panel::AddMember(UserInterface* member, int32 index)
{
	member->rootCanvas->AttachTo(this);
	index = Math::Clamp(index,-1,int32(members.size()));
	if (index >= 0)members.emplace(members.begin()+index, member->rootCanvas);
	else members.push_back(member->rootCanvas);
	members_ui.push_back(member);
	member->rootCanvas->attachedPanel = this;
	AdjustMemberSizeToUnit(member->rootCanvas);
	AdjustMemberPosition(member->rootCanvas, index >= 0 ? index : members.size() - 1);
}
void Panel::RemoveMember(UserInterface* member)
{
	member->rootCanvas->DetachFrom(this);
	members.erase(std::remove(members.begin(), members.end(), member->rootCanvas), members.end());
	members_ui.erase(std::remove(members_ui.begin(), members_ui.end(), member), members_ui.end());
	member->rootCanvas->attachedPanel = nullptr;
}

void Panel::AdjustMemberSizeToUnit(Widget* member)
{
	member->SetRelativeScale(unitSize / member->GetSize());
}

void HorizontalPanel::AdjustMemberPosition(Widget* member, int32 index)
{
	if(index<0)return;
	Vector2D pos = Vector2D(index * (unitSize.x + spacing), 0) + Vector2D(unitSize.x, unitSize.y)*0.5f;
	member->SetRelativePosition(pos);
}

Vector2D HorizontalPanel::GetSize() const
{
	return members.empty() ? Vector2D() : Vector2D(members.size() * (unitSize.x + spacing) - spacing, unitSize.y);
}

void VerticalPanel::AdjustMemberPosition(Widget* member, int32 index)
{
	if (index<0)return;
	Vector2D pos = Vector2D(0, index * (unitSize.y + spacing));
	member->SetRelativePosition(pos);
}

Vector2D VerticalPanel::GetSize() const
{
	return members.empty() ? Vector2D() : Vector2D(unitSize.x, members.size() * (unitSize.y + spacing) - spacing);
}

void GridPanel::AdjustMemberPosition(Widget* member, int32 index)
{
	if (index<0)return;
	Vector2D pos = Vector2D((index % column) * (unitSize.x + spacingX), (index / column) * (unitSize.y + spacingY));
	member->SetRelativePosition(pos);
}

Vector2D GridPanel::GetSize() const
{
	return members.empty() ? Vector2D() : Vector2D(column * (unitSize.x + spacingX) - spacingX, row * (unitSize.y + spacingY) - spacingY);
}






void Text::Update()
{
	Widget::Update();
	if (bindedText)texts.SetCharacters(*bindedText);
	size = Vector2D(float(texts.GetWidth()), float(texts.GetHeight()));
}

void Text::Render()
{
	texts.PrintCharacters(GetScreenPosition()-size*0.5f, textPattern);
}








void Image::Update()
{
	Widget::Update();
	if (!sprite)return;

	if (copy)spriteInfo.size = Pair(copy->getwidth(), copy->getheight());
	else spriteInfo.size = Pair(sprite->getwidth(), sprite->getheight());

	if (GetScreenRotation() != angle)
	{
		angle = GetScreenRotation();
		RotateImage(PI * Math::NormalizeDegree(angle) / 180);
		if (filterLayers.size() > 0)FilterImage();
	}
}

void Image::Render()
{
	if (uiPattern == UIPattern::None || alpha == 0)return;
	if (!sprite)return;

	Vector2D pos = GetScreenPosition();
	HDC dstDC = GetImageHDC();

	IMAGE* img = copy ? copy : sprite;
	HDC srcDC = GetImageHDC(img);
	int w = spriteInfo.endLoc.x - spriteInfo.startLoc.x;
	int h = spriteInfo.endLoc.y - spriteInfo.startLoc.y;

	int dw = int(GetSize().x * w / img->getwidth());
	int dh = int(GetSize().y * h / img->getheight());

	BLENDFUNCTION bf = { AC_SRC_OVER,0,alpha,AC_SRC_ALPHA };
	if (filterLayers.size() > 0 && filter)srcDC = GetImageHDC(filter);
	AlphaBlend(dstDC, (int)pos.x - GetSize().x / 2, (int)pos.y - GetSize().y / 2, dw, dh,
		srcDC,(int)spriteInfo.startLoc.x, (int)spriteInfo.startLoc.y, w, h, bf);
}

bool Image::IsMouseOn()
{
	return IsUnderCursor();
}







void Button::Update()
{
	Image::Update();

	if (!isHover && IsMouseOn()) { OnMouseHoverBegin.BroadCast(); isHover = true; }
	else if (isHover && !IsMouseOn()) { OnMouseHoverEnd.BroadCast(); isHover = false; }

	if (!isPressed && IsMousePressed()) {OnMousePressedBegin.BroadCast(); isPressed = true; }
	else if (isPressed &&!IsMousePressed()) {OnMousePressedEnd.BroadCast(); isPressed = false; }


	if (!hover && !pressed) {sprite = normal; return;}

	if (!isHover) { sprite = normal; spriteInfo.endLoc = { normal->getwidth(), normal->getheight() }; }
	else if (!isPressed) { sprite = hover; spriteInfo.endLoc = { hover->getwidth(), hover->getheight() }; }
	else {sprite = pressed; spriteInfo.endLoc = { pressed->getwidth(), pressed->getheight() };}
}

void Button::LoadNormalPicture(std::string path)
{
	normal = mainWorld.resourcePool->Fetch(path);
	if (!normal)return;
	spriteInfo.endLoc = { normal->getwidth(), normal->getheight() };
}

void Button::LoadHoverPicture(std::string path)
{
	hover = mainWorld.resourcePool->Fetch(path);
	if (!hover)return;
	spriteInfo.endLoc = { hover->getwidth(), hover->getheight() };
}

void Button::LoadClickPicture(std::string path)
{
	pressed = mainWorld.resourcePool->Fetch(path);
	if (!pressed)return;
	spriteInfo.endLoc = { pressed->getwidth(), pressed->getheight() };
}

bool Button::IsMousePressed()
{
	return IsMouseOn() && GameplayStatics::GetController()->IsMouseClicked();
}






void Bar::Update()
{
	Widget::Update();
}

void Bar::Render()
{
	if (uiPattern == UIPattern::None)return;
	Vector2D pos = GetScreenPosition();
	HDC dstDC = GetImageHDC();
	BLENDFUNCTION bf = { AC_SRC_OVER,0,255,AC_SRC_ALPHA };

	if (barBack) 
	{
		HDC srcDC = GetImageHDC(barBack);
		int w = barBack->getwidth();
		int h = barBack->getheight();
		AlphaBlend(dstDC, (int)pos.x - sizeBack.x / 2, (int)pos.y - sizeBack.y / 2, sizeBack.x, sizeBack.y, srcDC, 0, 0, w, h, bf);
	}

	if (barFront)
	{
		HDC srcDC = GetImageHDC(barFront);

		Pair startPosition,endPosition;

		switch (direction)
		{
		case BarDirection::RightToLeft: startPosition = { 0,0 }; endPosition = { (int)(sizeFront.x * percentage),sizeFront.y }; break;
		case BarDirection::LeftToRight: startPosition = { (int)(sizeFront.x * (1 - percentage)),0 }; endPosition = { sizeFront.x,sizeFront.y }; break;
		case BarDirection::TopToBottom: startPosition = { 0,(int)(sizeFront.y * (1 - percentage)) }; endPosition = { sizeFront.x,sizeFront.y}; break;
		case BarDirection::BottomToTop: startPosition = { 0,0 }; endPosition = { sizeFront.x,(int)(sizeFront.y * percentage), }; break;
		default: startPosition = { 0,0 }; endPosition = { 0,0 }; break;
		}

		int w = endPosition.x - startPosition.x;
		int h = endPosition.y - startPosition.y;
		AlphaBlend(dstDC, (int)pos.x - sizeFront.x / 2, (int)pos.y - sizeFront.y / 2, sizeFront.x, sizeFront.y, srcDC, 0, 0, w, h, bf);

		if (barButton)
		{
			AlphaBlend(dstDC, endPosition.x - sizeButton.x / 2, endPosition.y - sizeButton.y / 2, sizeButton.x, sizeButton.y, 
				GetImageHDC(barButton), 0, 0, barButton->getwidth(), barButton->getheight(), bf);
		}
	}

}

void Bar::LoadBarFrontPicture(std::string path)
{
	barFront = mainWorld.resourcePool->Fetch(path);
	SetFrontSize(Pair(barFront->getwidth(), barFront->getheight()));
}

void Bar::LoadBarBackPicture(std::string path)
{
	barBack = mainWorld.resourcePool->Fetch(path);
	SetBackSize(Pair(barBack->getwidth(), barBack->getheight()));
}

void Bar::LoadBarButtonPicture(std::string path)
{
	barButton = mainWorld.resourcePool->Fetch(path);
	SetButtonSize(Pair(barButton->getwidth(), barButton->getheight()));
}
