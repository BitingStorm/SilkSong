#include "Widgets.h"
#include "UserInterface.h"
#include "Tools/ResourceManager.h"
#include "GameplayStatics.h"
#include "Objects/Controller.h"
#include "Components/Animator.h"



std::unordered_map<std::string, COLORREF>ArtyEngine::Characters::TextColorMap =
{
	{"$0",WHITE}/*白色*/,{"$1",RGB(245, 245, 245)}/*烟白色*/,
	{"$2",LIGHTGRAY}/*浅灰色*/,{"$3",DARKGRAY}/*深灰色*/, {"$4",BLACK}/*黑色*/,
	{"$5",GREEN}/*深绿色*/,{"$6",LIGHTGREEN}/*绿色*/, {"$7",YELLOW}/*黄色*/,
	{"$8",RGB(255, 165, 0)}/*橙色*/,{"$9",LIGHTRED}/*浅红色*/, {"$a",RED}/*深红色*/,
	{"$b",LIGHTMAGENTA}/*粉色*/,{"$c",MAGENTA}/*品红色*/, {"$d",RGB(148, 0, 211)}/*紫色*/,
	{"$e",BLUE}/*深蓝色*/,{"$f",LIGHTBLUE}/*海蓝色*/, {"$g",LIGHTCYAN}/*天蓝色*/,
	{"$h",CYAN}/*青色*/,{"$i",BROWN}/*褐色*/
};

void ArtyEngine::Characters::SetCharacters(std::string text, int size, LPCTSTR style)
{
	row = 1;
	maxWidth = 0;
	std::string temp;
	for (int i = 0; i < text.length(); ++i)
	{
		if (text[i] == '\n') 
		{ 
			++row;
			int width = textwidth(temp.c_str());
			maxWidth = width > maxWidth ? width : maxWidth;
			temp.clear();
		}
		else if (text[i] == '$' && (uint64)i + 1 < text.length())
		{
			std::string buf = "$" + text[i + 1];
			if (TextColorMap.find(buf) != TextColorMap.end())++i;
			else temp.push_back(text[i]);
		}
		else temp.push_back(text[i]);
	}
	int width = textwidth(temp.c_str());
	maxWidth = width > maxWidth ? width : maxWidth;

	this->texts = text;
	this->size = size;
	this->style = style;
}

void ArtyEngine::Characters::PrintCharacters(FVector2D pos, BYTE alpha, CharactersPattern pattern)
{
	int pat = 1;
	switch (pattern)
	{
	case CharactersPattern::Left:pat = 0; break;
	case CharactersPattern::Middle:pat = 1; break;
	case CharactersPattern::Right:pat = 2; break;
	default:pat = 1;break;
	}

	COLORREF color = WHITE;

	std::string temp;
	int r = 0;
	for (int i = 0; i < texts.length(); ++i)
	{
		if (texts[i] == '\n')
		{
			settextstyle(6 * size, 3 * size, style);
			ImageToolkit::OutText((int)pos.x + (GetWidth() - textwidth(temp.c_str())) * pat / 2, (int)pos.y + r * 6 * size, temp.c_str(), alpha, color, size, style);
			temp.clear(), ++r;
		}
		else if (texts[i] == '$' && (uint64)i + 1 < texts.length())
		{
			std::string buf = "$" + std::string(1, texts[++i]);
			if (TextColorMap.find(buf) != TextColorMap.end())
			{
				color = TextColorMap[buf];
			}
			else temp.push_back(texts[i]);
		}
		else
		{
			temp.push_back(texts[i]);
		}
	}
	settextstyle(6 * size, 3 * size, style);
	ImageToolkit::OutText((int)pos.x + (GetWidth() - textwidth(temp.c_str())) * pat / 2, (int)pos.y + r * 6 * size, temp.c_str(), alpha, color, size, style);
}






FVector2D Widget::GetLayoutOffset() const
{
	switch (layoutPattern)
	{
	case LayoutPattern::LeftTop:return FVector2D(-parent->GetSize().x / 2, -parent->GetSize().y / 2);
	case LayoutPattern::MiddleTop:return FVector2D(0, -parent->GetSize().y / 2);
	case LayoutPattern::RightTop:return FVector2D(parent->GetSize().x / 2, -parent->GetSize().y / 2);
	case LayoutPattern::LeftMiddle:return FVector2D(-parent->GetSize().x / 2, 0);
	case LayoutPattern::Center:return FVector2D(0, 0);
	case LayoutPattern::RightMiddle:return FVector2D(parent->GetSize().x / 2, 0);
	case LayoutPattern::LeftBottom:return FVector2D(-parent->GetSize().x / 2, parent->GetSize().y / 2);
	case LayoutPattern::MiddleBottom:return FVector2D(0, parent->GetSize().y / 2);
	case LayoutPattern::RightBottom:return FVector2D(parent->GetSize().x / 2, parent->GetSize().y / 2);
	default:return FVector2D(0, 0);
	}
	return FVector2D(0, 0);
}

