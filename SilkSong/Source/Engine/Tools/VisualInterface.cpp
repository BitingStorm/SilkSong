#include "VisualInterface.h"
#include "ResourceManager.h"
#include "Core/World.h"
#include "Tools/FileManager.h"
#include "Components/Animator.h"



LayerInterface::LayerInterface()
{
	mainWorld.GameRenderers.insert(this);
}

LayerInterface::~LayerInterface()
{
	mainWorld.GameRenderers.erase(this);
}

void LayerInterface::SetLayer(int layer)
{
	mainWorld.GameRenderers.erase(this);
	this->layer = layer;
	mainWorld.GameRenderers.insert(this);
}

int LayerInterface::GetLayer()const
{
	return layer;
}





ImageInterface::~ImageInterface()
{
	if (copy)delete copy; if (filter)delete filter; if (blur)delete blur;
	if (animatorAttached)animatorAttached->rendererAttached = nullptr;
}

void ImageInterface::RotateImage(float degree)
{			
	if (!copy)copy = new IMAGE;
	spriteInfo.endLoc = ImageToolkit::RotateImage(sprite, copy, degree);
}

void ImageInterface::FilterImage()
{
	if (!sprite)return;

	IMAGE* img = (copy && angle) ? copy : sprite;
	if (!filter)filter = new IMAGE(img->getwidth(), img->getheight());
	filter->Resize(img->getwidth(), img->getheight());

	if (!filter) return;

	const DWORD* pBuf = GetImageBuffer(img);
	DWORD* pNewBuf = GetImageBuffer(filter);
	int num = img->getheight() * img->getwidth();

	if (!pBuf || !pNewBuf || num == 0) return;

	for (int i = 0; i < num; ++i)
	{
		int32 alpha = pBuf[i] >> 24;
		if (alpha)
		{
			// 获取BGR
			uint32 pNewBufB = pBuf[i] & 0xFF;
			uint32 pNewBufG = (pBuf[i] & 0xFF00) >> 8;
			uint32 pNewBufR = (pBuf[i] & 0xFF0000) >> 16;

			// 将颜色值进行平均化
			for (auto& filterInfo : filterLayers)
			{
				int32 level = filterInfo.level;
				if (alpha < 255)level = alpha * level >> 8;//使得颜色滤镜从低透明度到高透明度平滑过渡
				pNewBufB = (pNewBufB * (128 - level) + level * GetBValue(filterInfo.color)) >> 7;
				pNewBufG = (pNewBufG * (128 - level) + level * GetGValue(filterInfo.color)) >> 7;
				pNewBufR = (pNewBufR * (128 - level) + level * GetRValue(filterInfo.color)) >> 7;
			}
			// 重新组合成一个新的颜色值（BGR）
			pNewBuf[i] = (pBuf[i] & 0xFF000000) | (pNewBufR << 16) | (pNewBufG << 8) | pNewBufB;
		}
		else pNewBuf[i] = 0;
	}

}

void ImageInterface::AddFilter(ArtyEngine::FilterInfo filterInfo)
{
	filterLayers.insert(filterInfo);
	FilterImage();
}

void ImageInterface::RemoveFilter()
{
	filterLayers.erase({ BLACK,60,1 });
}

void ImageInterface::SetFilter(bool enable, COLORREF col, int32 level)
{
	filterLayers.erase(ArtyEngine::FilterInfo{ col, level });
	if (enable)
	{
		level = FMath::Clamp(level, 0, 100);
		filterLayers.insert(ArtyEngine::FilterInfo{ col, level });
		FilterImage();
	}
}

void ImageInterface::MeanBlur(unsigned level)
{
	if (!sprite || blur || level == 0)return;
	blur = new IMAGE(sprite->getwidth(), sprite->getheight());
	ImageToolkit::MeanFilter(sprite, blur, level);
	sprite = blur;
}

