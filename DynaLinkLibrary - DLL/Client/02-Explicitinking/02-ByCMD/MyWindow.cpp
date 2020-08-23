#include<windows.h>

// window procedure callback
LRESULT CALLBACK MyWndProcedure(HWND, UINT, WPARAM, LPARAM);

// WinMain() -> __WinMainCRTStartup() -> main()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	WNDCLASSEX wndClass;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyWindowApp");
	HWND hwnd;

	wndClass.cbSize = sizeof(wndClass);
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.lpfnWndProc = MyWndProcedure;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_HAND);
	wndClass.hInstance = hInstance;
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.lpszClassName = szAppName;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wndClass);

	int xCalc = GetSystemMetrics(SM_CXSCREEN);
	int yCalc = GetSystemMetrics(SM_CYSCREEN);
	xCalc -= 800;
	xCalc /= 2;

	yCalc -= 600;
	yCalc /= 2;

	hwnd = CreateWindow(szAppName,
		TEXT("Explicit DLL Linking"),
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
	return ((int)msg.wParam);

}

LRESULT CALLBACK MyWndProcedure(HWND hwnd, UINT iMsg, WPARAM wparam, LPARAM lparam)
{

	HMODULE hDll = NULL;
	// Function pointer
	typedef int (*MakeSquareFunctionPointer)(int);
	// Variable of MakeSquareFunctionPointer
	MakeSquareFunctionPointer makeSquareFunctionPointer = NULL;


	int i = 255;
	int j;
	TCHAR str[255];

	switch (iMsg)
	{
	case WM_CREATE:

		hDll = LoadLibrary(TEXT("08_MyMath.dll"));
		// Error Checking ...if library is not loaded ...
		if (hDll == NULL) {
			MessageBox(hwnd, TEXT("LoadLibrary() Failed !!!"), TEXT("Explicit DLL Linking Error ..."), MB_OK);
			DestroyWindow(hwnd);
		}

		// Library Loaded successfully ...
		// Get address of the MakeSquare in DLL ...
		// GetProcAdress returns FARPROC
		makeSquareFunctionPointer = (MakeSquareFunctionPointer)GetProcAddress(hDll, "MakeSquare");

		// Error ...Checking
		if (makeSquareFunctionPointer == NULL) {
			FreeLibrary(hDll);
			MessageBox(hwnd, TEXT("Function address resolution failed ... !!!"), TEXT("Explicit DLL Linking Error"), MB_OK);
			DestroyWindow(hwnd);
		}
		else {

			// Successfully retrieved function address of MakeSquare ...
			j = makeSquareFunctionPointer(i);
			wsprintf(str, TEXT("Square of %d is %d"), i, j);
			MessageBox(hwnd, str, TEXT("Square Via Explicit DLL Linking ..."), MB_OK);


			// Call was successfull so free the library ...
			FreeLibrary(hDll);

		}
		break;
	case WM_PAINT:

		break;

	case WM_DESTROY:
		PostQuitMessage(0);

	default:
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wparam, lparam));
}
