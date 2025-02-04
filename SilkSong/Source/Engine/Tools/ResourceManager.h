/**
 * @file	ResourceManager.h
 * @brief   ��Դ���������壬���ڷ��ʲ�����ȫ����Ϸͼ��ý����Դ
 * @author	Arty
 **/

#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "Core/Struct.h"



class IMAGE;

/* ������Դ�� */
struct AnimationResource
{
	IMAGE** images;
	int num;
};


/*----------------------------------
			 ��Դ������
  ----------------------------------*/
class ResourceManager
{
public:
	void Initialize();

	IMAGE* Fetch(std::string name);
	AnimationResource FetchAnimation(std::string name);

private:
	std::unordered_map<std::string, IMAGE*>imagePool;
	std::unordered_map<std::string, AnimationResource>animationPool;
	std::unordered_map<std::string, std::string>audioPool;

	/**
	 * @brief ���ص���ͼ����Դ
	 * @param[in] name			    ָ����Դ����
	 * @param[in] path              ��Դ·��
	 * @param[in] wid			    ͼ����
	 * @param[in] hei	            ͼ��߶� 
	 **/
	void Load(std::string name, std::string path, int wid, int hei);

	/**
	 * @brief ���ض��Ŷ���ͼ����Դ
	 * @param[in] name			    ָ����Դ����
	 * @param[in] path              ��Դ·��
	 * @param[in] wid			    ͼ����
	 * @param[in] hei	            ͼ��߶�
	 * @param[in] hei	            ����֡����
	 **/
	void Load(std::string name, std::string path, int wid, int hei, int num);

	/**
	 * @brief ���ض��Ŷ���ͼ����Դ
	 * @param[in] name			    ָ����Դ����
	 * @param[in] path              ��Դ·��
	 * @param[in] arr			    ����һ��Pair���飬����ָ��ÿһ֡�����Ŀ����߶�
	 **/
	void Load(std::string name, std::string path, const std::vector<FPair>& arr);

	/**
	 * @brief ����SpriteSheet����ͼ����Դ
	 * @param[in] name			    ָ����Դ����
	 * @param[in] path              ��Դ·��
	 * @param[in] wid			    SpriteSheet���
	 * @param[in] hei	            SpriteSheet�߶�
	 * @param[in] row	            SpriteSheet����
	 * @param[in] col	            SpriteSheet����
	 **/
	void Load(std::string name, std::string path, int wid, int hei, int num, int row, int col);

	//������Ƶý����Դ
	void Load(std::string name, std::string path);

	//����������Դ
	void LoadText(std::string path);

	//���������ʽ��Դ
	void LoadCustomCursor(std::string path);
};