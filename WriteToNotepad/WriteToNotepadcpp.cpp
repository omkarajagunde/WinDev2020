#include<windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK EnumWndProc(HWND, LPARAM);
BOOL CALLBACK EnumChildWndProc(HWND, LPARAM);
TCHAR buff[255];
TCHAR chText[255];
int flag = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szName[] = TEXT("MyApp");

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_CROSS);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.hIconSm = LoadIcon(NULL, IDI_ASTERISK);
	wndclass.lpszClassName = szName;
	wndclass.lpszMenuName = NULL;

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szName, TEXT("Omkar -Ajagunde"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL,

		NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	
	HDC hdc;
	RECT rc;
	PAINTSTRUCT ps;
	
	
	switch (imsg) {
		

	case WM_DESTROY:
		MessageBox(hwnd, TEXT("Sre you sure you want to quit!!"), TEXT("Close Window"), MB_ICONWARNING | MB_OKCANCEL);
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		
		GetKeyNameText(lParam, chText, 255);
		InvalidateRect(hwnd, NULL, true);
		EnumWindows(&EnumWndProc, lParam);
		break;

	case WM_PAINT:

		GetClientRect(hwnd, &rc);
		hdc = BeginPaint(hwnd, &ps);
		SetBkColor(hdc, RGB(0, 0, 0));
		SetTextColor(hdc, RGB(255, 255, 255));
		DrawText(hdc, chText, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);


		EndPaint(hwnd, &ps);

		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}

BOOL CALLBACK EnumWndProc(HWND hWnd, LPARAM lParam) {

	GetClassName(hWnd, buff, 255);
	if (lstrcmp(buff, TEXT("Notepad")) == 0) {
		EnumChildWindows(hWnd, EnumChildWndProc, lParam);
	}
	return true;
}

BOOL CALLBACK EnumChildWndProc(HWND hwnd, LPARAM lparam) {
	

	GetClassName(hwnd, buff, 255);
	if (lstrcmp(buff, TEXT("Edit")) == 0) {
		
		HDC hdc = GetDC(hwnd);
		RECT rc;
		GetClientRect(hwnd, &rc);

		TCHAR ch[255];
		GetKeyNameText(lparam, ch, 255);
		DrawText(hdc, ch, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	}

	return true;

}
