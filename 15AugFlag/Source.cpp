#include<windows.h>
#include<math.h>

#define TIMER_FLAG 100
#define TIMER_BACK_DOTS 102
#define TIMER_WAVING_FLAG 104

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CALLBACK TimerFlagProc(HWND, UINT, UINT_PTR, DWORD);
void CALLBACK TimerBackProc(HWND, UINT, UINT_PTR, DWORD);
void CALLBACK TimerFlagWaveProc(HWND, UINT, UINT_PTR, DWORD);

static RECT rc;
static RECT inviRect;
PAINTSTRUCT ps;
HDC hdc;
int flagStartX;
int flagEndX;
int drawFlag = 0;
static POINT *ptsOne = NULL;
static POINT *ptsTwo = NULL;
static float scale;
static float waveLength;

static int divideFactor = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	WNDCLASSEX wndClass;
	MSG msg;
	HWND hwnd;
	TCHAR szAppName[] = TEXT("15AugFlag");

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.cbWndExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hCursor = LoadCursor(NULL, IDC_HAND);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszClassName = szAppName;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hInstance = hInstance;

	RegisterClassEx(&wndClass);

	int xCalc = GetSystemMetrics(SM_CXMAXIMIZED);
	int yCalc = GetSystemMetrics(SM_CYMAXIMIZED);

	hwnd = CreateWindow(
		
		szAppName, 
		TEXT("Happy Independence Day 2k20"),
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		xCalc,
		yCalc,
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
	
	HBRUSH hBrush;
	RECT rect;
	HPEN hPen;

	switch (iMsg) {

	case WM_CREATE:
		
		GetClientRect(hwnd, &rc);
		flagStartX = rc.right / 2;
		flagEndX = rc.right / 2 + rc.right / 4 + 150;
		divideFactor = ((rc.right / 2 + rc.right / 4 + 150) - (rc.right / 2)) / 10;
		scale = rand() / (float)RAND_MAX; // [0, 1.0] 
		waveLength = -0.01 + scale * (0.01 - (-0.01));     

		inviRect.top = rc.bottom / 4;
		inviRect.left = rc.right / 2;
		inviRect.right = rc.right / 2 + rc.right / 4 + 150;
		inviRect.bottom = rc.bottom / 4 + (((rc.bottom / 2) / 4) * 3);

		ptsOne = (POINT*)malloc(sizeof(POINT) * (flagEndX - flagStartX));
		ptsTwo = (POINT*)malloc(sizeof(POINT) * (flagEndX - flagStartX));

		SetTimer(hwnd, TIMER_BACK_DOTS, 100, TimerBackProc);
		SetTimer(hwnd, TIMER_FLAG, 100, TimerFlagProc);
		
		break;

	

	case WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rc);
		flagStartX = rc.right / 2;
		flagEndX = rc.right / 2 + rc.right / 4 + 150;
		inviRect.top = rc.bottom / 4;
		inviRect.left = rc.right / 2;
		inviRect.right = rc.right / 2 + rc.right / 4 + 150;
		inviRect.bottom = rc.bottom / 4 + (((rc.bottom / 2) / 4) * 3);

		if (drawFlag == 1) {
			
			hPen = (HPEN)GetStockObject(NULL_PEN);
			SelectObject(hdc, hPen);

			hBrush = CreateSolidBrush(RGB(255, 128, 0));
			SelectObject(hdc, hBrush);
			FillRect(hdc, &inviRect, hBrush);

			hBrush = CreateSolidBrush(RGB(255, 255, 255));
			SelectObject(hdc, hBrush);

			Polygon(hdc, ptsOne, (flagEndX - flagStartX));

			hBrush = CreateSolidBrush(RGB(0, 255, 0));
			SelectObject(hdc, hBrush);
			Polygon(hdc, ptsTwo, (flagEndX - flagStartX));

			hBrush = CreateSolidBrush(RGB(0, 0, 255));
			SelectObject(hdc, hBrush);
			Ellipse(hdc, (flagStartX + (flagEndX - flagStartX)/2) - 20, rc.bottom / 4 + ((rc.bottom / 2) / 4) + 20, (flagStartX + (flagEndX - flagStartX) / 2) + 20, rc.bottom / 4 + (((rc.bottom / 2) / 4) * 2) - 20);

		}

		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		free(ptsOne);
		ptsOne = NULL;
		free(ptsTwo);
		ptsTwo = NULL;
		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}


