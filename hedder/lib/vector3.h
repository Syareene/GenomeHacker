#pragma once
#include <cmath>

class Vector3
{
public:
	float x, y, z;

	// コンストラクタ
	Vector3() 
	{
		// 初期化
		x = y = z = 0.0f;
	};

	// コピーコンストラクタ
	Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {};
	// 3つの値で作成する
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {};

	// 代入
	Vector3& operator=(const Vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	// 比較チェック

	bool operator==(const Vector3& v) const
	{
		return (x == v.x && y == v.y && z == v.z);
	}

	bool operator!=(const Vector3& v) const
	{
		return (x != v.x || y != v.y || z != v.z);
	}

	// ベクトル操作
	
	// ベクトルを0に設定する
	void zero()
	{
		x = y = z = 0.0f;
	}

	// 単位式のマイナスは反転したベクトルを返す
	Vector3 operator-() const
	{
		return Vector3(-x, -y, -z);
	}

	// 二項式の+と-は、ベクトルの加算と減算を行う
	Vector3 operator+(const Vector3& v) const
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(const Vector3& v) const
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	// ベクトル同士の要素ごとの乗算
	Vector3 mul(const Vector3& v) const
	{
		return Vector3(x * v.x, y * v.y, z * v.z);
	}

	// スカラーによる乗算と除算
	Vector3 operator*(float s) const
	{
		return Vector3(x * s, y * s, z * s);
	}

	Vector3 operator/(float s) const
	{
		float oneOverS = 1.0f / s;
		return Vector3(x * oneOverS, y * oneOverS, z * oneOverS);
	}

	// 組み合わせ代入演算
	Vector3& operator+=(const Vector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector3& operator-=(const Vector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector3& operator*=(float s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	Vector3& operator/=(float s)
	{
		float oneOverS = 1.0f / s;
		x *= oneOverS;
		y *= oneOverS;
		z *= oneOverS;
		return *this;
	}

	// ベクトルを正規化する
	void normalize()
	{
		float length = x * x + y * y + z * z;
		if (length > 0.0f)
		{
			float oneOverLength = 1.0f / sqrt(length);
			x *= oneOverLength;
			y *= oneOverLength;
			z *= oneOverLength;
		}
	}

	// ベクトルの内積
	float operator*(const Vector3& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	// ベクトルの長さを計算する
	float length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	// ベクトルの長さの二乗を計算する(判定時など、厳密な値がいらない場合はこっちを使ってコストを下げる)
	float lengthSquared() const
	{
		return x * x + y * y + z * z;
	}
};

// ベクトルの大きさを計算する
inline float vectorMag(const Vector3& v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// 2つのベクトルの外積を計算する
inline Vector3 CrossProduct(const Vector3& v1, const Vector3& v2)
{
	return Vector3(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	);
}

// 対称性のために、左からスカラーを乗算する
inline Vector3 operator*(float s, const Vector3& v)
{
	return Vector3(v.x * s, v.y * s, v.z * s);
}

// 2つの点の距離を計算する
inline float distance(const Vector3& v1, const Vector3& v2)
{
	float dx = v1.x - v2.x;
	float dy = v1.y - v2.y;
	float dz = v1.z - v2.z;
	return sqrt(dx * dx + dy * dy + dz * dz);
}

// グローバル変数
extern const Vector3 g_ZeroVector3; // ゼロベクトル