// Peakmeter.cpp -- WinMain and dialog box functions

#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>


static void DrawPeakMeter(HWND, float);
LRESULT CALLBACK MyWndProcedure(HWND , UINT , WPARAM , LPARAM );

static IAudioMeterInformation* pMeterInfo = NULL;

// Timer ID and period (in milliseconds)
#define ID_TIMER  1
#define TIMER_PERIOD  125

#define EXIT_ON_ERROR(hr)  \
              if (FAILED(hr)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

//-----------------------------------------------------------
// DrawPeakMeter -- Draws the peak meter in the dialog box.
//-----------------------------------------------------------

void DrawPeakMeter(HWND hPeakMeter, float peak)
{
    HDC hdc;
    RECT rect;

    GetClientRect(hPeakMeter, &rect);
    hdc = GetDC(hPeakMeter);
    rect.left = rect.right / 2 - 40;
    rect.right = rect.right / 2 + 40;
    rect.top = rect.bottom / 2 - 100;
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 128, 0));
    SelectObject(hdc, hBrush);
    FillRect(hdc, &rect, hBrush);
    rect.left++;
    rect.top++;
    rect.top = rect.bottom -
        max(0, (LONG)(peak * (rect.bottom - rect.top) - 1.5 ));
    rect.bottom--;

    hBrush = CreateSolidBrush(RGB(128, 128, 255));
    SelectObject(hdc, hBrush);
    FillRect(hdc, &rect, hBrush);
    ReleaseDC(hPeakMeter, hdc);
}


// WinMain() -> __WinMainCRTStartup() -> main()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	WNDCLASSEX wndClass;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyWindowApp");
	HWND hwnd;

    HRESULT hr;
    IMMDeviceEnumerator* pEnumerator = NULL;
    IMMDevice* pDevice = NULL;


    CoInitialize(NULL);

    // Get enumerator for audio endpoint devices.
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),
        NULL, CLSCTX_INPROC_SERVER,
        __uuidof(IMMDeviceEnumerator),
        (void**)&pEnumerator);
    EXIT_ON_ERROR(hr)

        // Get peak meter for default audio-rendering device.
        hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    EXIT_ON_ERROR(hr)

        hr = pDevice->Activate(__uuidof(IAudioMeterInformation),
            CLSCTX_ALL, NULL, (void**)&pMeterInfo);
    EXIT_ON_ERROR(hr)

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



	hwnd = CreateWindow(szAppName,
		TEXT("Volume Meter"),
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


Exit:
    if (FAILED(hr))
    {
        MessageBox(NULL, TEXT("This program requires Windows Vista."),
            TEXT("Error termination"), MB_OK);
    }
    SAFE_RELEASE(pEnumerator)
        SAFE_RELEASE(pDevice)
        SAFE_RELEASE(pMeterInfo)
        CoUninitialize();
	return ((int)msg.wParam);

}

LRESULT CALLBACK MyWndProcedure(HWND hwnd, UINT iMsg, WPARAM wparam, LPARAM lparam)
{


    static HWND hPeakMeter = NULL;
    static float peak = 0;
    HRESULT hr;

	switch (iMsg)
	{
	case WM_CREATE:

        
        SetTimer(hwnd, ID_TIMER, TIMER_PERIOD, NULL);
		break;

    case WM_TIMER:
        switch ((int)wparam)
        {
        case ID_TIMER:
            // Update the peak meter in the dialog box.
            hr = pMeterInfo->GetPeakValue(&peak);
            if (FAILED(hr))
            {
                MessageBox(hwnd, TEXT("The program will exit."),
                    TEXT("Fatal error"), MB_OK);
                KillTimer(hwnd, ID_TIMER);
                return TRUE;
            }
            DrawPeakMeter(hwnd, peak);
            return TRUE;
        }
        break;

    case WM_PAINT:
        // Redraw the peak meter in the dialog box.
        ValidateRect(hPeakMeter, NULL);
        DrawPeakMeter(hPeakMeter, peak);
        break;
    
	
	case WM_DESTROY:
        PostQuitMessage(0);
        break;
	}
	return(DefWindowProc(hwnd, iMsg, wparam, lparam));
}



