/**
 * @file	Struct.h
 * @brief	���ýṹ�嶨��
 * @author	Arty
 **/


#pragma once
#include<iostream>
#include<string>



constexpr double PI = 3.1415926;




/* ��ά���� */
struct FVector2D
{
	float x, y;
	FVector2D() :x(0), y(0) {}
	FVector2D(float a, float b) :x(a), y(b) {}

	//��ȡ��������
	float Size() const
	{
		return sqrtf(x * x + y * y);
	}

	//��ȡ��λ����
	FVector2D Normalize() const
	{
		return FVector2D(x / Size(), y / Size());
	}

	//������ת��Ϊ�ַ���
	std::string ToString() const
	{
		return "(" + std::to_string((int)x) + "," + std::to_string((int)y) + ")";
	}

	//�������
	static float DotProduct(const FVector2D& v1, const FVector2D& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	//�����������z��ֵΪ�����
	static float CrossProduct(const FVector2D& v1, const FVector2D& v2)
	{
		return v1.x * v2.y - v2.x * v1.y;
	}

	//��ȡ��������֮��ľ���
	static float Distance(const FVector2D& a, const FVector2D& b)
	{
		return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	}

	//��תĳһ������������ת�����������λ�ǽǶȣ�
	static FVector2D RotateVector(float angle, const FVector2D& another)
	{
		double radian = PI * angle / 180;
		float fsin = float(sin(radian));
		float fcos = float(cos(radian));
		return FVector2D(another.x * fcos - another.y * fsin, another.x * fsin + another.y * fcos);
	}

	//������ u ͶӰ������ v ���ڵ�ֱ����
	static FVector2D ProjectVector(const FVector2D& u, const FVector2D& v)
	{
		float scalar = DotProduct(u, v) / v.Size();  // ����ͶӰ��������������
		return v.Normalize() * scalar;
	}

	//������ת��Ϊ�Ƕ�
	static float VectorToDegree(const FVector2D& v)
	{
		if (v == FVector2D())
		{
			return 0.0f; 
		}
		float radian = atan2(-v.y, v.x);
		return radian * 180.0f / PI;
	}

	FVector2D operator+(const FVector2D& another)const
	{
		return FVector2D(x + another.x, y + another.y);
	}
	FVector2D operator-(const FVector2D& another)const
	{
		return FVector2D(x - another.x, y - another.y);
	}
	FVector2D& operator+=(const FVector2D& another)
	{
		x += another.x; y += another.y;
		return *this;
	}
	FVector2D& operator-=(const FVector2D& another)
	{
		x -= another.x; y -= another.y;
		return *this;
	}


	bool operator==(const FVector2D& another) const
	{
		return x == another.x && y == another.y;
	}
	bool operator!=(const FVector2D& another) const
	{
		return x != another.x || y != another.y;
	}
	FVector2D operator*(float scale) const
	{
		return FVector2D(x * scale, y * scale);
	}
	FVector2D& operator*=(float scale)
	{
		x *= scale; y *= scale;
		return *this;
	}
	FVector2D operator/(float scale) const
	{
		return FVector2D(x / scale, y / scale);
	}
	FVector2D& operator/=(float scale)
	{
		x /= scale; y /= scale;
		return *this;
	}
	FVector2D operator-() const
	{
		return FVector2D(-x, -y);
	}
	FVector2D operator*(const FVector2D& another) const
	{
		return FVector2D(x * another.x, y * another.y);
	}
	FVector2D operator/(const FVector2D& another) const
	{
		return FVector2D(x / another.x, y / another.y);
	}
};

inline std::ostream& operator<<(std::ostream& out, const FVector2D& vec)
{
	std::cout << "(" << vec.x << "," << vec.y << ")";
	return out;
}

inline FVector2D operator*(float scale, FVector2D vec)
{
	return FVector2D(vec.x * scale, vec.y * scale);
}





/* ���� */
struct FPair
{
	int x, y;
	FPair() :x(0), y(0) {}
	FPair(int x, int y) :x(x), y(y) {}
	FPair(FVector2D vec) :x(int(vec.x)), y(int(vec.y)) {}
	bool operator==(const FPair& another) const
	{
		return x == another.x && y == another.y;
	}
	bool operator!=(const FPair& another) const
	{
		return x != another.x || y != another.y;
	}
};




/* ���������� */
struct FTransform
{
	FVector2D position;
	float rotation;
	FVector2D scale;

	FTransform() :position(0, 0), rotation(0), scale(1, 1) {}
	FTransform(const FVector2D& pos, float angle, const FVector2D& scale)
		:position(pos), rotation(angle), scale(scale) {}

	FTransform Indentity() const
	{
		return FTransform(FVector2D(0, 0), 0, FVector2D(1, 1));
	}
};


/* ���ʽ�Ϸ�ʽ */
enum class ECombinePattern :unsigned char { Min, Mid, Max };

/* ��������� */
struct FPhysicsMaterial
{
	float friction;
	float bounciness;
	FPhysicsMaterial() :friction(0.4f), bounciness(0) {}
	FPhysicsMaterial(float friction, float bounciness) 
		:friction(friction), bounciness(bounciness) {}

	//������ʽ��Ч��
	static FPhysicsMaterial Combine(const FPhysicsMaterial& m1, const FPhysicsMaterial& m2,ECombinePattern pattern = ECombinePattern::Mid)
	{
		if (pattern == ECombinePattern::Mid)
		{
			return FPhysicsMaterial((m1.friction + m2.friction) * 0.5f, (m1.bounciness + m2.bounciness) * 0.5f);
		}
		else 
		{
			return pattern == ECombinePattern::Min ? FPhysicsMaterial(std::min(m1.friction,m2.friction), std::min(m1.bounciness,m2.bounciness))
				: FPhysicsMaterial(std::max(m1.friction, m2.friction), std::max(m1.bounciness, m2.bounciness));
		}
	}
};



/* ���ο� */
struct FRect
{
	float left, top, right, bottom;

	FRect(float left = 0, float top = 0, float right = 0, float bottom = 0)
		: left(left), top(top), right(right), bottom(bottom) {}

	// ��ȡ���ε����ĵ�
	FVector2D GetCenter() const { return { (left + right) / 2, (bottom + top) / 2 }; }

	//��ȡ���δ�С
	FVector2D GetSize() const { return { right - left,top - bottom }; }

	// ������������Ƿ��ཻ
	bool Intersects(const FRect& other) const
	{
		return left < other.right && right > other.left && bottom < other.top && top > other.bottom;
	}
};