void CALLBACK TimerFlagProc(HWND hwnd, UINT iMsg, UINT_PTR timerEvent, DWORD ms) {

	static RECT rectDxDy;
	static int oColorFilled = 0;
	static int wColorFilled = 0;
	static int gColorFilled = 0;
	static HBRUSH hBrush;

	static int right = 8;

	KillTimer(hwnd, TIMER_FLAG);
	
	switch (iMsg) {

	case WM_TIMER:

		hdc = GetDC(hwnd);

		if (oColorFilled != 1) {
			// Drawing Orangle Color ...
			hBrush = CreateSolidBrush(RGB(255, 128, 0));
			SelectObject(hdc, hBrush);

			if (rectDxDy.right <= flagEndX) {
				rectDxDy.left = rc.right / 2;
				rectDxDy.top = rc.bottom / 4;
				rectDxDy.bottom = rc.bottom / 4 + ((rc.bottom / 2) / 4);

				rectDxDy.right = rc.right / 2 + right;
				FillRect(hdc, &rectDxDy, hBrush);

			}
			else {
				right = 8;
				oColorFilled = 1;
				rectDxDy.right = rc.right / 2;
			}
		}
		else if (wColorFilled != 1) {
			// Drawing White Color ...
			hBrush = CreateSolidBrush(RGB(255, 255, 255));
			SelectObject(hdc, hBrush);

			if (rectDxDy.right <= flagEndX) {
				rectDxDy.left = rc.right / 2;
				rectDxDy.top = rc.bottom / 4 + ((rc.bottom / 2) / 4);
				rectDxDy.bottom = rc.bottom / 4 + (((rc.bottom / 2) / 4) * 2);

				rectDxDy.right = rc.right / 2 + right;
				FillRect(hdc, &rectDxDy, hBrush);

			}
			else {
				right = 8;
				wColorFilled = 1;
				rectDxDy.right = rc.right / 2;
			}
		}
		else if (gColorFilled != 1) {
			// Drawing Green Color ...
			hBrush = CreateSolidBrush(RGB(0, 255, 0));
			SelectObject(hdc, hBrush);

			if (rectDxDy.right <= flagEndX) {
				rectDxDy.left = rc.right / 2;
				rectDxDy.top = rc.bottom / 4 + (((rc.bottom / 2) / 4) * 2);
				rectDxDy.bottom = rc.bottom / 4 + (((rc.bottom / 2) / 4) * 3);

				rectDxDy.right = rc.right / 2 + right;
				FillRect(hdc, &rectDxDy, hBrush);

			}
			else {
				right = 8;
				gColorFilled = 1;
			}
		}

		right += 10;


		if (oColorFilled == 1 && wColorFilled == 1 && gColorFilled == 1) {
			KillTimer(hwnd, TIMER_FLAG);
			ReleaseDC(hwnd, hdc);
		}
		else
			SetTimer(hwnd, TIMER_FLAG, 100, TimerFlagProc);

		break;
	}
	
}

