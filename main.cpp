#include "main.h"
#include "scene/manager.h"
#include "lib/mouse.h"
#include <thread>


const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "DX11ゲーム";


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


HWND g_Window;
RECT g_Rect;
Mouse g_Mouse;

HWND GetWindow()
{
	return g_Window;
}

RECT GetRect()
{
	// 更新する
	GetClientRect(g_Window, &g_Rect); // クライアント領域(純粋なゲームの画面)のサイズを取得
	return g_Rect;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{


	WNDCLASSEX wcex;
	{
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = 0;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = nullptr;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = nullptr;
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = CLASS_NAME;
		wcex.hIconSm = nullptr;

		RegisterClassEx(&wcex);


		g_Rect = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
		AdjustWindowRect(&g_Rect, WS_OVERLAPPEDWINDOW, FALSE);

		g_Window = CreateWindowEx(0, CLASS_NAME, WINDOW_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			g_Rect.right - g_Rect.left, g_Rect.bottom - g_Rect.top, nullptr, nullptr, hInstance, nullptr);
	}

	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);


	Manager::Init();



	ShowWindow(g_Window, nCmdShow);
	UpdateWindow(g_Window);




	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	timeBeginPeriod(1);
	dwExecLastTime = timeGetTime();
	dwCurrentTime = 0;



	MSG msg;
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				Manager::Update();
				Manager::Draw();
			}
		}
	}

	timeEndPeriod(1);

	UnregisterClass(CLASS_NAME, wcex.hInstance);

	Manager::Uninit();

	CoUninitialize();

	return (int)msg.wParam;
}




LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool updateMouse = false;
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;
	// マウス検知追加
	case WM_LBUTTONDOWN:
		// 左クリック押された
		g_Mouse.SetLeftButtonDown(true);
		break;
	case WM_LBUTTONUP:
		// 左クリック離された
		g_Mouse.SetLeftButtonUp(true);
		break;
	case WM_RBUTTONDOWN:
		// 右クリック押された
		g_Mouse.SetRightButtonDown(true);
		break;
	case WM_RBUTTONUP:
		// 右クリック離された
		g_Mouse.SetRightButtonUp(true);
		break;
	case WM_XBUTTONUP:
		// サイドキー離れた
		switch (GET_XBUTTON_WPARAM(wParam))
		{
		case XBUTTON1:
			// サイドキー1
			g_Mouse.SetSideButton1Up(true);
			break;
		case XBUTTON2:
			// サイドキー2
			g_Mouse.SetSideButton2Up(true);
			break;
		}
		break;
	case WM_XBUTTONDOWN:
		// サイドキー押された
		switch (GET_XBUTTON_WPARAM(wParam))
		{
		case XBUTTON1:
			// サイドキー1
			g_Mouse.SetSideButton1Down(true);
			break;
		case XBUTTON2:
			// サイドキー2
			g_Mouse.SetSideButton2Down(true);
			break;
		}
		break;
	case WM_MOUSEMOVE:
		// マウス動いた
		GetClientRect(hWnd, &g_Rect);
		POINTS pt = MAKEPOINTS(lParam);
		// pt.x, pt.y でマウスの位置を取得し代入
		g_Mouse.AddAccumulatedPosition(Vector2(static_cast<float>(pt.x), static_cast<float>(pt.y)));
		updateMouse = true;
		break;
	case WM_MOUSELEAVE:
		// マウスがウィンドウから出た
		break;
	case WM_MOUSEWHEEL:
		// ホイールが回った
		// wParamのHIWORDで回転量を取得できる
		g_Mouse.SetWheelDiff(static_cast<int>(GET_WHEEL_DELTA_WPARAM(wParam)));
		break;
	default:
		break;
	}

	//if (!updateMouse)
	//{
	//	// マウスの位置更新がなかった場合、現在の位置を再設定しておく
	//	g_Mouse.SetPosition(g_Mouse.GetPosition());
	//}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}