/**
 * @file	Transform.h
 * @brief   �����任ģ�嶨��
 * @author	Arty
 **/


#pragma once
#include"Vector2D.h"


namespace Math
{
	/* �����任���� */
	template<typename T>
	struct TTransform
	{
		TVector2<T> position;
		T rotation;
		TVector2<T> scale;

		TTransform<T>() 
			:position(0, 0), rotation(0), scale(1, 1) {}
		TTransform<T>(const TVector2<T>& pos, T angle, const TVector2<T>& scale)
			:position(pos), rotation(angle), scale(scale) {}

		static const TTransform<T> Indentity;
	};

	template<typename T>
	const TTransform<T> TTransform<T>::Indentity = TTransform<T>();
}