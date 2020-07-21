#include<windows.h>
#include "MyHeader.h"



LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDialogProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE ghInstance;
int flag;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmsLine, int iCmdShow) {

	
	HMENU hMenu;
	WNDCLASSEX wndClass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("ZoomOutRectangleApp");

	ghInstance = hInstance;
	hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(MYMENU));

	wndClass.lpszClassName = szAppName;
	wndClass.cbClsExtra = 0;
	wndClass.cbSize = sizeof(wndClass);
	wndClass.hInstance = hInstance;
	wndClass.cbWndExtra = 0;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.lpfnWndProc = WndProc;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.style = CS_HREDRAW | CS_HREDRAW;
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
		hMenu,
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

	static HBITMAP hBitmap;
	HDC hdc;
	HDC hdcMem;
	PAINTSTRUCT ps;
	RECT rc;
	BITMAP bitmap;
	static HINSTANCE hInstance;

	switch (iMsg) {

	case WM_CREATE:
		hInstance = GetModuleHandle(NULL);
		break;

	case WM_COMMAND:

		switch (LOWORD(wParam)) {

		case IDM_HORSE:
			flag = 1;
			InvalidateRect(hwnd, NULL, true);
			break;

		case IDM_CRICKET:
			flag = 2;
			InvalidateRect(hwnd, NULL, true);
			break;

		case IDM_FOOTBALL:
			flag = 3;
			InvalidateRect(hwnd, NULL, true);
			break;

		case IDM_BEACH:
			flag = 4;
			InvalidateRect(hwnd, NULL, true);
			break;

		case IDM_ASTROMEDICOMP:
			flag = 5;
			InvalidateRect(hwnd, NULL, true);
			break;

		case IDM_APPABOUT:
			DialogBox(ghInstance, TEXT("about"), hwnd, AboutDialogProc);
			break;
		}

	case WM_PAINT:
		GetClientRect(hwnd, &rc);
		hdc = BeginPaint(hwnd, &ps);
		hdcMem = CreateCompatibleDC(NULL);

		if (flag == 1)
			hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(HORSE));

		if (flag == 2)
			hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(CRICKET));

		if (flag == 3)
			hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(FOOTBALL));

		if (flag == 4)
			hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(BEACH));

		if (flag == 5)
			hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(ASTROMEDICOMP));
		
		
		SelectObject(hdcMem, hBitmap);
		GetObject(hBitmap, sizeof(BITMAP), &bitmap);
		StretchBlt(hdc,0,0,rc.right, rc.bottom, hdcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
		DeleteObject(hBitmap);
		DeleteDC(hdcMem);
		EndPaint(hwnd, &ps);
		break;

	
	case WM_DESTROY:
		MessageBox(hwnd, TEXT("Bye !, Saad you are going !"), TEXT("Message:"), MB_OKCANCEL | MB_ICONWARNING);
		PostQuitMessage(0);
		break;

	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

BOOL CALLBACK AboutDialogProc(HWND hdlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	switch (iMsg) {

	case WM_INITDIALOG:
		return true;

	case WM_COMMAND:

		switch (LOWORD(wParam)) {

		case IDOK:
		case IDCANCEL:
			EndDialog(hdlg, false);
			return true;
		}

		break;
	}

	return false;
}

