#include<windows.h>
#include "MyHeader.h"



LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

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

	HBITMAP horseBitmap, lionBitmap, dogBitmap;
	HDC hdc;
	HDC hdcMem;
	PAINTSTRUCT ps;
	RECT rc;
	BITMAP bHorseBitmap, bLionBitmap, bDogBitmap;
	static HINSTANCE hInstance;

	switch (iMsg) {

	case WM_CREATE:
		hInstance = GetModuleHandle(NULL);
		break;

	case WM_PAINT:
		GetClientRect(hwnd, &rc);
		hdc = BeginPaint(hwnd, &ps);

		hdcMem = CreateCompatibleDC(NULL);

		horseBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(HORSEBITMAP));
		SelectObject(hdcMem, horseBitmap);
		GetObject(horseBitmap, sizeof(BITMAP), &bHorseBitmap);
		StretchBlt(hdc,0,0,rc.bottom/4, rc.right/2, hdcMem, 0, 0, bHorseBitmap.bmWidth, bHorseBitmap.bmHeight, SRCCOPY);

		Sleep(2000);

		lionBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(LIONBITMAP));
		SelectObject(hdcMem, lionBitmap);
		GetObject(lionBitmap, sizeof(BITMAP), &bLionBitmap);
		StretchBlt(hdc, rc.bottom / 4, 0, rc.bottom / 2, rc.right / 2, hdcMem, 0, 0, bLionBitmap.bmWidth, bLionBitmap.bmHeight, SRCCOPY);

		Sleep(2000);

		dogBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(DOGBITMAP));
		SelectObject(hdcMem, dogBitmap);
		GetObject(dogBitmap, sizeof(BITMAP), &bDogBitmap);
		StretchBlt(hdc, rc.bottom / 2, 0, rc.bottom * 0.75, rc.right / 2, hdcMem, 0, 0, bDogBitmap.bmWidth, bDogBitmap.bmHeight, SRCCOPY);

		
		DeleteObject(horseBitmap);
		DeleteObject(lionBitmap);
		DeleteObject(dogBitmap);

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

