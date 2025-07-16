#pragma once
#include <cmath>

class Vector2
{
public:
	float x, y;

	// コンストラクタ
	Vector2() 
	{
		// 初期化
		x = y = 0.0f;
	};

	// コピーコンストラクタ
	Vector2(const Vector2& v) : x(v.x), y(v.y) {};
	// 3つの値で作成する
	Vector2(float x, float y) : x(x), y(y) {};

	// 代入
	Vector2& operator=(const Vector2& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	// 比較チェック

	bool operator==(const Vector2& v) const
	{
		return (x == v.x && y == v.y);
	}

	bool operator!=(const Vector2& v) const
	{
		return (x != v.x || y != v.y);
	}

	// ベクトル操作
	
	// ベクトルを0に設定する
	void zero()
	{
		x = y = 0.0f;
	}

	// 単位式のマイナスは反転したベクトルを返す
	Vector2 operator-() const
	{
		return Vector2(-x, -y);
	}

	// 二項式の+と-は、ベクトルの加算と減算を行う
	Vector2 operator+(const Vector2& v) const
	{
		return Vector2(x + v.x, y + v.y);
	}

	Vector2 operator-(const Vector2& v) const
	{
		return Vector2(x - v.x, y - v.y);
	}

	// スカラーによる乗算と除算
	Vector2 operator*(float s) const
	{
		return Vector2(x * s, y * s);
	}

	Vector2 operator/(float s) const
	{
		float oneOverS = 1.0f / s;
		return Vector2(x * oneOverS, y * oneOverS);
	}

	// 組み合わせ代入演算
	Vector2& operator+=(const Vector2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	Vector2& operator-=(const Vector2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Vector2& operator*=(float s)
	{
		x *= s;
		y *= s;
		return *this;
	}

	Vector2& operator/=(float s)
	{
		float oneOverS = 1.0f / s;
		x *= oneOverS;
		y *= oneOverS;
		return *this;
	}

	// ベクトルを正規化する
	void normalize()
	{
		float length = x * x + y * y;
		if (length > 0.0f)
		{
			float oneOverLength = 1.0f / sqrt(length);
			x *= oneOverLength;
			y *= oneOverLength;
		}
	}

	// ベクトルの内積
	float operator*(const Vector2& v) const
	{
		return x * v.x + y * v.y;
	}

	// ベクトルの長さを計算する
	float length() const
	{
		return sqrt(x * x + y * y);
	}

	// ベクトルの長さの二乗を計算する(判定時など、厳密な値がいらない場合はこっちを使ってコストを下げる)
	float lengthSquared() const
	{
		return x * x + y * y;
	}
};

// ベクトルの大きさを計算する
inline float vectorMag(const Vector2& v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

// 対称性のために、左からスカラーを乗算する
inline Vector2 operator*(float s, const Vector2& v)
{
	return Vector2(v.x * s, v.y * s);
}

// 2つの点の距離を計算する
inline float distance(const Vector2& v1, const Vector2& v2)
{
	float dx = v1.x - v2.x;
	float dy = v1.y - v2.y;
	return sqrt(dx * dx + dy * dy);
}

// グローバル変数
extern const Vector2 g_ZeroVector2; // ゼロベクトル