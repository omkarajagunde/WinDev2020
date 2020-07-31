#include<windows.h>
#include "MyHeader.h"
#define COLORTIMER 100
#define RECTIMER 100



LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CALLBACK FlagTimerProc(HWND, UINT, UINT_PTR, DWORD);

int r = 0;
int g = 0;
int b = 0;
int flag = 0;
RECT rcCopy, secondRcCopy;
RECT rc;
HBRUSH hBrush;
HWND ghwnd;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmsLine, int iCmdShow) {


	WNDCLASSEX wndClass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("ZoomOutRectangleApp");


	wndClass.lpszClassName = szAppName;
	wndClass.cbClsExtra = 0;
	wndClass.cbSize = sizeof(wndClass);
	wndClass.hInstance = hInstance;
	wndClass.cbWndExtra = 0;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.lpfnWndProc = WndProc;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.style = NULL;
	wndClass.hCursor = LoadCursor(NULL, IDC_HAND);
	wndClass.lpszMenuName = NULL;

	RegisterClassEx(&wndClass);

	hwnd = CreateWindow(

		szAppName,
		TEXT("ZoomOutReactangleAnimation -OmkarAjagunde ..."),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);


	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return ((int)msg.wParam);


}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {


	HDC hdc, hdcMem;
	PAINTSTRUCT ps;
	static HINSTANCE hInstance;
	HBITMAP hBitmap;
	BITMAP bitmap;

	ghwnd = hwnd;


	switch (iMsg) {

	case WM_CREATE:


		GetClientRect(hwnd, &rc);
		rcCopy = rc;
		hInstance = GetModuleHandle(NULL);
		PlaySound(TEXT("anthem.wav"),NULL, SND_ASYNC);
		SetTimer(hwnd, COLORTIMER, 100, NULL);
		SetTimer(hwnd, RECTIMER, 100, FlagTimerProc);
		break;



	case WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);

		hBrush = CreateSolidBrush(RGB(r, g, b));
		SelectObject(hdc, hBrush);


		SetBkColor(hdc, RGB(r, g, b));
		
		//Rectangle()
		if (flag == 0) {

			FillRect(hdc, &rcCopy, hBrush);
		}
		else {

			//hBrush = CreateSolidBrush(RGB(255, 255, 255));
			//SelectObject(hdc, hBrush);

			FillRect(hdc, &secondRcCopy, hBrush);
			//Ellipse(hdc, rcCopy.left, rcCopy.top, rcCopy.right, rcCopy.bottom);
			hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(MYBITMAP));
			hdcMem = CreateCompatibleDC(NULL);
			SelectObject(hdcMem, hBitmap);
			GetObject(hBitmap, sizeof(BITMAP), &bitmap);
			StretchBlt(hdc, rcCopy.left, rcCopy.top, rc.bottom/3, rc.bottom/3, hdcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
			DeleteObject(hBitmap);
			DeleteDC(hdcMem);
			
		}

		EndPaint(hwnd, &ps);
		break;


	case WM_TIMER:

		KillTimer(hwnd, COLORTIMER);

		if (r < 256) {
			r += 10;
			g = 128;
			b = 255;
		}
		else if (g < 256) {
			r = 255;
			g += 10;
			b = 128;
		}
		else if (b < 256) {
			r = 128;
			g = 255;
			b += 10;
		}



		if (b >= 256)	b = 0;
		else if (g >= 256)	g = 0;
		else if (r >= 256)	r = 0;


		SetTimer(hwnd, COLORTIMER, 100, NULL);
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}


void CALLBACK FlagTimerProc(HWND hwnd, UINT iMsg, UINT_PTR eventMsg, DWORD ms) {


	static int i = 5;
	static int j = 0;
	static int k = 0;
	static int chakraBottom = rc.bottom / 3;
	static int chakraTop = 0;
	static int chakraLeft = rc.right / 2 - ((rc.bottom / 3) / 2);
	static int chakraRight = rc.right / 2 + ((rc.bottom / 3) / 2);

	KillTimer(ghwnd, RECTIMER);

	if (flag == 0) {

		rcCopy.left = 0;
		rcCopy.bottom = i;
		rcCopy.top = j;


		if (i < rc.bottom / 3) {
			r = 255;
			g = 128;
			b = 0;
		}

		else if (i >= (rc.bottom / 3) && i < ((rc.bottom / 3) * 2)) {
			r = 255;
			g = 255;
			b = 255;
		}

		else if (i >= ((rc.bottom / 3) * 2) && i <= rc.bottom) {
			r = 0;
			g = 255;
			b = 0;
		}
		else
			flag = 1;

		i += 5;
		j += 5;
		InvalidateRect(ghwnd, &rcCopy, false);

	}

	if (flag == 1) {

		secondRcCopy = rcCopy;

		rcCopy.left = chakraLeft;
		rcCopy.bottom = chakraBottom;
		rcCopy.top = chakraTop;
		rcCopy.right = chakraRight;

		if (k < rc.bottom / 3) {
			r = 255;
			g = 128;
			b = 0;
		}

		else if (k >= (rc.bottom / 3) && k < ((rc.bottom / 3) * 2)) {
			r = 255;
			g = 255;
			b = 255;
		}

		else if (k >= ((rc.bottom / 3) * 2) && k <= rc.bottom) {
			r = 0;
			g = 255;
			b = 0;
		}
		else
			flag = 1;



		
		chakraTop += 10;
		chakraBottom += 10;
		k += 10;
		InvalidateRect(ghwnd, &secondRcCopy, false);

		if (chakraBottom > ((rc.bottom / 3) * 2)) {
			flag = 3;
			
		}

	}

	SetTimer(ghwnd, RECTIMER, 100, FlagTimerProc);


}

