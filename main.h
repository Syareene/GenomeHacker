#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define NOMINMAX
#include <windows.h>
#include <assert.h>
#include <functional>

#include <d2d1_1.h>
#include <d3d11.h>
#pragma comment(lib,"d2d1.lib")
#pragma comment (lib, "d3d11.lib")


#include <DirectXMath.h>
using namespace DirectX;

#include "lib/vector2.h"
#include "lib/vector3.h"
#include "lib/DirectXTex.h"

#if _DEBUG
#pragma comment(lib, "hedder/lib/DirectXTex_Debug.lib")
#else
#pragma comment(lib, "hedder/lib/DirectXTex_Release.lib")
#endif






#pragma comment (lib, "winmm.lib")


#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)


HWND GetWindow();

void Invoke(std::function<void()> Function, int Time);
RECT GetRect();

