#include "main.h"
#include "lib/random_number.h"

std::unique_ptr<RandomNumber> RandomNumber::m_Instance = nullptr;


void RandomNumber::Init()
{
	// シード値をセット
	std::random_device rd;
	m_Seed = rd();
	m_Mt.seed(m_Seed);
}

void RandomNumber::SetSeed(int seed)
{
	m_Seed = seed;
	m_Mt.seed(m_Seed);
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