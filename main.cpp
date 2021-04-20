#include <windows.h>
#include <stdio.h>
#include <atomic>
#include <map>
#include <immintrin.h>

DWORD WINAPI     EventListener(LPVOID);
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);

HHOOK   DefaultHook;
HANDLE  ActionThread;

MSG  ThreadMsg;
bool switchClick;

DWORD WINAPI EventListener(LPVOID)
{
    INPUT Input[2];

    Input[0].type          = INPUT_MOUSE;
    Input[0].mi.dwFlags    = MOUSEEVENTF_LEFTDOWN;

    Input[1].type          = INPUT_MOUSE;
    Input[1].mi.dwFlags    = MOUSEEVENTF_LEFTUP;

    while (true)
    {
        if (switchClick)
        {
            Sleep(40);
            SendInput(ARRAYSIZE(Input), Input, sizeof(INPUT));
        }
    }

    return FALSE;
}

LRESULT CALLBACK LowLevelMouseProc(
    int nCode,
    WPARAM wParam,
    LPARAM lParam
)
{
    if (nCode < 0)
        return CallNextHookEx(DefaultHook, nCode, wParam, lParam);

    LPMSLLHOOKSTRUCT MouseHookStruct = reinterpret_cast<LPMSLLHOOKSTRUCT>(lParam);

    /*
        131072  is X1 mouse button
        65536   is X2 mouse button
    */

    if (MouseHookStruct->mouseData == 131072)
        switchClick = !switchClick;

    return CallNextHookEx(DefaultHook, nCode, wParam, lParam);
}

int main(int argc, char **argv) {
    DefaultHook     = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, NULL);
    ActionThread    = CreateThread(NULL, NULL, EventListener, NULL, NULL, NULL);

    if(!DefaultHook)
        return 0;

    while (GetMessage(&ThreadMsg, NULL, NULL, NULL)) {}

    return 0;
}