#pragma once

/*
	windows.h required
	returns -1 if unable to open file
 	returns -2 if file is empty
	returns 1 if successful
*/
extern "C" int GetPoints(const char*, POINT***);

/* Frees all the created arrays ...
   This function if used in Win32 then calling
   this function in WM_DESTROY will be appropriate,
   you can also use this function as per you requirement 
   to free the arrays

*/
extern "C" int FreePointsArray(POINT * **ptsArr);