void CALLBACK TimerBackProc(HWND hwnd, UINT iMsg, UINT_PTR timerEvent, DWORD ms) {

	static int right = divideFactor;
	static RECT rectDots;
	static int oColorFilled = 0;
	static int wColorFilled = 0;
	static int gColorFilled = 0;


	KillTimer(hwnd, TIMER_BACK_DOTS);

	switch (iMsg) {

	case WM_TIMER:

		hdc = GetDC(hwnd);


		if (oColorFilled != 1) {
			// Drawing Orangle Background Dots ...

			if (rectDots.right <= rc.right) {
				rectDots.left = rc.left;
				rectDots.top = rc.top;
				rectDots.right = right;
				rectDots.bottom = rc.bottom / 4 + ((rc.bottom / 2) / 4);

				for (int i = 0; i < 100; i++) {
					SetPixel(hdc,
						rand() % rectDots.right,
						rand() % rectDots.bottom,
						RGB(255, 128, 0));
				}

			}
			else {
				right = 0;
				oColorFilled = 1;
				rectDots.right = rc.right;
			}
		}
		else if (wColorFilled != 1) {
			// Drawing White Background Dots ...

			if (rectDots.right <= rc.right) {
				rectDots.left = rc.left;
				rectDots.right = right;
				rectDots.top = rc.bottom / 4 + ((rc.bottom / 2) / 4);
				rectDots.bottom = rc.bottom / 4 + (((rc.bottom / 2) / 4) * 2);

				for (int i = 0; i < 50; i++) {
					SetPixel(hdc,
						rand() % rectDots.right,
						(rand() % (rectDots.bottom - rectDots.top)) + rectDots.top,
						RGB(255, 255, 255));
				}

			}
			else {
				right = divideFactor;
				wColorFilled = 1;
				rectDots.right = rc.right;
			}
		}
		else if (gColorFilled != 1) {
			// Drawing Green Background Dots ...

			if (rectDots.right <= rc.right) {
				rectDots.left = rc.left;
				rectDots.right = right;
				rectDots.top = rc.bottom / 4 + (((rc.bottom / 2) / 4) * 2);
				rectDots.bottom = rc.bottom;

				for (int i = 0; i < 200; i++) {
					SetPixel(hdc,
						rand() % rectDots.right,
						(rand() % (rectDots.bottom - rectDots.top)) + rectDots.top,
						RGB(0, 255, 0));
				}


			}
			else {
				right = divideFactor;
				gColorFilled = 1;
			}
		}

		right += divideFactor;


		if (oColorFilled == 1 && wColorFilled == 1 && gColorFilled == 1) {
			KillTimer(hwnd, TIMER_BACK_DOTS);
			ReleaseDC(hwnd, hdc);
			drawFlag = 1;
			//InvalidateRect(hwnd, &inviRect, TRUE);
			SetTimer(hwnd, TIMER_WAVING_FLAG, 200, TimerFlagWaveProc);
		}
		else
			SetTimer(hwnd, TIMER_BACK_DOTS, 200, TimerBackProc);

		break;
	}
}


void CALLBACK TimerFlagWaveProc(HWND hwnd, UINT iMsg, UINT_PTR timerEvent, DWORD ms) {


	static float inc = 0.005;

	KillTimer(hwnd, TIMER_WAVING_FLAG);

	for (int i = 0; i < (flagEndX - flagStartX - 3); i++) {
		ptsOne[i].x = rc.right/2 + i;														// x3, y3 -> travelling to x4, y4 while tracig curve
		ptsOne[i].y = (rc.bottom/4 + ((rc.bottom/2)/4)) + sin(i * waveLength + inc) * 35;		
		inc += 0.001;
	}

	// In order to make a enclosed figure thsese additional paths are drawn
	ptsOne[(flagEndX - flagStartX - 3)].x = rc.right / 2 + rc.right / 4 + 150;	//x8
	ptsOne[(flagEndX - flagStartX - 3)].y = rc.bottom / 4 + rc.bottom / 2;		//y8

	ptsOne[(flagEndX - flagStartX - 2)].x = rc.right/2;							//x7
	ptsOne[(flagEndX - flagStartX - 2)].y = rc.bottom / 4 + rc.bottom / 2;		//y7

	ptsOne[(flagEndX - flagStartX - 1)].x = ptsOne[0].x;						//x3		
	ptsOne[(flagEndX - flagStartX - 1)].y = ptsOne[0].y;						//y3

	for (int i = 0; i < (flagEndX - flagStartX - 3); i++) {
		ptsTwo[i].x = ptsOne[i].x ;												// x5, y5 -> travelling to x6, y6 while tracing curve
		ptsTwo[i].y = ptsOne[i].y + ((rc.bottom / 2) / 4);						

	}

	// In order to make a enclosed figure thsese additional paths are drawn
	ptsTwo[(flagEndX - flagStartX - 3)].x = rc.right / 2 + rc.right / 4 + 150;	//x8	
	ptsTwo[(flagEndX - flagStartX - 3)].y = rc.bottom / 4 + rc.bottom / 2;		//y8

	ptsTwo[(flagEndX - flagStartX - 2)].x = rc.right/2;							//x7
	ptsTwo[(flagEndX - flagStartX - 2)].y = rc.bottom / 4 + rc.bottom / 2;		//y7

	ptsTwo[(flagEndX - flagStartX - 1)].x = ptsTwo[0].x;						//x5
	ptsTwo[(flagEndX - flagStartX - 1)].y = ptsTwo[0].y;						//y5

	InvalidateRect(hwnd, &inviRect, TRUE);

	SetTimer(hwnd, TIMER_WAVING_FLAG, 100, TimerFlagWaveProc);
}


