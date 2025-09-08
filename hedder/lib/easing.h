#pragma once

#include "main.h"
#include "lib/vector3.h"
//#include "lib/renderer.h"

// 参考
// https://easings.net/ja
// 説明
// https://ics.media/entry/18730/

// イージング描画用構造体
struct EasingDrawData2D
{
	XMFLOAT2 StartPos;
	XMFLOAT2 EndPos;
	float Time;
	float TimeCounter;
};

struct EasingDrawData3D
{
	Vector3 StartPos;
	Vector3 EndPos;
	float Time;
	float TimeCounter;
};


// 直線補間関数
XMFLOAT2 Vec2Lerp(XMFLOAT2 start, XMFLOAT2 end, float t);
Vector3 Vec3Lerp(Vector3 start, Vector3 end, float t);
// 現在の表示時間を最大時間割った値を計算
float GetTimeRate(float time, float max_time);

// sine(弱)
// 最初は早いが徐々に減速
float OutSine(float t);
// 最初は遅いが徐々に加速
float InSine(float t);
// 最初と最後は早いが途中は遅い
float InOutSine(float t);

// Quad(二次関数)
// 最初は早いが徐々に減速
float OutQuad(float t);
// 最初は遅いが徐々に加速
float InQuad(float t);
// 最初と最後は早いが途中は遅い
float InOutQuad(float t);

// Cubic(三次関数)
// 最初は早いが徐々に減速
float OutCubic(float t);
// 最初は遅いが徐々に加速
float InCubic(float t);
// 最初と最後は早いが途中は遅い
float InOutCubic(float t);

// Quart(四次関数)
// 最初は早いが徐々に減速
float OutQuart(float t);
// 最初は遅いが徐々に加速
float InQuart(float t);
// 最初と最後は早いが途中は遅い
float InOutQuart(float t);

// Quint(五次関数)
// 最初は早いが徐々に減速
float OutQuint(float t);
// 最初は遅いが徐々に加速
float InQuint(float t);
// 最初と最後は早いが途中は遅い
float InOutQuint(float t);

// Expo(強)
// 最初は早いが徐々に減速
float OutExpo(float t);
// 最初は遅いが徐々に加速
float InExpo(float t);
// 最初と最後は早いが途中は遅い
float InOutExpo(float t);

// Circ(円)
// 最初は早いが徐々に減速
float OutCirc(float t);
// 最初は遅いが徐々に加速
float InCirc(float t);
// 最初と最後は早いが途中は遅い
float InOutCirc(float t);

// Back(バネ)
// 最初は早いが徐々に減速
float OutBack(float t, float overshoot = 1.70158f);
// 最初は遅いが徐々に加速
float InBack(float t, float overshoot = 1.70158f);
// 最初と最後は早いが途中は遅い
float InOutBack(float t, float overshoot = 1.70158f);

// Elastic(弾性)
// 最初は早いが徐々に減速
float InElastic(float t, float freq = 1.0f);
// 最初は遅いが徐々に加速
float OutElastic(float t, float freq = 1.0f);
// 最初と最後は早いが途中は遅い
float InOutElastic(float t, float freq = 1.0f);

// Bounce(バウンド)
// 最初は早いが徐々に減速
float OutBounce(float t);
// 最初は遅いが徐々に加速
float InBounce(float t);
// 最初と最後は早いが途中は遅い
float InOutBounce(float t);