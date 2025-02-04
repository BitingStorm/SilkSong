/**
 * @file	VisualInterface.h
 * @brief	����һ����2Dͼ��������Ⱦ�йصĽӿ�
 * @author	Arty
 **/


#pragma once
#include"Core/Struct.h"
#include<easyx.h>
#include<set>



/*----------------------------------
			   ͼ��ӿ�
  ----------------------------------*/
class LayerInterface
{
	int layer = 0;
public:
	LayerInterface();
	virtual ~LayerInterface();

	//���ò㼶�������ڹ����BeginPlay������ʹ�ã���ֹ�󶨵�����ӳ��
	void SetLayer(int layer);
	
	int GetLayer()const;

	virtual void Render() = 0;
};


/* ��Ⱦ������Ϣ */
struct FSpriteInfo
{
	FVector2D offset = FVector2D(0, 0);
	FPair size = FPair(0, 0);
	FPair startLoc = FPair(0, 0);
	FPair endLoc = FPair(0, 0);
};


/* �˾���Ϣ */
struct FilterInfo 
{
	COLORREF color = BLACK;//�˾���ɫ
	int level = 50;//��ɫ���ɲ㼶��1~100��
	int layer = 0;//�˾��㼶��0��1��
};


/* �˾�ͼ��������� */
struct FFilterSort
{
	bool operator()(const FilterInfo& a, const FilterInfo& b) const
	{
        return a.layer < b.layer;
	}
};

class Animator;

/*----------------------------------
			   ͼ��ӿ�
  ----------------------------------*/
class ImageInterface
{
	friend Animator;

protected:
	IMAGE* sprite{};
	FSpriteInfo spriteInfo;
	BYTE alpha = 255;

	IMAGE* copy{};
	float angle = 0;
	void RotateImage(float degree);

	IMAGE* filter{};
	std::set<FilterInfo, FFilterSort>filterLayers;
	void FilterImage();
	void AddFilter(FilterInfo filterInfo);
	void RemoveFilter();

	IMAGE* blur{};

	Animator* animatorAttached = nullptr;//���ŵĶ���������
	virtual void DealImage() = 0;

public:
	virtual ~ImageInterface();

	BYTE GetTransparency()const { return alpha; }
	void SetTransparency(BYTE tranparency) { alpha = tranparency; }


	/**
	 * @brief �����˾�
	 * @param[in] enable			�Ƿ����˾�
	 * @param[in] color	            �˾���ɫ
	 * @param[in] level             �˾�ʩ�ӳ̶ȣ����Ϊ100����СΪ0��
	 **/
	void SetFilter(bool enable, COLORREF col = BLACK, int level = 60);

	/**
	 * @brief ��ֵģ����������Gameplay��ʼǰԤ����ʹ�ã�
	 * @param[in] level			    ģ���̶ȣ����С������������Ӱ��
	 **/
	void MeanBlur(unsigned level);

	/**
	 * @brief ��˹ģ������ͬʱ����ݡ���Ϊ�ĕr��������Gameplay��ʼǰԤ����ʹ�ã�
	 * @param[in] level			    ģ���̶ȣ�Խ��ͬʱ�����Ч��Խ�ã����������ľ͸���
	 **/
	void GaussianBlur(unsigned level);

	//���طǶ�����Դ 
	IMAGE* LoadSprite(std::string name);

	/**
	 * @brief ��ȡ�Ƕ�����Դָ������ 
	 * @param[in] start			    ��ȡͼ����ʼλ������
	 * @param[in] end               ��ȡͼ���βλ������
	 **/
	void SetStartAndEndLoc(FPair start, FPair end);
};



/*----------------------------------
			  ͼ������
  ----------------------------------*/
class ImageToolkit final
{
	friend class World;
	friend class GameplayStatics;

public:
	//��ȡ��ά���괦����ֵ
	static DWORD GetPixel(IMAGE* img, int i, int j);

	//��ȡ��ǰ��Ϸ��ͼ����ֹ���lʹ�ã���
	static void GetScreenShot();


	/** �任 **/

	//����ת�任
	static void FlipImage(IMAGE* srcImg, IMAGE* dstImg, bool bIsHorizontal = true);

	//��ȡ�нǲ��ֵ�ͼ��
	static void GetSectorImage(IMAGE* srcImg, IMAGE* dstImg, float start, float end);

	//��תͼ��
	static FPair RotateImage(IMAGE* srcImg, IMAGE* dstImg, float degree);

	/** �˲� **/

	//���پ�ֵ�˲�(ģ��)
	static void MeanFilter(IMAGE* srcImg, IMAGE* dstImg, int radius);

	//��˹�˲�(ģ���ҿ���ݡ�����Ϊ�ĕr������)
	static void GaussianFilter(IMAGE* srcImg, IMAGE* dstImg, int radius);

private:
	//��������Ļʩ�Ӹ�˹�˲�
	static void ApplyGaussianFilterOnScreen();

	static bool bIsGaussianFilterOn;

	static int GaussianFilterLevel;
};
