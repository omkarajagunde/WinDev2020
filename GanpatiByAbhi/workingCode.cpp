#include<windows.h>
#include<stdio.h>
#include<time.h>

#define MYTIMER 100
#define TIME USER_TIMER_MINIMUM
#define CONTOURS 36
#define TOTALPTS 29160


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CALLBACK paint(HWND, UINT, UINT_PTR, DWORD);					// timer for painting bappa

HWND hwnd;
RECT gRect, gRectCopy;
HDC hdc;
FILE *fp, *fp2, *fp3;
HPEN hPen, hPen2;
RECT rc;


int** matrix = NULL;

int i, mIndex, iter, num;

int contoursSize[CONTOURS];
int contoursHalfSize[CONTOURS];
int contoursHalfSizeEmpty[CONTOURS];
int masterContourPts[TOTALPTS];


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	WNDCLASSEX wndclass;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyApp");

	wndclass.hInstance = hInstance;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.cbClsExtra = 0;
	wndclass.lpszClassName = szAppName;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szAppName, TEXT("Ganpati Bappa"), WS_OVERLAPPEDWINDOW, 0, 0, 1200, 1200, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, SW_MAXIMIZE);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return((int)msg.wParam);
}


void initializeArrays() {
	static int i = 0;

//####################################################################################################################

// initializing 2D array

	matrix = (int**)malloc(CONTOURS * sizeof(int*));
	
	if (matrix == NULL) {
		MessageBox(hwnd, TEXT("matrix allocation failed"), TEXT("Message"), MB_OK);
	}

//####################################################################################################################

// taking contours sizes from file

	fp = fopen("size.txt", "r");
	
	if (fp == NULL) {
		MessageBox(hwnd, TEXT("size.txt file opening failed"), TEXT("Message"), MB_OK);
	}

	while (fscanf(fp, "%d", &num) != EOF) {
		contoursSize[i] = num;
		matrix[i] = (int*)malloc(num * sizeof(int));
		contoursHalfSize[i++] = num / 2;
	}

//####################################################################################################################

// taking all points from file

	fp2 = fopen("masterfile.txt", "r");	
	
	if (fp2 == NULL) {
		MessageBox(hwnd, TEXT("masterfile.txt file opening failed"), TEXT("Message"), MB_OK);
	}

	while (fscanf(fp2, "%d", &num) != EOF) {
		masterContourPts[mIndex++] = num;
	}

	//MessageBox(hwnd, TEXT("masterContour Array filling DONE"), TEXT("Message"), MB_OK);

//####################################################################################################################

// checking is all points are filled in array correctly ???

	fp3 = fopen("masterfileIsOk.txt", "w");

	if (fp3 == NULL) {
		MessageBox(hwnd, TEXT("masterfileIsOk.txt file opening failed"), TEXT("Message"), MB_OK);
	}

	mIndex = 0;

	for (int i = 0; i < CONTOURS; i++) {
		for (int j = 0; j < contoursSize[i]; j++) {
			matrix[i][j] = masterContourPts[mIndex];
			mIndex++;
		}
	}

	//MessageBox(hwnd, TEXT("matrix filling DONE"), TEXT("Message"), MB_OK);

	for (int i = 0; i < CONTOURS; i++) {
		for (int j = 0; j < contoursSize[i]; j++) {
			 fprintf(fp3, "%d  ", matrix[i][j]);
		}
	}
	//MessageBox(hwnd, TEXT("masterfileIsOk filling DONE"), TEXT("Message"), MB_OK);

//####################################################################################################################

	fclose(fp);
	fclose(fp2);
	fclose(fp3);

}