void ImageInterface::GaussianBlur(unsigned level)
{
	if (!sprite || blur || level == 0)return;
	blur = new IMAGE(sprite->getwidth(), sprite->getheight());
	ImageToolkit::GaussianFilter(sprite, blur, level);
	sprite = blur;
}

IMAGE* ImageInterface::LoadSprite(std::string name)
{
	sprite = mainWorld.resourcePool->Fetch(name); 
	if (sprite) 
	{
		spriteInfo.endLoc = { sprite->getwidth(), sprite->getheight() };
	}
	return sprite;
}

void ImageInterface::SetStartAndEndLoc(FIntVector2 start, FIntVector2 end)
{
	start.x = FMath::Clamp(start.x, 0, spriteInfo.size.x);
    start.y = FMath::Clamp(start.y, 0, spriteInfo.size.y);
    end.x = FMath::Clamp(end.x, 0, spriteInfo.size.x);
    end.y = FMath::Clamp(end.y, 0, spriteInfo.size.y);
    spriteInfo.startLoc = start;
    spriteInfo.endLoc = end;
}






bool ImageToolkit::bIsGaussianFilterOn = false;
int ImageToolkit::GaussianFilterLevel = 2;

DWORD ImageToolkit::GetPixel(IMAGE* img, int32 i, int32 j)
{
	DWORD* pBuf = GetImageBuffer(img);
	j = FMath::Clamp(j, 0, img->getwidth() - 1);
	i = FMath::Clamp(i, 0, img->getheight() - 1);
	return pBuf[i * img->getwidth() + j];
}

void ImageToolkit::GetScreenShot()
{
	static int i = 0;
	std::string path = "SaveGame/ScreenShot/" + GetRealTime() + "_" + std::to_string(i++) + ".png";

	FileManager::CreateFolder("SaveGame");
	FileManager::CreateFolder("SaveGame/ScreenShot");
	IMAGE buf;
	getimage(&buf, 0, 0, WIN_WIDTH, WIN_HEIGHT);
	saveimage(path.c_str(), &buf);
}

void ImageToolkit::FlipImage(IMAGE* srcImg, IMAGE* dstImg, bool bIsHorizontal)
{
	if (!srcImg || !dstImg)return;

	const DWORD* pBuf = GetImageBuffer(srcImg);
	DWORD* pNewBuf = GetImageBuffer(dstImg);
	int width = srcImg->getwidth(), height = srcImg->getheight();

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			pNewBuf[bIsHorizontal ? ((i + 1) * width - j - 1) : ((height - i - 1) * width + j)] = pBuf[i * width + j];
		}
	}
}

void ImageToolkit::GetSectorImage(IMAGE* srcImg, IMAGE* dstImg, float start, float end)
{
	if (!srcImg || !dstImg || start == end)return;

	const DWORD* pBuf = GetImageBuffer(srcImg);
	DWORD* pNewBuf = GetImageBuffer(dstImg);
	int width = srcImg->getwidth(), height = srcImg->getheight();

	float centerX = width * 0.5f;
	float centerY = height * 0.5f;

	start = FMath::NormalizeDegree(start);
	end = FMath::NormalizeDegree(end);
	

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			float x = j - centerX;
			float y = i - centerY;
			float theta = FMath::RadianToDegree(FMath::Atan2(-y, x));  
			theta = FMath::NormalizeDegree(theta);

			if (end <= start)
			{
				if (theta >= end && theta < start)
				{
					continue;
				}
			}
			else if(theta <= start || theta > end)
			{
				continue;
			}
			pNewBuf[i * width + j] = pBuf[i * width + j];
		}
	}
}

