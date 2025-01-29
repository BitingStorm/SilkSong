/**
 * @file	Struct.h
 * @brief	常用结构体定义
 * @author	Arty
 **/


#pragma once
#include<iostream>
#include<string>



constexpr double PI = 3.1415926;




/* 二维向量 */
struct Vector2D
{
	float x, y;
	Vector2D() :x(0), y(0) {}
	Vector2D(float a, float b) :x(a), y(b) {}

	//获取向量长度
	float Size() const
	{
		return sqrtf(x * x + y * y);
	}

	//获取单位向量
	Vector2D Normalize() const
	{
		return Vector2D(x / Size(), y / Size());
	}

	//将向量转化为字符串
	std::string ToString() const
	{
		return "(" + std::to_string((int)x) + "," + std::to_string((int)y) + ")";
	}

	//向量点积
	static float DotProduct(const Vector2D& v1, const Vector2D& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	//向量叉积
	static Vector2D CrossProduct(const Vector2D& v1, const Vector2D& v2)
	{
		return { v1.y * v2.x - v1.x * v2.y, v1.x * v2.y - v1.y * v2.x };
	}

	//获取两个坐标之间的距离
	static float Distance(const Vector2D& a, const Vector2D& b)
	{
		return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	}

	//旋转某一向量，返回旋转后的向量（单位是角度）
	static Vector2D RotateVector(float angle, const Vector2D& another)
	{
		double radian = PI * angle / 180;
		float fsin = float(sin(radian));
		float fcos = float(cos(radian));
		return Vector2D(another.x * fcos - another.y * fsin, another.x * fsin + another.y * fcos);
	}

	//将向量 u 投影到向量 v 所在的直线上
	static Vector2D ProjectVector(const Vector2D& u, const Vector2D& v)
	{
		float scalar = DotProduct(u, v) / v.Size();  // 计算投影向量的缩放因子
		return v.Normalize() * scalar;
	}

	//将向量转换为角度
	static float VectorToDegree(const Vector2D& v)
	{
		if (v == Vector2D())
		{
			return 0.0f; 
		}
		float radian = atan2(-v.y, v.x);
		return radian * 180.0f / PI;
	}

	Vector2D operator+(const Vector2D& another)const
	{
		return Vector2D(x + another.x, y + another.y);
	}
	Vector2D operator-(const Vector2D& another)const
	{
		return Vector2D(x - another.x, y - another.y);
	}
	Vector2D& operator+=(const Vector2D& another)
	{
		x += another.x; y += another.y;
		return *this;
	}
	Vector2D& operator-=(const Vector2D& another)
	{
		x -= another.x; y -= another.y;
		return *this;
	}


	bool operator==(const Vector2D& another) const
	{
		return x == another.x && y == another.y;
	}
	bool operator!=(const Vector2D& another) const
	{
		return x != another.x || y != another.y;
	}
	Vector2D operator*(float scale) const
	{
		return Vector2D(x * scale, y * scale);
	}
	Vector2D& operator*=(float scale)
	{
		x *= scale; y *= scale;
		return *this;
	}
	Vector2D operator/(float scale) const
	{
		return Vector2D(x / scale, y / scale);
	}
	Vector2D& operator/=(float scale)
	{
		x /= scale; y /= scale;
		return *this;
	}
	Vector2D operator-() const
	{
		return Vector2D(-x, -y);
	}
	Vector2D operator*(const Vector2D& another) const
	{
		return Vector2D(x * another.x, y * another.y);
	}
	Vector2D operator/(const Vector2D& another) const
	{
		return Vector2D(x / another.x, y / another.y);
	}
};

inline std::ostream& operator<<(std::ostream& out, const Vector2D& vec)
{
	std::cout << "(" << vec.x << "," << vec.y << ")";
	return out;
}

inline Vector2D operator*(float scale, Vector2D vec)
{
	return Vector2D(vec.x * scale, vec.y * scale);
}





/* 数对 */
struct Pair
{
	int x, y;
	Pair() :x(0), y(0) {}
	Pair(int x, int y) :x(x), y(y) {}
	Pair(Vector2D vec) :x(int(vec.x)), y(int(vec.y)) {}
	bool operator==(const Pair& another) const
	{
		return x == another.x && y == another.y;
	}
	bool operator!=(const Pair& another) const
	{
		return x != another.x || y != another.y;
	}
};




/* 场景属性类 */
struct Transform
{
	Vector2D position;
	float rotation;
	Vector2D scale;

	Transform() :position(0, 0), rotation(0), scale(1, 1) {}
	Transform(const Vector2D& pos, float angle, const Vector2D& scale)
		:position(pos), rotation(angle), scale(scale) {}

	Transform Indentity() const
	{
		return Transform(Vector2D(0, 0), 0, Vector2D(1, 1));
	}
};


/* 材质结合方式 */
enum class CombinePattern :unsigned char { Min, Mid, Max };

/* 物理材质类 */
struct PhysicsMaterial
{
	float friction;
	float bounciness;
	PhysicsMaterial() :friction(0.4f), bounciness(0) {}
	PhysicsMaterial(float friction, float bounciness) 
		:friction(friction), bounciness(bounciness) {}

	//物理材质结合效果
	static PhysicsMaterial Combine(const PhysicsMaterial& m1, const PhysicsMaterial& m2,CombinePattern pattern = CombinePattern::Mid)
	{
		if (pattern == CombinePattern::Mid)
		{
			return PhysicsMaterial((m1.friction + m2.friction) * 0.5f, (m1.bounciness + m2.bounciness) * 0.5f);
		}
		else 
		{
			return pattern == CombinePattern::Min ? PhysicsMaterial(std::min(m1.friction,m2.friction), std::min(m1.bounciness,m2.bounciness))
				: PhysicsMaterial(std::max(m1.friction, m2.friction), std::max(m1.bounciness, m2.bounciness));
		}
	}
};



/* 矩形框 */
struct Rect
{
	float left, top, right, bottom;

	Rect(float left = 0, float top = 0, float right = 0, float bottom = 0)
		: left(left), top(top), right(right), bottom(bottom) {}

	// 获取矩形的中心点
	Vector2D GetCenter() const { return { (left + right) / 2, (bottom + top) / 2 }; }

	//获取矩形大小
	Vector2D GetSize() const { return { right - left,top - bottom }; }

	// 检查两个矩形是否相交
	bool Intersects(const Rect& other) const
	{
		return left < other.right && right > other.left && bottom < other.top && top > other.bottom;
	}
};