void CALLBACK paint(HWND hwnd, UINT iMsg, UINT_PTR timerEvent, DWORD ms) {
	iter += 2;
	KillTimer(hwnd, MYTIMER);
	hdc = GetDC(hwnd);
	SelectObject(hdc, hPen);

	static int x = 2;

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


	if (contoursHalfSizeEmpty[0] <= contoursHalfSize[0]) {					// C0
		static int once = 1;
		if (once) {
			once = 0;
			i = 0;
		}
		//SetPixel(hdc, matrix[0][i] - x, matrix[0][i + 1] - x, matrix[0][i] + x, matrix[0][i + 1] + x);
		Ellipse(hdc, matrix[0][i] - x, matrix[0][i + 1] - x, matrix[0][i] + x, matrix[0][i + 1] + x);
		contoursHalfSizeEmpty[0]++;
		i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[1] <= contoursHalfSize[1]) {				// C1
		static int once = 1;
		if (once) {
			once = 0;
			i = 0;
		}
		Ellipse(hdc, matrix[1][i] - x, matrix[1][i + 1] - x, matrix[1][i] + x, matrix[1][i + 1] + x);
		contoursHalfSizeEmpty[1]++;
		i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[2] <= contoursHalfSize[2]) {				// C2
		static int once = 1;
		if (once) {
			once = 0;
			i = 0;
		}
		Ellipse(hdc, matrix[2][i] - x, matrix[2][i + 1] - x, matrix[2][i] + x, matrix[2][i + 1] + x);
		contoursHalfSizeEmpty[2]++;
		i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[3] <= contoursHalfSize[3]) {				// C3 && C4
		static int once = 1;
		if (once) {
			once = 0;
			i = 0;
		}
		Ellipse(hdc, matrix[3][i] - x, matrix[3][i + 1] - x, matrix[3][i] + x, matrix[3][i + 1] + x);
		
		if (contoursHalfSizeEmpty[4] <= contoursHalfSize[4]) {
			Ellipse(hdc, matrix[4][i] - x, matrix[4][i + 1] - x, matrix[4][i] + x, matrix[4][i + 1] + x);
			contoursHalfSizeEmpty[4]++;
		}
		contoursHalfSizeEmpty[3]++;
		i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[5] <= contoursHalfSize[5]) {				// C5 && C6
		static int once = 1;
		if (once) {
			once = 0;
			i = 0;
		}
		Ellipse(hdc, matrix[5][i] - x, matrix[5][i + 1] - x, matrix[5][i] + x, matrix[5][i + 1] + x);
		Ellipse(hdc, matrix[6][i] - x, matrix[6][i + 1] - x, matrix[6][i] + x, matrix[6][i + 1] + x);
		contoursHalfSizeEmpty[5]++;
		contoursHalfSizeEmpty[6]++;
		i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[7] <= contoursHalfSize[7]) {				// C7 && C8
		static int once = 1;
		if (once) {
			once = 0;
			i = 0;
		}
		Ellipse(hdc, matrix[7][i] - x, matrix[7][i + 1] - x, matrix[7][i] + x, matrix[7][i + 1] + x);
		Ellipse(hdc, matrix[8][i] - x, matrix[8][i + 1] - x, matrix[8][i] + x, matrix[8][i + 1] + x);
		contoursHalfSizeEmpty[7]++;
		contoursHalfSizeEmpty[8]++;
		i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[9] <= contoursHalfSize[9]) {				// C9 && C10
		static int once = 1;
		if (once) {
			once = 0;
			i = 0;
		}
		Ellipse(hdc, matrix[9][i] - x, matrix[9][i + 1] - x, matrix[9][i] + x, matrix[9][i + 1] + x);
		Ellipse(hdc, matrix[10][i] - x, matrix[10][i + 1] - x, matrix[10][i] + x, matrix[10][i + 1] + x);
		contoursHalfSizeEmpty[9]++;
		contoursHalfSizeEmpty[10]++;
		i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[11] <= contoursHalfSize[11]) {			// C11 && C12
		static int once = 1;
		if (once) {
			once = 0;
			i = 0;
		}
		Ellipse(hdc, matrix[11][i] - x, matrix[11][i + 1] - x, matrix[11][i] + x, matrix[11][i + 1] + x);
		Ellipse(hdc, matrix[12][i] - x, matrix[12][i + 1] - x, matrix[12][i] + x, matrix[12][i + 1] + x);
		contoursHalfSizeEmpty[11]++;
		contoursHalfSizeEmpty[12]++;
		i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[14] <= contoursHalfSize[14]) {			// C13 && C14
		static int once = 1;
		if (once) {
			once = 0;
			i = 0;
		}
		Ellipse(hdc, matrix[14][i] - x, matrix[14][i + 1] - x, matrix[14][i] + x, matrix[14][i + 1] + x);
		if (contoursHalfSizeEmpty[13] <= contoursHalfSize[13]) {
			Ellipse(hdc, matrix[13][i] - x, matrix[13][i + 1] - x, matrix[13][i] + x, matrix[13][i + 1] + x);
			contoursHalfSizeEmpty[13]++;
		}
		contoursHalfSizeEmpty[14]++;
		i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[15] <= contoursHalfSize[15]) {			// C15
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;
	}
	Ellipse(hdc, matrix[15][i] - x, matrix[15][i + 1] - x, matrix[15][i] + x, matrix[15][i + 1] + x);
	contoursHalfSizeEmpty[15]++;
	i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[16] <= contoursHalfSize[16]) {			// C16
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;
	}
	Ellipse(hdc, matrix[16][i] - x, matrix[16][i + 1] - x, matrix[16][i] + x, matrix[16][i + 1] + x);
	contoursHalfSizeEmpty[16]++;
	i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[17] <= contoursHalfSize[17]) {			// C17
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;
	}
	Ellipse(hdc, matrix[17][i] - x, matrix[17][i + 1] - x, matrix[17][i] + x, matrix[17][i + 1] + x);
	contoursHalfSizeEmpty[17]++;
	i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[18] <= contoursHalfSize[18]) {			// C18
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;
	}
	Ellipse(hdc, matrix[18][i] - x, matrix[18][i + 1] - x, matrix[18][i] + x, matrix[18][i + 1] + x);
	contoursHalfSizeEmpty[18]++;
	i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[19] <= contoursHalfSize[19]) {			// C19
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;
	}
	Ellipse(hdc, matrix[19][i] - x, matrix[19][i + 1] - x, matrix[19][i] + x, matrix[19][i + 1] + x);
	contoursHalfSizeEmpty[19]++;
	i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[20] <= contoursHalfSize[20]) {			// C20
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;
	}
	Ellipse(hdc, matrix[20][i] - x, matrix[20][i + 1] - x, matrix[20][i] + x, matrix[20][i + 1] + x);
	contoursHalfSizeEmpty[20]++;
	i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[22] <= contoursHalfSize[22]) {			// C21 && C22
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;
	}
	Ellipse(hdc, matrix[22][i] - x, matrix[22][i + 1] - x, matrix[22][i] + x, matrix[22][i + 1] + x);
	if (contoursHalfSizeEmpty[21] <= contoursHalfSize[21]) {
		Ellipse(hdc, matrix[21][i] - x, matrix[21][i + 1] - x, matrix[21][i] + x, matrix[21][i + 1] + x);
		contoursHalfSizeEmpty[21]++;
	}
	contoursHalfSizeEmpty[22]++;
	i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[24] <= contoursHalfSize[24]) {			// C23 && C24
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;
	}
	Ellipse(hdc, matrix[24][i] - x, matrix[24][i + 1] - x, matrix[24][i] + x, matrix[24][i + 1] + x);
	if (contoursHalfSizeEmpty[23] <= contoursHalfSize[23]) {
		Ellipse(hdc, matrix[23][i] - x, matrix[23][i + 1] - x, matrix[23][i] + x, matrix[23][i + 1] + x);
		contoursHalfSizeEmpty[23]++;
	}
	contoursHalfSizeEmpty[24]++;
	i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[26] <= contoursHalfSize[26]) {			// C25 && C26
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;
	}
	Ellipse(hdc, matrix[26][i] - x, matrix[26][i + 1] - x, matrix[26][i] + x, matrix[26][i + 1] + x);
	if (contoursHalfSizeEmpty[25] <= contoursHalfSize[25]) {
		Ellipse(hdc, matrix[25][i] - x, matrix[25][i + 1] - x, matrix[25][i] + x, matrix[25][i + 1] + x);
		contoursHalfSizeEmpty[25]++;
	}
	contoursHalfSizeEmpty[26]++;
	i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[27] <= contoursHalfSize[27]) {			// C27 && C28
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;
	}
	Ellipse(hdc, matrix[27][i] - x, matrix[27][i + 1] - x, matrix[27][i] + x, matrix[27][i + 1] + x);
	if (contoursHalfSizeEmpty[28] <= contoursHalfSize[28]) {
		Ellipse(hdc, matrix[28][i] - x, matrix[28][i + 1] - x, matrix[28][i] + x, matrix[28][i + 1] + x);
		contoursHalfSizeEmpty[28]++;
	}
	contoursHalfSizeEmpty[27]++;
	i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[30] <= contoursHalfSize[30]) {			// C29 && C30
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;
	}
	Ellipse(hdc, matrix[30][i] - x, matrix[30][i + 1] - x, matrix[30][i] + x, matrix[30][i + 1] + x);
	if (contoursHalfSizeEmpty[29] <= contoursHalfSize[29]) {
		Ellipse(hdc, matrix[29][i] - x, matrix[29][i + 1] - x, matrix[29][i] + x, matrix[29][i + 1] + x);

		contoursHalfSizeEmpty[29]++;
	}
	contoursHalfSizeEmpty[30]++;
	i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[31] <= contoursHalfSize[31]) {			// C31
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;
	}
	Ellipse(hdc, matrix[31][i] - x, matrix[31][i + 1] - x, matrix[31][i] + x, matrix[31][i + 1] + x);
	contoursHalfSizeEmpty[31]++;
	i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[32] <= contoursHalfSize[32]) {			// C32
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;
	}
	Ellipse(hdc, matrix[32][i] - x, matrix[32][i + 1] - x, matrix[32][i] + x, matrix[32][i + 1] + x);
	contoursHalfSizeEmpty[32]++;
	i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[33] <= contoursHalfSize[33]) {			// C33
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;
	}
	Ellipse(hdc, matrix[33][i] - x, matrix[33][i + 1] - x, matrix[33][i] + x, matrix[33][i + 1] + x);
	contoursHalfSizeEmpty[33]++;
	i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[34] <= contoursHalfSize[34]) {			// C34
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;
	}
	Ellipse(hdc, matrix[34][i] - x, matrix[34][i + 1] - x, matrix[34][i] + x, matrix[34][i + 1] + x);
	contoursHalfSizeEmpty[34]++;
	i += 2;
	}

