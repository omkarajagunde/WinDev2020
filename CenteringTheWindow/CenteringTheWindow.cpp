#include <windows.h>


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

	int xCalc = GetSystemMetrics(SM_CXSCREEN);
	int yCalc = GetSystemMetrics(SM_CYSCREEN);

	xCalc -= 800;
	xCalc /= 2;

	yCalc -= 600;
	yCalc /= 2;

	hwnd = CreateWindow(
		szAppName,
		TEXT("Omkar Ajagunde ..."),
		WS_OVERLAPPEDWINDOW,
		xCalc,
		yCalc,
		800,
		600,
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

	switch (iMsg) {

	case WM_CREATE:
		break;

	case WM_DESTROY:
		
		PostQuitMessage(0);
		break;

	case WM_LBUTTONDOWN:
		MessageBox(hwnd, TEXT("Window Is Centered To Desktop ..."), TEXT("Message!"), MB_OKCANCEL);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