FIntVector2 ImageToolkit::RotateImage(IMAGE* srcImg, IMAGE* dstImg, float degree)
{
	if (!srcImg || !dstImg)return{};

	degree = FMath::NormalizeDegree(degree);
	float radian = FMath::DegreeToRadian(degree);
	radian = -radian;
	float fSin = FMath::Sin(radian), fCos = FMath::Cos(radian);
	int w = srcImg->getwidth(), h = srcImg->getheight();
	POINT points[4] = { {0, 0}, {w, 0}, {0, h}, {w, h} };
	int min_x = 0, min_y = 0;
	int max_x = 0, max_y = 0;
	for (int i = 0; i < 4; i++)
	{
		points[i] =
		{
			(int)(points[i].x * fCos - points[i].y * fSin),
			(int)(points[i].x * fSin + points[i].y * fCos)
		};
		min_x = MIN(min_x, points[i].x);
		min_y = MIN(min_y, points[i].y);
		max_x = MAX(max_x, points[i].x);
		max_y = MAX(max_y, points[i].y);
	}

	int nw = max_x - min_x;
	int nh = max_y - min_y;

	dstImg->Resize(nw, nh);

	DWORD* pNewBuf = GetImageBuffer(dstImg);
	const DWORD* pBuf = GetImageBuffer(srcImg);

	for (int i = min_x, ni = 0; ni < nw; ++i, ++ni)//i用于映射原图像坐标，ni用于定位旋转后图像坐标
	{
		for (int j = min_y, nj = 0; nj < nh; ++j, ++nj)
		{
			pNewBuf[nj * nw + ni] = 0;
			int nx = (int)(i * fCos + j * fSin);//从旋转后的图像坐标向原图像坐标映射
			int ny = (int)(-i * fSin + j * fCos);
			if (nx >= 0 && nx < w && ny >= 0 && ny < h)
			{
				pNewBuf[nj * nw + ni] = pBuf[ny * w + nx];//若目标映射在原图像范围内，则拷贝色值
			}
		}
	}

	return FIntVector2(nw, nh);
}

