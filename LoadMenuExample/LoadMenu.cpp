#include<windows.h>
#include "MyHeader.h"


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDialogProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE ghInstance;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmsLine, int iCmdShow) {

	WNDCLASSEX wndClass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("ZoomOutRectangleApp");

	HMENU hMenu;
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


	static int idColor[5] = {
		WHITE_BRUSH,
		LTGRAY_BRUSH,
		GRAY_BRUSH,
		DKGRAY_BRUSH,
		BLACK_BRUSH

	};

	HBRUSH hBrush;

	switch (iMsg) {

	case WM_CREATE:

		break;

	case WM_PAINT:
		break;

	case WM_COMMAND:
		
		switch (LOWORD(wParam)) {
		case IDM_WHITE:
		case IDM_LTGRAY:
		case IDM_GRAY:
		case IDM_BLACK:
			hBrush = (HBRUSH)GetStockObject(idColor[LOWORD(wParam) - IDM_WHITE]);
			SetClassLong(hwnd, GCL_HBRBACKGROUND, (long)hBrush);
			InvalidateRect(hwnd, NULL, true);
			break;

		case IDM_APP_ABOUT:
			DialogBox(ghInstance, TEXT("about"), hwnd, AboutDialogProc);
			break;
		}

		break;

	case WM_DESTROY:
		MessageBox(hwnd, TEXT("Bye !, Saad you are going !"), TEXT("Message:"), MB_OKCANCEL | MB_ICONWARNING);
		PostQuitMessage(0);
		break;

	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

BOOL CALLBACK AboutDialogProc(HWND hDlg, UINT iMsg, WPARAM wparam, LPARAM lparam) {

	switch (iMsg) {
	case WM_INITDIALOG:
		return true;

	case WM_COMMAND:
		switch (LOWORD(wparam)) {

		case IDOK:
		case IDCANCEL:
			EndDialog(hDlg, false);
			return true;
		}
		break;
	}

	return false;

}

