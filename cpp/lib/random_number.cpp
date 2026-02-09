#include "main.h"
#include "lib/random_number.h"
#include <numeric>

std::unique_ptr<RandomNumber> RandomNumber::m_Instance = nullptr;


void RandomNumber::Init()
{
	// シード値をセット
	std::random_device rd;
	m_Seed = rd();
	m_Mt.seed(m_Seed);

	// パーリンノイズ用のテーブルを初期化
	m_Permutation.resize(256);
	std::iota(m_Permutation.begin(), m_Permutation.end(), 0);
	// シャッフル
	std::shuffle(m_Permutation.begin(), m_Permutation.end(), m_Mt);

	// 256要素を2回繰り返して512要素にする(参照時のオーバーフロー防止)
	m_Permutation.insert(m_Permutation.end(), m_Permutation.begin(), m_Permutation.end());
}

void RandomNumber::SetSeed(int seed)
{
	m_Seed = seed;
	m_Mt.seed(m_Seed);

	// シード変更時はノイズパターンも再生成する
	std::iota(m_Permutation.begin(), m_Permutation.begin() + 256, 0);
	std::shuffle(m_Permutation.begin(), m_Permutation.begin() + 256, m_Mt);
	// 後半のコピーを更新
	std::copy(m_Permutation.begin(), m_Permutation.begin() + 256, m_Permutation.begin() + 256);
}

int RandomNumber::GetSeed() const
{
	return m_Seed;
}

int RandomNumber::GetRandomInt(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(m_Mt);
}

float RandomNumber::GetRandomFloat(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(m_Mt);
}

int RandomNumber::GetRandomValue()
{
	return m_Mt();
}

float RandomNumber::Fade(float t) 
{
	// 6t^5 - 15t^4 + 10t^3
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float RandomNumber::Lerp(float t, float a, float b) 
{
	return a + t * (b - a);
}

float RandomNumber::Grad(int hash, float x, float y, float z) 
{
	// ハッシュ値の下位4ビットを使って12方向の勾配ベクトルを算出
	int h = hash & 15;
	float u = h < 8 ? x : y;
	float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float RandomNumber::GetPerlinNoise(float x, float y, float z) 
{
	// 単位立方体の座標
	int X = (int)floor(x) & 255;
	int Y = (int)floor(y) & 255;
	int Z = (int)floor(z) & 255;

	// 立方体内の相対座標(0.0~1.0)
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	// フェード曲線
	float u = Fade(x);
	float v = Fade(y);
	float w = Fade(z);

	// ハッシュ値
	int A = m_Permutation[X] + Y;
	int AA = m_Permutation[A] + Z;
	int AB = m_Permutation[A + 1] + Z;
	int B = m_Permutation[X + 1] + Y;
	int BA = m_Permutation[B] + Z;
	int BB = m_Permutation[B + 1] + Z;

	// 補間計算(-1.0~1.0 の範囲)
	float res = Lerp(w, Lerp(v, Lerp(u, Grad(m_Permutation[AA], x, y, z),
		Grad(m_Permutation[BA], x - 1, y, z)),
		Lerp(u, Grad(m_Permutation[AB], x, y - 1, z),
			Grad(m_Permutation[BB], x - 1, y - 1, z))),
		Lerp(v, Lerp(u, Grad(m_Permutation[AA + 1], x, y, z - 1),
			Grad(m_Permutation[BA + 1], x - 1, y, z - 1)),
			Lerp(u, Grad(m_Permutation[AB + 1], x, y - 1, z - 1),
				Grad(m_Permutation[BB + 1], x - 1, y - 1, z - 1))));

	// 0.0~1.0に正規化して返す
	return (res + 1.0) * 0.5;
}