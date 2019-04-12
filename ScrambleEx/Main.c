
#include <windows.h>

#pragma comment(lib ,"msimg32.lib")

#define NUM 300

COLORREF Color[] = { RGB(255,0,0),RGB(0,128,0), RGB(0,0,255),RGB(0, 255, 255),RGB(255, 0, 255) ,RGB(255, 255, 0) ,RGB(0, 0, 128) };

#define CLNUM (sizeof(Color)/sizeof(Color[0]))

HBRUSH ColorBrush[CLNUM][8];//0是实心，1-6对应0-5

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	HDC        hdcScr, hdcSwap, hdcMem;

	int        cx, cy;
	HBITMAP    hBitmapSwap, hBitmap;
	HWND       hwnd;
	int        j, x1, y1, x2, y2;

	if (LockWindowUpdate(hwnd = GetDesktopWindow()))
	{
		hdcScr = GetDCEx(hwnd, NULL, DCX_CACHE | DCX_LOCKWINDOWUPDATE);
		hdcSwap = CreateCompatibleDC(hdcScr);
		cx = GetSystemMetrics(SM_CXSCREEN);// 10;
		cy = GetSystemMetrics(SM_CYSCREEN);// 10;
		hBitmap = CreateCompatibleBitmap(hdcScr, cx, cy);
		hdcMem = CreateCompatibleDC(hdcScr);
		
		SelectObject(hdcMem, hBitmap);
		BitBlt(hdcMem, 0, 0, cx, cy, hdcScr, 0, 0, SRCCOPY);
		cx /= 10;
		cy /= 10;
		for (int i = 0; i < CLNUM; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				ColorBrush[i][j] = j ? CreateHatchBrush(j-1,Color[i]) : CreateSolidBrush(Color[i]);
			}
		}
		hBitmapSwap = CreateCompatibleBitmap(hdcScr, cx, cy);
		SelectObject(hdcSwap, hBitmapSwap);

		SetBkMode(hdcSwap, TRANSPARENT);
		srand((int)GetCurrentTime());

		for (j = 0; j < NUM; j++)
		{
			x1 = cx * (rand() % 10);
			y1 = cy * (rand() % 10);
			x2 = cx * (rand() % 10);
			y2 = cy * (rand() % 10);

			//BitBlt(hdcMem, 0, 0, cx, cy, hdcScr, x1, y1, SRCCOPY);
			BitBlt(hdcScr, x1, y1, cx, cy, hdcMem, x2, y2, SRCCOPY);

			
			{
				BLENDFUNCTION bf = { 0 };
				bf.SourceConstantAlpha = rand() % 255;
				SelectObject(hdcSwap, GetStockObject(NULL_PEN));
				SelectObject(hdcSwap, (rand() & 3) ? ColorBrush[rand() % CLNUM][0] : ColorBrush[rand() % CLNUM][(rand() % 6) + 1]);
				BitBlt(hdcSwap, 0, 0, cx, cy, hdcMem, x1, y1, SRCCOPY);
				
				Rectangle(hdcSwap, 0, 0, cx, cy);
				AlphaBlend(hdcScr, x1, y1, cx, cy, hdcSwap, 0, 0, cx, cy, bf);

				BitBlt(hdcScr, x2, y2, cx, cy, hdcMem, x1, y1, SRCCOPY);

			}

			Sleep(10);
		}

		DeleteDC(hdcSwap);
		DeleteDC(hdcMem);
		ReleaseDC(hwnd, hdcScr);
		DeleteObject(hBitmapSwap);
		DeleteObject(hBitmap);

		LockWindowUpdate(NULL);
	}
	return FALSE;
}
