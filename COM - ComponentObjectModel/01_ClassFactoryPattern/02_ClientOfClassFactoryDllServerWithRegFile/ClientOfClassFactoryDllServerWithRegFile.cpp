#include<windows.h>
#include "ClassFactoryDllServerWithRegFile.h"

// gloabal functions declarations

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// global variable declarations
ISum* pISum = NULL;
ISubtract* pISubtract = NULL;

//WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	//variable declarations
	WNDCLASSEX wndClass;
	MSG msg;
	HWND hwnd;
	TCHAR str[] = TEXT("szAppName");
	HRESULT hr;

	//code
	// COM helper function for initialisation
	hr = CoInitialize(NULL);
	if (FAILED(hr)) {
		MessageBox(NULL, TEXT("COM Library Can Not Br Initialized\n Program will terminate"), TEXT("Runtime Error"), MB_OKCANCEL);
		exit(0);
	}

	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = str;
	wndClass.lpszMenuName = NULL;
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// register window class
	RegisterClassEx(&wndClass);

	// create window
	hwnd = CreateWindow(str, TEXT("My First COM Client -CSumSubtract"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	//message Loop - "DIL KI DHADKAN ..."
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// COM -helper function for Uninitialization
	CoUninitialize();
	return((int)msg.wParam);
}

//Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	// functio declarations
	void SafeInterfaceRelease(void);

	// variable declarations
	HRESULT hr;
	int iNumOne, iNumTwo, iSum;
	TCHAR str[255];

	//code
	switch (iMsg) {
	case WM_CREATE:
		hr = CoCreateInstance(CLSID_SumSubtract, NULL, CLSCTX_INPROC_SERVER, IID_ISubtract, (void**)&pISubtract);
		if (FAILED(hr)) {
			MessageBox(hwnd, TEXT("ISum Interface can not be obtained"), TEXT("Error"), MB_OKCANCEL);
			DestroyWindow(hwnd);
		}

		// initialize arguments hardcoded
		iNumOne = 55;
		iNumTwo = 45;

		// call SUmOfTwoIntegers() of ISum to get sum
		pISubtract->SubtractionOfTwoIntegers(iNumOne, iNumTwo, &iSum);
		//display the result ...

		wsprintf(str, TEXT("Subtraction of %d and %d is %d"), iNumOne, iNumTwo, iSum);
		MessageBox(hwnd, str, TEXT("Result"), MB_OK);
		
		// call QueryInterface() on ISum, to get ISubtract's pointer
		hr = pISubtract->QueryInterface(IID_ISum, (void**)&pISum);
		if (FAILED(hr)) {
			MessageBox(hwnd, TEXT("ISubtract Interface Can Not Be Obtained"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}
		// as ISum is now not needed onwords, release it
		pISubtract->Release();
		pISubtract = NULL;				// make released interface NULL

		// again initalise arguments to now get subtraction result
		iNumOne = 155;
		iNumTwo = 145;

		// now call SubtractionOfTwoIntegers() to get the subtraction ...
		pISum->SumOfTwoIntegers(iNumOne, iNumTwo, &iSum);
		// as ISubtract is now not needed onwords, release it
		pISum->Release();
		pISum = NULL;
		// diplay the result
		wsprintf(str, TEXT("Sum of %d and %d is %d"), iNumOne, iNumTwo, iSum);
		MessageBox(hwnd, str, TEXT("Result"), MB_OK);
		DestroyWindow(hwnd);

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void SafeInterfaceRelease(void) {

	//code
	if (pISum) {
		pISum->Release();
		pISum = NULL;
	}

	if (pISubtract) {
		pISubtract->Release();
		pISubtract = NULL;
	}
}