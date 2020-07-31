 #include <windows.h>
#define MYTIMER 102


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


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


	static HBRUSH hBrush[8];
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rc;

	static int iPaintFlag = 0;

	switch (iMsg) {

	case WM_CREATE:

		hBrush[0] = CreateSolidBrush(RGB(255, 0, 0));
		hBrush[1] = CreateSolidBrush(RGB(0, 255, 0));
		hBrush[2] = CreateSolidBrush(RGB(0, 0, 255));
		hBrush[3] = CreateSolidBrush(RGB(255, 255, 0));
		hBrush[4] = CreateSolidBrush(RGB(255, 0, 255));
		hBrush[5] = CreateSolidBrush(RGB(0, 255, 255));
		hBrush[6] = CreateSolidBrush(RGB(255, 128, 0));
		hBrush[7] = CreateSolidBrush(RGB(0, 128, 255));

		SetTimer(hwnd, MYTIMER, 100, NULL);

		break;

	case WM_PAINT:

		GetClientRect(hwnd, &rc);
		hdc = BeginPaint(hwnd, &ps);

		SelectObject(hdc, hBrush[iPaintFlag]);
		FillRect(hdc, &rc, hBrush[iPaintFlag]);

		EndPaint(hwnd, &ps);
		break;

	case WM_TIMER:

		KillTimer(hwnd, MYTIMER);

		iPaintFlag++;
		if (iPaintFlag > 7)
			iPaintFlag = 0;
		
		InvalidateRect(hwnd, NULL, true);
		SetTimer(hwnd, MYTIMER, 100, NULL);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_LBUTTONDOWN:
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