void ImageToolkit::MeanFilter(IMAGE* srcImg, IMAGE* dstImg, int32 radius)
{
	if (!srcImg||!dstImg||radius == 0)return;

	const DWORD* pBuf = GetImageBuffer(srcImg);
	DWORD* pNewBuf = GetImageBuffer(dstImg);
	int width = srcImg->getwidth(), height = srcImg->getheight();

	auto GetIndex = [=](int i, int j)->int
		{
			j = FMath::Clamp(j, 0, width - 1);
			i = FMath::Clamp(i, 0, height - 1);
			return i * width + j;
		};

	struct rgb 
	{
		int r, g, b;
		rgb operator+(const rgb& another) { return { r + another.r,g + another.g,b + another.b }; }
		rgb operator-(const rgb& another) { return { r - another.r,g - another.g,b - another.b }; }
	};

	/* 积分图构建 */
	std::vector<std::vector<rgb>>integralImage;

	integralImage.resize(height + 1, std::vector<rgb>(width + 1));

	for (int i = 1; i <= height; i++) 
	{
		for (int j = 1; j <= width; j++) 
		{
			int color = pBuf[GetIndex(i - 1, j - 1)];
			integralImage[i][j] = integralImage[i - 1][j] + integralImage[i][j - 1] - integralImage[i - 1][j - 1]
				+ rgb{ (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF };
		}
	}

	/* 均值滤波 */
	int w = radius;

	for (int i = 0; i < height; ++i) 
	{
		for (int j = 0; j < width; ++j) 
		{
			int x = i + 1, y = j + 1;
			int x1 = MAX(x - w, 1), y1 = MAX(y - w, 1);
			int x2 = MIN(x + w, height), y2 = MIN(y + w, width);
			rgb center = integralImage[x2][y2] + integralImage[x1][y1] - integralImage[x2][y1] - integralImage[x1][y2];
			int validArea = (x2 - x1) * (y2 - y1);
			int r = FMath::Clamp(center.r / validArea, 0, 255), g = FMath::Clamp(center.g / validArea, 0, 255), b = FMath::Clamp(center.b / validArea, 0, 255);

			pNewBuf[GetIndex(i, j)] = (pBuf[GetIndex(i, j)] & 0xFF000000) | (r << 16) | (g << 8) | b;
		}
	}
}

void ImageToolkit::GaussianFilter(IMAGE* srcImg, IMAGE* dstImg, int32 radius)
{
	if (!srcImg || !dstImg || radius == 0)return;

	const DWORD* pBuf = GetImageBuffer(srcImg);
	DWORD* pNewBuf = GetImageBuffer(dstImg);
	int width = srcImg->getwidth(), height = srcImg->getheight();

	// 高斯核
	int size = radius * 2 + 1;
	float* kernel = new float[size];
	float sigma = float(radius) / 3.0f;
	float sum = 0.0f;
	for (int i = 0; i < size; i++)
	{
		float x = float(i) - radius;
		kernel[i] = FMath::Exp(-x * x / (2 * sigma * sigma)) / (FMath::Sqrt(2 * AE_PI) * sigma);
		sum += kernel[i];
	}
	for (int i = 0; i < size; i++) kernel[i] /= sum;
	

	// 水平模糊
	DWORD* temp = new DWORD[width * height];
	for (int y = 0; y < height; y++) 
	{
		for (int x = 0; x < width; x++) 
		{
			float r = 0, g = 0, b = 0;
			for (int i = -radius; i <= radius; i++) 
			{
				int xx = x + i;
				if (xx < 0) xx = 0;
				if (xx >= width) xx = width - 1;
				int index = y * width + xx;
				r += ((pBuf[index] >> 16) & 0xFF) * kernel[i + radius];
				g += ((pBuf[index] >> 8) & 0xFF) * kernel[i + radius];
				b += (pBuf[index] & 0xFF) * kernel[i + radius];
			}
			temp[y * width + x] = RGB((int)r, (int)g, (int)b);
		}
	}

	// 垂直模糊
	for (int y = 0; y < height; y++) 
	{
		for (int x = 0; x < width; x++) 
		{
			float r = 0, g = 0, b = 0;
			for (int i = -radius; i <= radius; i++)
			{
				int yy = y + i;
				if (yy < 0) yy = 0;
				if (yy >= height) yy = height - 1;
				int index = yy * width + x;
				r += ((temp[index] >> 16) & 0xFF) * kernel[i + radius];
				g += ((temp[index] >> 8) & 0xFF) * kernel[i + radius];
				b += (temp[index] & 0xFF) * kernel[i + radius];
			}
			pNewBuf[y * width + x] = (pBuf[y * width + x] & 0xFF000000) | RGB((int)r, (int)g, (int)b);
		}
	}

	delete[] kernel;
	delete[] temp;
}

void ImageToolkit::OutText(int x, int y, LPCTSTR str, BYTE alpha, COLORREF color, int size, LPCTSTR style)
{
	IMAGE img;
	SetWorkingImage(&img);

	setbkmode(TRANSPARENT);
	settextstyle(6 * size, 3 * size, style);
	settextcolor(color);

	int w = textwidth(str), h = textheight(str);
	img.Resize(w, h);
	outtextxy(0, 0, str);

	SetWorkingImage();

	DWORD* pMem = GetImageBuffer(&img);
	for (int i = 0; i < img.getwidth(); i++)
	{
		for (int j = 0; j < img.getheight(); j++)
		{
			int xy = j * img.getwidth() + i;
			if (pMem[xy] != 0) pMem[xy] = SET_ALPHA(pMem[xy], alpha);
		}
	}

	AlphaBlend(GetImageHDC(), x, y, w, h, GetImageHDC(&img), 0, 0, w, h, { AC_SRC_OVER,0,alpha,AC_SRC_ALPHA });
}

void ImageToolkit::ApplyGaussianFilterOnScreen()
{
	if (!bIsGaussianFilterOn)return;
	IMAGE buf;
	getimage(&buf, 0, 0, WIN_WIDTH, WIN_HEIGHT);
	GaussianFilter(&buf, &buf, GaussianFilterLevel);
	putimage(0, 0, &buf);
}