bool Widget::IsUnderCursor() const
{
	float x = InputComponent::GetMousePosition().x;
	float y = InputComponent::GetMousePosition().y;

	int i = FMath::Clamp((int)x / 200, 0, 5);
	int j = FMath::Clamp((int)y / 200, 0, 3);

	for (auto it = mainWorld.UIDetectZones[i][j].rbegin(); it != mainWorld.UIDetectZones[i][j].rend(); ++it)
	{
		if (Widget* widget = Cast<Widget>(*it))
		{
			FVector2D loc = widget->GetScreenPosition();
			FBox2D box(loc, GetSize().x, GetSize().y);
			if (box.IsInside(InputComponent::GetMousePosition()) && widget == this)
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

	FVector2D pos = GetScreenPosition() - GetSize() / 2;
	FIntVector2 newPoint(FMath::Clamp(int(pos.x) / 200, 0, 5), FMath::Clamp(int(pos.y) / 200, 0, 3));
	pos += size;
	FIntVector2 newPoint_1(FMath::Clamp(int(pos.x) / 200, 0, 5), FMath::Clamp(int(pos.y) / 200, 0, 3));
	if (newPoint == point && newPoint_1 == point_1)return;

	if (point != FIntVector2(-1, -1))for (int i = point.x; i <= point_1.x; ++i)for (int j = point.y; j <= point_1.y; ++j)mainWorld.UIDetectZones[i][j].erase(this);
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

		FVector2D pos = InputComponent::GetMousePosition() + FVector2D(-15, 15);
		fillrectangle((int)pos.x, (int)pos.y, (int)pos.x + InfoText.GetWidth(), (int)pos.y + InfoText.GetHeight());
		InfoText.PrintCharacters(pos);
	}
}

void Widget::DrawDebugRect()
{
	setlinecolor(BLUE);
	FVector2D pos = GetScreenPosition() - GetSize() / 2;
	int left = int(pos.x), top = int(pos.y);
	pos += GetSize();
	int right = int(pos.x), bottom = int(pos.y);
	rectangle(left, top, right, bottom);
}

void Widget::SetUIPattern(UIPattern pattern)
{
	if (uiPattern == pattern)
	{
		return;
	}
	uiPattern = pattern;
	for (auto& child : children)
	{
		child->SetUIPattern(pattern);
	}
	if (uiPattern != UIPattern::VisibleAndInteractive)
	{
		if (point != FIntVector2(-1, -1))for (int i = point.x; i <= point_1.x; ++i)for (int j = point.y; j <= point_1.y; ++j)mainWorld.UIDetectZones[i][j].erase(this);
		point = { -1, -1 }, point_1 = { -1, -1 };
	}
}

FVector2D Widget::GetSize() const
{
	return size * GetScreenScale();
}

FVector2D Widget::GetScreenPosition() const
{
	if (parent)return parent->GetScreenPosition() + GetRelativePosition() + GetLayoutOffset();
	else return GetRelativePosition(); 
}

float Widget::GetScreenRotation() const
{
	if (parent)return parent->GetScreenRotation() + GetRelativeRotation();
	else return GetRelativeRotation();
}

FVector2D Widget::GetScreenScale() const
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

void Panel::SetUnitSize(FVector2D size)
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
	AdjustMemberPosition(member, index >= 0 ? index : int32(members.size()) - 1);
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
	index = FMath::Clamp(index,-1,int32(members.size()));
	if (index >= 0)members.emplace(members.begin()+index, member->rootCanvas);
	else members.push_back(member->rootCanvas);
	members_ui.push_back(member);
	member->rootCanvas->attachedPanel = this;
	AdjustMemberSizeToUnit(member->rootCanvas);
	AdjustMemberPosition(member->rootCanvas, index >= 0 ? index : int32(members.size()) - 1);
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
	if (index < 0)return;
	FVector2D pos = FVector2D(index * (unitSize.x + spacing), 0) + FVector2D(unitSize.x, unitSize.y)*0.5f;
	member->SetRelativePosition(pos);
}

FVector2D HorizontalPanel::GetSize() const
{
	return members.empty() ? FVector2D() : FVector2D(members.size() * (unitSize.x + spacing) - spacing, unitSize.y);
}

void VerticalPanel::AdjustMemberPosition(Widget* member, int32 index)
{
	if (index < 0)return;
	FVector2D pos = FVector2D(0, index * (unitSize.y + spacing));
	member->SetRelativePosition(pos);
}

