#include "lib/easing.h"

// 一次関数版

// 最初は早いが徐々に減速
float OutSine(float t)
{
	return sinf(t * XM_PIDIV2);
}

// 最初は遅いが徐々に加速
float InSine(float t)
{
	return 1.0f - cosf(t * XM_PIDIV2);
}

// 最初と最後は早いが途中は遅い
float InOutSine(float t)
{
	return -0.5f * (cosf(XM_PI * t) - 1.0f);
}


// 二次関数版

// 最初は早いが徐々に減速
float OutQuad(float t)
{
    return -t * (t - 2.0f);
}

// 最初は遅いが徐々に加速
float InQuad(float t)
{
    return t * t;
}

// 最初と最後は早いが途中は遅い
float InOutQuad(float t)
{
    t *= 2.0f;
    if (t < 1.0f)
    {
        return 0.5f * t * t;
    }
    else
    {
        t -= 1.0f;
        return -0.5f * (t * (t - 2.0f) - 1.0f);
    }
}

// 三次関数版

// 最初は早いが徐々に減速
float OutCubic(float t)
{
	t -= 1.0f;
	return t * t * t + 1.0f;
}

// 最初は遅いが徐々に加速
float InCubic(float t)
{
	return t * t * t;
}

// 最初と最後は早いが途中は遅い
float InOutCubic(float t)
{
	t *= 2.0f;
	if (t < 1.0f)
	{
		return 0.5f * t * t * t;
	}
	else
	{
		t -= 2.0f;
		return 0.5f * (t * t * t + 2.0f);
	}
}

// 四次関数版 二次関数よりも大げさに動く

// 最初は早いが徐々に減速
float OutQuart(float t)
{
    t -= 1.0f;
    return -(t * t * t * t - 1.0f);
}

// 最初は遅いが徐々に加速
float InQuart(float t)
{
    return t * t * t * t;
}

// 最初と最後は早いが途中は遅い
float InOutQuart(float t)
{
    t *= 2.0f;
    if (t < 1.0f)
    {
        return 0.5f * t * t * t * t;
    }
    else
    {
        t -= 2.0f;
        return -0.5f * (t * t * t * t - 2.0f);
    }
}

// 五次関数版

// 最初は早いが徐々に減速
float OutQuint(float t)
{
	t -= 1.0f;
	return t * t * t * t * t + 1.0f;
}

// 最初は遅いが徐々に加速
float InQuint(float t)
{
	return t * t * t * t * t;
}

// 最初と最後は早いが途中は遅い
float InOutQuint(float t)
{
	t *= 2.0f;
	if (t < 1.0f)
	{
		return 0.5f * t * t * t * t * t;
	}
	else
	{
		t -= 2.0f;
		return 0.5f * (t * t * t * t * t + 2.0f);
	}
}

// 指数関数版

// 最初は早いが徐々に減速
float OutExpo(float t)
{
	return (t == 1.0f) ? 1.0f : -powf(2.0f, -10.0f * t) + 1.0f;
}

// 最初は遅いが徐々に加速
float InExpo(float t)
{
	return (t == 0.0f) ? 0.0f : powf(2.0f, 10.0f * (t - 1.0f));
}

// 最初と最後は早いが途中は遅い
float InOutExpo(float t)
{
	if (t == 0.0f || t == 1.0f)
	{
		return t;
	}

	t *= 2.0f;
	if (t < 1.0f)
	{
		return 0.5f * powf(2.0f, 10.0f * (t - 1.0f));
	}
	else
	{
		return 0.5f * (-powf(2.0f, -10.0f * (t - 1.0f)) + 2.0f);
	}
}

// 円関数版

// 最初は早いが徐々に減速
float OutCirc(float t)
{
	t -= 1.0f;
	return sqrtf(1.0f - t * t);
}

// 最初は遅いが徐々に加速
float InCirc(float t)
{
	return -(sqrtf(1.0f - t * t) - 1.0f);
}

// 最初と最後は早いが途中は遅い
float InOutCirc(float t)
{
	t *= 2.0f;
	if (t < 1.0f)
	{
		return -0.5f * (sqrtf(1.0f - t * t) - 1.0f);
	}
	else
	{
		t -= 2.0f;
		return 0.5f * (sqrtf(1.0f - t * t) + 1.0f);
	}
}

