/**
 * @file	VisualInterface.h
 * @brief	包含一切与2D图像处理与渲染有关的接口
 * @author	Arty
 **/


#pragma once
#include"Core/Struct.h"
#include<easyx.h>
#include<set>



/*----------------------------------
			   图层接口
  ----------------------------------*/
class LayerInterface
{
	int layer = 0;
public:
	LayerInterface();
	virtual ~LayerInterface();

	//设置层级，尽量在构造或BeginPlay函数中使用，禁止绑定到按键映射
	void SetLayer(int layer);
	
	int GetLayer()const;

	virtual void Render() = 0;
};


/* 渲染补充信息 */
struct SpriteInfo
{
	Vector2D offset = Vector2D(0, 0);
	Pair size = Pair(0, 0);
	Pair startLoc = Pair(0, 0);
	Pair endLoc = Pair(0, 0);
};


/* 滤镜信息 */
struct FilterInfo 
{
	COLORREF color = BLACK;//滤镜颜色
	int level = 50;//颜色过渡层级（1~100）
	int layer = 0;//滤镜层级（0和1）
};


/* 滤镜图层排序规则 */
struct FilterSort
{
	bool operator()(const FilterInfo& a, const FilterInfo& b) const
	{
        return a.layer < b.layer;
	}
};



/*----------------------------------
			   图像接口
  ----------------------------------*/
class ImageInterface
{
protected:
	IMAGE* sprite{};
	SpriteInfo spriteInfo;
	BYTE alpha = 255;

	IMAGE* copy{};
	float angle = 0;
	void RotateImage(double radian);

	IMAGE* filter{};
	std::set<FilterInfo, FilterSort>filterLayers;
	void FilterImage();
	void AddFilter(FilterInfo filterInfo);
	void RemoveFilter();

	IMAGE* blur{};

public:
	virtual ~ImageInterface() { if (copy)delete copy; if (filter)delete filter; if (blur)delete blur; }

	BYTE GetTransparency()const { return alpha; }
	void SetTransparency(BYTE tranparency) {alpha = tranparency; }


	/**
	 * @brief 设置滤镜
	 * @param[in] enable			是否开启滤镜
	 * @param[in] color	            滤镜颜色
	 * @param[in] level             滤镜施加程度（最大为100，最小为0）
	 **/
	void SetFilter(bool enable, COLORREF col = BLACK, int level = 60);

	/**
	 * @brief 均值模糊（建议在Gameplay开始前预处理使用）
	 * @param[in] level			    模糊程度，其大小对性能无显著影响
	 **/
	void MeanBlur(unsigned level);

	/**
	 * @brief 高斯模糊（可同时抗锯齿。较为耗時，建议在Gameplay开始前预处理使用）
	 * @param[in] level			    模糊程度，越高同时抗锯齿效果越好，但性能消耗就更大
	 **/
	void GaussianBlur(unsigned level);

	//加载非动画资源 
	void LoadSprite(std::string name);

	/**
	 * @brief 截取非动画资源指定部分 
	 * @param[in] start			    截取图像起始位置坐标
	 * @param[in] end               截取图像结尾位置坐标
	 **/
	void SetStartAndEndLoc(Pair start, Pair end);

	////抗锯齿(快速傅里叶变换版)
	//static void Convolve(IMAGE* img,int radius);
};



/*----------------------------------
			  图像处理器
  ----------------------------------*/
class ImageToolkit
{
	friend class World;
	friend class GameplayStatics;

public:
	//获取二维坐标处像素值
	static DWORD GetPixel(IMAGE* img, int i, int j);

	//获取当前游戏截图（禁止高頻使用！）
	static void GetScreenShot();


	/** 变换 **/

	//镜像翻转变换
	static void FlipImage(IMAGE* srcImg, IMAGE* dstImg, bool bIsHorizontal = true);

	//截取夹角部分的图像
	static void GetSectorImage(IMAGE* srcImg, IMAGE* dstImg, float start, float end);

	/** 滤波 **/

	//快速均值滤波(模糊)
	static void MeanFilter(IMAGE* srcImg, IMAGE* dstImg, int radius);

	//高斯滤波(模糊且抗锯齿。但较为耗時，慎用)
	static void GaussianFilter(IMAGE* srcImg, IMAGE* dstImg, int radius);

private:
	//对整个屏幕施加高斯滤波
	static void ApplyGaussianFilterOnScreen();

	static bool bIsGaussianFilterOn;

	static int GaussianFilterLevel;
};
