#include<windows.h>

#define MYTIMER 200

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

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
		TEXT("Taurus Sign"),
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

	ShowWindow(hwnd, SW_MAXIMIZE);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return ((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {


	static RECT rc;
	static POINT pts[12];
	static HDC hdc;
	HBRUSH hBrush;
	HPEN hPen;
	PAINTSTRUCT ps;
	static float scaleFactor = 0.002f;
	static int flag = 0;
	static XFORM xForm;
	static int counter = 0;
	TCHAR str[20];
	static POINT stars[300];

	switch (iMsg) {

	case WM_CREATE:
	case WM_SIZE:
		GetClientRect(hwnd, &rc);
		pts[0].x = -rc.right / 4 - rc.right/8 - 90;
		pts[0].y = -rc.bottom / 4 - rc.bottom / 8 - 95;

		pts[1].x = -rc.right / 8 + 50;
		pts[1].y = -rc.bottom / 4 + 50 - 20;

		pts[2].x = -10;
		pts[2].y = -rc.bottom/4 + rc.bottom/8 + 50;

		pts[3].x = 0;
		pts[3].y = 0;

		pts[4].x = rc.right / 8;
		pts[4].y = rc.bottom / 8;

		pts[5].x = rc.right / 4 ;
		pts[5].y = rc.bottom / 8 + 40;

		pts[6].x = rc.right / 8;
		pts[6].y = rc.bottom / 8 + 100;


		pts[7].x = rc.right / 4;
		pts[7].y = rc.bottom / 8 + 40;

		pts[8].x = rc.right / 8;
		pts[8].y = rc.bottom / 8;

		pts[9].x = 0;
		pts[9].y = 0;

		pts[10].x = -rc.right / 8;
		pts[10].y = 0;

		pts[11].x = -rc.right / 4 - rc.right / 8 - 75;
		pts[11].y = -rc.bottom/4 + rc.bottom/8;
		

		xForm.eM11 = (FLOAT)0.000;
		xForm.eM12 = (FLOAT)0.0;
		xForm.eM21 = (FLOAT)0.0;
		xForm.eM22 = (FLOAT)0.000;
		xForm.eDx = (FLOAT)0.00;
		xForm.eDy = (FLOAT)0.00;

		for (int i = 0; i < 300; i++) {
			stars[i].x = rand() % rc.right;
			stars[i].y = rand() % rc.bottom;

		}

		SetTimer(hwnd, MYTIMER, 500, NULL);
		break;

	case WM_TIMER:

		KillTimer(hwnd, MYTIMER);
		

		xForm.eM11 += (FLOAT)scaleFactor;
		xForm.eM22 += (FLOAT)scaleFactor;

		//scaleFactor += 0.0005;

		for (int i = 0; i < 500; i++) {
			stars[i].x = rand() % rc.right;
			stars[i].y = rand() % rc.bottom;

		}
		
		flag = 1;
		counter++;
		if (counter < 400)
			SetTimer(hwnd, MYTIMER, 20, NULL);
		else if (counter < 500)
			SetTimer(hwnd, MYTIMER, 40, NULL);
		else if (counter < 600){
			SetTimer(hwnd, MYTIMER, 10, NULL);
			scaleFactor += 0.0015;
		}
		else if (counter < 948){
			SetTimer(hwnd, MYTIMER, 10, NULL);
			scaleFactor -= 0.001;
		}
		InvalidateRect(hwnd, NULL, true);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		for (int i = 0; i < 300; i++)
			SetPixel(hdc, stars[i].x, stars[i].y, RGB(255, 255, 255));
		
		SetViewportOrgEx(hdc, rc.right / 2, rc.bottom / 2, NULL);
		SetGraphicsMode(hdc, GM_ADVANCED);
		
		SetWorldTransform(hdc, &xForm);

		hPen = CreatePen(PS_SOLID, 2, RGB(5, 250, 18));
		SelectObject(hdc, hPen);
		SetTextColor(hdc, RGB(5, 250, 18));
		SetBkColor(hdc, RGB(0, 0, 0));
		
		if (flag == 1)
		Polyline(hdc, pts, 12);

		if (counter == 948) {
			DrawText(hdc, TEXT("Taurus Sign"), -1, &rc, DT_VCENTER | DT_SINGLELINE);
		}
		
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, hBrush);
		hPen = (HPEN)GetStockObject(NULL_PEN);
		SelectObject(hdc, hPen);
		if (flag == 1) {
			for (int i = 0; i < 12; i++)
				Ellipse(hdc, pts[i].x - 5, pts[i].y - 5, pts[i].x + 5, pts[i].y + 5);

			Ellipse(hdc, pts[10].x - 10, pts[10].y - 10, pts[10].x + 10, pts[10].y + 10);
		}

		
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}