// バネ関数版

// 最初は早いが徐々に減速
float OutBack(float t, float overshoot)
{
	return t * t * ((overshoot + 1.0f) * t - overshoot);
}

// 最初は遅いが徐々に加速
float InBack(float t, float overshoot)
{
	return t * t * ((overshoot + 1.0f) * t - overshoot);
}

// 最初と最後は早いが途中は遅い
float InOutBack(float t, float overshoot)
{
	t *= 2.0f;
	if (t < 1.0f)
	{
		overshoot *= 1.525f;
		return 0.5f * (t * t * ((overshoot + 1.0f) * t - overshoot));
	}
	else
	{
		t -= 2.0f;
		overshoot *= 1.525f;
		return 0.5f * (t * t * ((overshoot + 1.0f) * t + overshoot) + 2.0f);
	}
}


// 弾性関数版

// 最初は早いが徐々に減速
float InElastic(float t, float freq)
{
	if (t == 0.0f || t == 1.0f)
	{
		return t;
	}

	float period = 1.0f / freq;
	float s = period / 4.0f;
	t -= 1.0f;
	return -(powf(2.0f, 10.0f * t) * sinf((t - s) * XM_2PI / period));
}

// 最初は遅いが徐々に加速
float OutElastic(float t, float freq)
{
	if (t == 0.0f || t == 1.0f)
	{
		return t;
	}

	float period = 1.0f / freq;
	float s = period / 4.0f;
	return powf(2.0f, -10.0f * t) * sinf((t - s) * XM_2PI / period) + 1.0f;
}

// 最初と最後は早いが途中は遅い
float InOutElastic(float t, float freq)
{
	if (t == 0.0f || t == 1.0f)
	{
		return t;
	}

	float period = 1.0f / freq;
	float s = period / 4.0f;
	t *= 2.0f;
	t -= 1.0f;
	if (t < 0.0f)
	{
		return -0.5f * (powf(2.0f, 10.0f * t) * sinf((t - s) * XM_2PI / period));
	}
	else
	{
		return powf(2.0f, -10.0f * t) * sinf((t - s) * XM_2PI / period) * 0.5f + 1.0f;
	}
}

// サイン関数版

// 最初は早いが徐々に減速
float OutBounce(float t)
{
	if (t < 1.0f / 2.75f)
	{
		return 7.5625f * t * t;
	}
	else if (t < 2.0f / 2.75f)
	{
		t -= 1.5f / 2.75f;
		return 7.5625f * t * t + 0.75f;
	}
	else if (t < 2.5f / 2.75f)
	{
		t -= 2.25f / 2.75f;
		return 7.5625f * t * t + 0.9375f;
	}
	else
	{
		t -= 2.625f / 2.75f;
		return 7.5625f * t * t + 0.984375f;
	}
}

// 最初は遅いが徐々に加速
float InBounce(float t)
{
	return 1.0f - OutBounce(1.0f - t);
}

// 最初と最後は早いが途中は遅い
float InOutBounce(float t)
{
	if (t < 0.5f)
	{
		return InBounce(t * 2.0f) * 0.5f;
	}
	else
	{
		return OutBounce(t * 2.0f - 1.0f) * 0.5f + 0.5f;
	}
}


// 直線補間関数
// time=0.0fでstart、time=1.0fでendになるよう直線補間する
XMFLOAT2 Vec2Lerp(XMFLOAT2 start, XMFLOAT2 end, float t)
{
    return XMFLOAT2(
        start.x + (end.x - start.x) * t,
        start.y + (end.y - start.y) * t
    );
}

Vector3 Vec3Lerp(Vector3 start, Vector3 end, float t)
{
	return Vector3(
		start.x + (end.x - start.x) * t,
		start.y + (end.y - start.y) * t,
		start.z + (end.z - start.z) * t
	);
}

// 現在の表示時間を最大時間割った値を計算
// 入力時間が最大時間を超えた場合は1.0f返すようにできてないので注意してください
float GetTimeRate(float time, float max_time)
{

	return time / max_time;
}