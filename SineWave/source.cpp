#include<windows.h>
#include<math.h>

#define NUM 1000
#define TWOPI (2 * 3.14159)
#define MYTIMER 2000
#define MYTIMERTWO 4000

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CALLBACK ChangeTimer(HWND, UINT, UINT_PTR, DWORD);

static float scale;
static float waveLength;

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	WNDCLASSEX wndClass;
	MSG msg;
	TCHAR szAppName[] = TEXT("SineWaveProgram");
	HWND hwnd;

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_HAND);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = szAppName;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wndClass);

	hwnd = CreateWindow(
	
		szAppName,
		TEXT("SineWave Program CHARLES-PETZOLD"),
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

	static int cxClient, cyClient;
	static HDC hdc;
	int i;
	PAINTSTRUCT ps;
	static POINT *apt = NULL;
	static POINT* aptTwo = NULL;
	HBRUSH hBrush;
	static RECT rc;
	HPEN hPen;

	static float inc = 0.005;

	switch (iMsg) {

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		GetClientRect(hwnd, &rc);
		apt = (POINT*)malloc(sizeof(POINT)*(cxClient+3));
		aptTwo = (POINT*)malloc(sizeof(POINT) * (cxClient + 3));
		return 0;

	case WM_CREATE:
		scale = rand() / (float)RAND_MAX; /* [0, 1.0] */
		 waveLength = -0.01 + scale * (0.01 - (-0.01));      /* [min, max] */
		 GetClientRect(hwnd, &rc);
		SetTimer(hwnd, MYTIMER,100,  NULL);
		//SetTimer(hwnd, MYTIMERTWO, 100, ChangeTimer);
		break;

	case WM_TIMER:


		KillTimer(hwnd, MYTIMER);
		for (i = 0; i < cxClient; i++) {
			apt[i].x = i;
			apt[i].y = cyClient / 3 + sin(i * waveLength + inc) * 20;
			inc += 0.001;
		}

		apt[cxClient].x = rc.right;
		apt[cxClient].y = rc.bottom;

		apt[cxClient + 1].x = rc.left;
		apt[cxClient + 1].y = rc.bottom;

		apt[cxClient + 2].x = apt[0].x;
		apt[cxClient + 2].y = apt[0].y;

		for (i = 0; i < cxClient; i++) {
			aptTwo[i].x = i;
			aptTwo[i].y = apt[i].y + rc.bottom / 3;

		}

		aptTwo[cxClient].x = rc.right;
		aptTwo[cxClient].y = rc.bottom / 3 * 3;

		aptTwo[cxClient + 1].x = rc.left;
		aptTwo[cxClient + 1].y = rc.bottom / 3 * 3;

		aptTwo[cxClient + 2].x = aptTwo[0].x;
		aptTwo[cxClient + 2].y = aptTwo[0].y;

		InvalidateRect(hwnd, NULL, TRUE);
		SetTimer(hwnd, MYTIMER, 200, NULL);

		break;

	case WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);
		hPen = (HPEN)GetStockObject(NULL_PEN);
		SelectObject(hdc, hPen);



		hBrush = CreateSolidBrush(RGB(255, 128, 0));
		SelectObject(hdc, hBrush);
		FillRect(hdc, &rc, hBrush);
		
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, hBrush);

		Polygon(hdc, apt, (cxClient+3));

		hBrush = CreateSolidBrush(RGB(0, 255, 0));
		SelectObject(hdc, hBrush);
		Polygon(hdc, aptTwo, (cxClient + 3));
		break;

	case WM_DESTROY:
		free(apt);
		apt = NULL;
		free(aptTwo);
		aptTwo = NULL;
		PostQuitMessage(0);
		break;
	}
	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void CALLBACK ChangeTimer(HWND hwnd, UINT iMsg, UINT_PTR tiemrEvent, DWORD ms) {

	switch (iMsg) {
	case WM_TIMER:

		KillTimer(hwnd, MYTIMERTWO);
		scale = rand() / (float)RAND_MAX; /* [0, 1.0] */
		waveLength = -0.01 + scale * (0.01 - (-0.01));      /* [min, max] */
		SetTimer(hwnd, MYTIMERTWO, 2000, ChangeTimer);
		break;
	}
	
}