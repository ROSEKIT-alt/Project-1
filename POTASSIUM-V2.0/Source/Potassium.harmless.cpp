#include <windows.h>

static ULONGLONG n, r;
int randy() { 
    return n = r, n ^= 0x8ebf635bee3c6d25, n ^= n << 5 | n >> 26, n *= 0xf3e05ca5c43e376b, r = n, n & 0x7fffffff;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
   if (MessageBoxW(NULL, L"Run Malware?", L"Potassium.exe", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
       ExitProcess(0);
   } else {
       if (MessageBoxW(NULL, L"Are you sure?", L"Potassium.exe - LAST WARNING!", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
           ExitProcess(0);
       } else {
           int time = GetTickCount();
           int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
           RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
           for (int i = 0;; i++, i %= 3) {
                HDC desk = GetDC(NULL);
                HDC hdcdc = CreateCompatibleDC(desk);
                HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
                SelectObject(hdcdc, hbm);
                BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
                GetBitmapBits(hbm, 4 * h * w, data);
                int v = 0;
                BYTE byte = 0;
                if ((GetTickCount() - time) > 10)
                   byte = randy()%0xff;
                for (int i = 0; w * h > i; i++) {
                   if (i % h == 0 && randy() % 100==0)
                      v = randy() % 25;
                   ((BYTE*)(data + i + v))[v % 6] = ((BYTE*)(data + i))[v] ^ byte;
                }
                SetBitmapBits(hbm, w * h * 4, data);
                BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
                DeleteObject(hbm);
                DeleteObject(hdcdc);
                DeleteObject(desk);
           }
           return 0;
       }
   }
}