//----------------------------------------------------------------------------------------------------------------------------------

	else if (contoursHalfSizeEmpty[35] <= contoursHalfSize[35]) {			// C35
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;
	}
	Ellipse(hdc, matrix[35][i] - x, matrix[35][i + 1] - x, matrix[35][i] + x, matrix[35][i + 1] + x);
	contoursHalfSizeEmpty[35]++;
	i += 2;
	}
	
	
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$



	if (iter < TOTALPTS) {
		SetTimer(hwnd, MYTIMER, TIME, paint);
	}
	else {
		KillTimer(hwnd, MYTIMER);
	}

	ReleaseDC(hwnd, hdc);

}





LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	
	PAINTSTRUCT ps;

	switch (iMsg) {
	case WM_CREATE :
		//f = fopen("C:\\Users\\Abhi\\source\\repos\\Win32_Projects\\GanpatiBappa\\points\\32_c14.txt", "r");
		hPen = CreatePen(PS_SOLID, 2, RGB(95, 255, 0));
		hPen2 = CreatePen(PS_SOLID, 5, RGB(0, 0, 255));
		
		initializeArrays();
		SetTimer(hwnd, MYTIMER, TIME, paint);

		break;

	case WM_SIZE :
		GetClientRect(hwnd, &gRect);
		break;

	case WM_RBUTTONDOWN :
		//SetTimer(hwnd, MYTIMER, TIME, paint);
		break;

	case WM_PAINT :
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY :
		//fclose(f);
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}