FVector2D VerticalPanel::GetSize() const
{
	return members.empty() ? FVector2D() : FVector2D(unitSize.x, members.size() * (unitSize.y + spacing) - spacing);
}

void GridPanel::AdjustMemberPosition(Widget* member, int32 index)
{
	if (index < 0)return;
	FVector2D pos = FVector2D((index % column) * (unitSize.x + spacingX), (index / column) * (unitSize.y + spacingY));
	member->SetRelativePosition(pos);
}

FVector2D GridPanel::GetSize() const
{
	return members.empty() ? FVector2D() : FVector2D(column * (unitSize.x + spacingX) - spacingX, row * (unitSize.y + spacingY) - spacingY);
}






void Text::Update()
{
	Widget::Update();
	if (bindedText)texts.SetCharacters(*bindedText);
	size = FVector2D(float(texts.GetWidth()), float(texts.GetHeight()));
}

void Text::Render()
{
	if (uiPattern == UIPattern::None || alpha == 0)return;
	texts.PrintCharacters(GetScreenPosition() - size * 0.5f, alpha, textPattern);
}

void Text::SetAlpha(BYTE alpha)
{
	alpha = FMath::Clamp(alpha, BYTE(0), BYTE(255)); this->alpha = alpha;
}




Image::~Image()
{
	if (ani)delete ani;
}

void Image::DealImage()
{
	if (GetScreenRotation() != 0)
	{
		RotateImage(angle);
	}
	if (filterLayers.size() > 0)
	{
		FilterImage();
	}
}

void Image::Update()
{
	Widget::Update();

	if(ani)ani->Update(0);

	if (!sprite)return;

	if (copy)spriteInfo.size = FIntVector2(copy->getwidth(), copy->getheight());
	else spriteInfo.size = FIntVector2(sprite->getwidth(), sprite->getheight());

	if (GetScreenRotation() != angle)
	{
		angle = GetScreenRotation();
		RotateImage(angle);
		if (filterLayers.size() > 0)
		{
			FilterImage();
		}
	}
}

void Image::Render()
{
	if (uiPattern == UIPattern::None || alpha == 0)return;
	if (!sprite)return;

	FVector2D pos = GetScreenPosition();
	HDC dstDC = GetImageHDC();

	IMAGE* img = copy ? copy : sprite;
	HDC srcDC = GetImageHDC(img);
	int w = spriteInfo.endLoc.x - spriteInfo.startLoc.x;
	int h = spriteInfo.endLoc.y - spriteInfo.startLoc.y;

	BLENDFUNCTION bf = { AC_SRC_OVER,0,alpha,AC_SRC_ALPHA };
	if (filterLayers.size() > 0 && filter)srcDC = GetImageHDC(filter);

	if (ani)
	{
		int dw = int(GetScreenScale().x * w);
		int dh = int(GetScreenScale().y * h);
		AlphaBlend(dstDC, int(pos.x - dw * 0.5f), int(pos.y - dh * 0.5f), dw, dh,
			srcDC, (int)spriteInfo.startLoc.x, (int)spriteInfo.startLoc.y, w, h, bf);
	}
	else
	{
		int dw = int(GetSize().x * w / img->getwidth());
		int dh = int(GetSize().y * h / img->getheight());
		AlphaBlend(dstDC, int(pos.x - GetSize().x * 0.5f), int(pos.y - GetSize().y * 0.5f), dw, dh,
			srcDC, (int)spriteInfo.startLoc.x, (int)spriteInfo.startLoc.y, w, h, bf);
	}
}

bool Image::IsMouseOn()
{
	return IsUnderCursor();
}

IMAGE* Image::LoadSprite(std::string name)
{
	IMAGE* spr = ImageInterface::LoadSprite(name);
	if (spr && size == FVector2D::ZeroVector)
	{
		size = FVector2D(float(spr->getwidth()), float(spr->getheight()));
	}
	return spr;
}

