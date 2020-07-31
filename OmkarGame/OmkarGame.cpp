#include <windows.h>
#include <math.h>
#include <Mmsystem.h>
#include "MyGameHeader.h"
#define TWOPI (2 * 3.14159)
#define MYTIMER 300

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void drawShooter(HWND, HDC, int);
void calcFireBallPoints(HWND);


RECT rc;
RECT rcShooter;
HBRUSH hBrush;
TCHAR buff[40];
POINT pt[5];
RECT rect;
RECT fireBallRect, prevFireBallRect;
int topPt = 10;
int bottomPt = 5;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	WNDCLASSEX wndClass;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyOmkarGame");
	HWND hwnd;

	wndClass.cbSize = sizeof(wndClass);
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_HAND);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpszClassName = szAppName;
	wndClass.hInstance = hInstance;

	RegisterClassEx(&wndClass);

	hwnd = CreateWindow(
		szAppName,
		TEXT("Omkar Ajagunde ..."),
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

	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	HDC hdc = NULL;
	PAINTSTRUCT ps;

	HBITMAP hBitmap = NULL;
	HDC hdcmem = NULL;
	BITMAP bitmap;
	static HINSTANCE hInstance;
	static int backArtInit = 1;
	static int shooterAngle = 0;
	static RECT shooterRect;
	static int shootFlag = 0;

	switch (iMsg) {

	case WM_CREATE:
		hInstance = GetModuleHandle(NULL);
		SetTimer(hwnd, MYTIMER, 10, NULL);
		hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(BACKBITMAP));
		hdcmem = CreateCompatibleDC(NULL);
		break;

	case WM_DESTROY:
		DeleteObject(hBitmap);
		DeleteDC(hdcmem);
		PostQuitMessage(0);
		break;

	case WM_TIMER:

		KillTimer(hwnd, MYTIMER);
		if (shootFlag == 1)
			calcFireBallPoints(hwnd);
		InvalidateRect(hwnd, &prevFireBallRect, true);
		SetTimer(hwnd, MYTIMER, 10, NULL);
		break;

	case WM_KEYDOWN:
		// Temprory variable Need to find proper solution for this var
		//shootFlag = 0;

		if (wParam == 0x27) {
			//shootFlag = 0;
			shooterAngle -= 5;
			if (shooterAngle <= -90)
				shooterAngle = -90;
			PlaySound(TEXT("tank.wav"),NULL,SND_FILENAME | SND_ASYNC);
			//topPt = 10;
			//bottomPt = 5;
			InvalidateRect(hwnd, &rc, false);
		}

		if (wParam == 0x25) {
			//shootFlag = 0;
			shooterAngle += 5;
			if (shooterAngle >= 90 )
				shooterAngle = 90;
			PlaySound(TEXT("tank.wav"), NULL, SND_FILENAME | SND_ASYNC);
			//topPt = 10;
			//bottomPt = 5;
			InvalidateRect(hwnd, &rc, false);
		}

		if (wParam == 0x26) {
			shootFlag = 1;
			//calcFireBallPoints(hwnd);
		}
		break;



	case WM_PAINT:
		//MessageBox(hwnd, TEXT("arrow Keys"), TEXT("keys Event"), MB_OKCANCEL);
		
		wsprintf(buff,TEXT("                       Angle  %d"),-shooterAngle);
		GetClientRect(hwnd, &rc);
		hdc = BeginPaint(hwnd, &ps);

		// initialise background -art
		
		SelectObject(hdcmem, hBitmap);
		GetObject(hBitmap, sizeof(BITMAP), &bitmap);
		StretchBlt(hdc, 0, 0, rc.right, rc.bottom, hdcmem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

			
			
		// shooter -functions
		hBrush = CreateSolidBrush(RGB(128, 128, 255));
		SelectObject(hdc, hBrush);
		drawShooter(hwnd, hdc, shooterAngle);

		if (shootFlag == 1) {
			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			SelectObject(hdc, hBrush);
			Ellipse(hdc, fireBallRect.left, fireBallRect.top, fireBallRect.right, fireBallRect.bottom);
		}
		
		hBrush = CreateSolidBrush(RGB(255, 128, 128));
		SelectObject(hdc, hBrush);

		// Angle text show
		DrawText(hdc,buff,40,&rc, DT_SINGLELINE);
		// Show earth glowing ...
		Ellipse(hdc, - 80,  - 80, 80,  80);

		EndPaint(hwnd, &ps);
		break;
	}


	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}


void drawShooter(HWND hwnd, HDC hdc, int angle) {

	int xPt, yPt;

	SetViewportOrgEx(hdc, rc.right/2, rc.bottom - 25, NULL);
	
	pt[0].x = -20;
	pt[0].y = 0;

	xPt = - 20;
	yPt = - 150;
	pt[1].x = (int)(xPt * cos(TWOPI * angle / 360) + yPt * sin(TWOPI * angle / 360));
	pt[1].y = (int)(yPt * cos(TWOPI * angle / 360) - xPt * sin(TWOPI * angle / 360));

	xPt =  + 20;
	yPt =  - 150;
	pt[2].x = (int)(xPt * cos(TWOPI * angle / 360) + yPt * sin(TWOPI * angle / 360));
	pt[2].y = (int)(yPt * cos(TWOPI * angle / 360) - xPt * sin(TWOPI * angle / 360));

	pt[3].x = +20;
	pt[3].y = 0;

	pt[4].x = -20;
	pt[4].y = 0;


	rect.left = pt[1].x;
	rect.top = pt[1].y;
	rect.right = pt[3].x;
	rect.bottom = pt[3].y;
	InvalidateRect(hwnd, &rect, true);
	
	Polygon(hdc, pt, 5);
}

void calcFireBallPoints(HWND hwnd) {

	

	// Slope of a line m = y2 -y1/ x2 -x1
	float arbitrarySlope = (pt[1].y - pt[0].y) / (pt[1].x - pt[0].x);
	if (arbitrarySlope == 0.0f)
		arbitrarySlope = 1.0f;

	fireBallRect.left = pt[1].x + topPt;
	fireBallRect.top = pt[1].y + (topPt * arbitrarySlope);

	fireBallRect.right = fireBallRect.left + 50;
	fireBallRect.bottom = fireBallRect.top + 50;

	prevFireBallRect = fireBallRect;
	topPt += 10;
	bottomPt += 10;

	if (topPt > rc.right/2)
		topPt = 10;
	if (bottomPt > rc.bottom)
		bottomPt = 5;


}

