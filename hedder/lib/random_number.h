#pragma once


#include <random>
#include <memory>

class RandomNumber
{
public:
	static RandomNumber* GetInstance()
	{
		if (!m_Instance)
		{
			// インスタンスがなければ生成
			m_Instance = std::make_unique<RandomNumber>();
		}
		return m_Instance.get();
	}
	void Init();
	void SetSeed(int seed);
	int GetSeed() const;
	// 指定した範囲の整数をランダムに取得
	int GetRandomInt(int min, int max);
	// 指定した範囲の浮動小数点数をランダムに取得
	float GetRandomFloat(float min, float max);
	// とりあえずランダムな値を取得する関数
	int GetRandomValue();
private:
	static std::unique_ptr<RandomNumber> m_Instance;
	std::mt19937 m_Mt;
	std::random_device m_RandomDevice;
	int m_Seed;
};