void Image::EnableAnimControl()
{
	if (ani)
	{
		return;
	}
	ani = new Animator;
	ani->SetupAttachment(this);
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
	FVector2D pos = GetScreenPosition();
	HDC dstDC = GetImageHDC();
	BLENDFUNCTION bf = { AC_SRC_OVER,0,255,AC_SRC_ALPHA };

	if (barBack)
	{
		HDC srcDC = GetImageHDC(barBack);
		int w = barBack->getwidth();
		int h = barBack->getheight();
		AlphaBlend(dstDC, int(pos.x - sizeBack.x * 0.5f), int(pos.y - sizeBack.y * 0.5f), sizeBack.x, sizeBack.y, srcDC, 0, 0, w, h, bf);
	}

	if (barFront)
	{
		HDC srcDC = GetImageHDC(barFront);

		FIntVector2 startPosition, endPosition;

		switch (direction)
		{
		case BarDirection::RightToLeft: startPosition = { 0,0 }; endPosition = { int(sizeFront.x * percentage),sizeFront.y }; break;
		case BarDirection::LeftToRight: startPosition = { int(sizeFront.x * (1.f - percentage)),0 }; endPosition = { sizeFront.x,sizeFront.y }; break;
		case BarDirection::TopToBottom: startPosition = { 0,int(sizeFront.y * (1.f - percentage)) }; endPosition = { sizeFront.x,sizeFront.y }; break;
		case BarDirection::BottomToTop: startPosition = { 0,0 }; endPosition = { sizeFront.x,int(sizeFront.y * percentage), }; break;
		default: startPosition = { 0,0 }; endPosition = { 0,0 }; break;
		}

		int w = endPosition.x - startPosition.x;
		int h = endPosition.y - startPosition.y;
		AlphaBlend(dstDC, int(pos.x - sizeFront.x * 0.5f), int(pos.y - sizeFront.y * 0.5f), w, h, srcDC, 0, 0,
			float(barFront->getwidth() * w) / sizeFront.x, float(barFront->getheight() * h) / sizeFront.y, bf);

		if (barButton)
		{
			AlphaBlend(dstDC, int(pos.x - sizeFront.x * 0.5f + endPosition.x - sizeButton.x * 0.5f), 
				int(pos.y - sizeFront.y * 0.5f - sizeButton.y * 0.5f), sizeButton.x, sizeButton.y,
				GetImageHDC(barButton), 0, 0, barButton->getwidth(), barButton->getheight(), bf);
		}
	}
}

void Bar::LoadBarFrontPicture(std::string path)
{
	barFront = mainWorld.resourcePool->Fetch(path);
	SetFrontSize(FIntVector2(barFront->getwidth(), barFront->getheight()));
}

void Bar::LoadBarBackPicture(std::string path)
{
	barBack = mainWorld.resourcePool->Fetch(path);
	SetBackSize(FIntVector2(barBack->getwidth(), barBack->getheight()));
}

void Bar::LoadBarButtonPicture(std::string path)
{
	barButton = mainWorld.resourcePool->Fetch(path);
	SetButtonSize(FIntVector2(barButton->getwidth(), barButton->getheight()));
}

void Bar::SetPercentage(float per)
{ 
	percentage = FMath::Clamp(per, 0.f, 1.f);
}






void Sector::Update()
{
	Widget::Update();
}

void Sector::Render()
{
	if (uiPattern == UIPattern::None)return;
	FVector2D pos = GetScreenPosition();
	HDC dstDC = GetImageHDC();
	BLENDFUNCTION bf = { AC_SRC_OVER,0,255,AC_SRC_ALPHA };

	if (sectorBack)
	{
		HDC srcDC = GetImageHDC(sectorBack);
		int w = sectorBack->getwidth();
		int h = sectorBack->getheight();
		AlphaBlend(dstDC, (int)pos.x - sizeBack.x / 2, (int)pos.y - sizeBack.y / 2, sizeBack.x, sizeBack.y, srcDC, 0, 0, w, h, bf);
	}

	if (sectorFront)
	{
		IMAGE* dest{};
		dest = new IMAGE(sectorFront->getwidth(), sectorFront->getheight());
		ImageToolkit::GetSectorImage(sectorFront, dest, startDegree, startDegree + 360 * percentage);
		int w = sectorFront->getwidth();
		int h = sectorFront->getheight();
		HDC srcDC = GetImageHDC(dest);
		AlphaBlend(dstDC, (int)pos.x - sizeFront.x / 2, (int)pos.y - sizeFront.y / 2, sizeFront.x, sizeFront.y, srcDC, 0, 0, w, h, bf);
		delete dest;
	}
}

void Sector::LoadSectorFrontPicture(std::string path)
{
	sectorFront = mainWorld.resourcePool->Fetch(path);
	SetFrontSize(FIntVector2(sectorFront->getwidth(), sectorFront->getheight()));
}

void Sector::LoadSectorBackPicture(std::string path)
{
	sectorBack = mainWorld.resourcePool->Fetch(path);
	SetBackSize(FIntVector2(sectorBack->getwidth(), sectorBack->getheight()));
}

void Sector::SetPercentage(float per)
{
	percentage = FMath::Clamp(per, 0.f, 1.f);
}

void Sector::SetStartDegree(float start)
{
	startDegree = FMath::NormalizeDegree(start);
}
