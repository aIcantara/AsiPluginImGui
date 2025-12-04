#include "plugin.h"

CPlugin::CPlugin(HMODULE hModule) : hModule(hModule)
{
    using namespace std::placeholders;

    hookWndProc.set_cb(std::bind(&CPlugin::onWndProc, this, _1, _2, _3, _4, _5));
    hookWndProc.install();
}

HRESULT CPlugin::onWndProc(const decltype(hookWndProc)& hook, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

    if (ImGui::GetIO().WantCaptureKeyboard || ImGui::GetIO().WantCaptureMouse)
    {
        return 1;
    }

    if (uMsg == WM_KEYDOWN && wParam == VK_F9 && (HIWORD(lParam) & KF_REPEAT) != KF_REPEAT)
    {
        render.toggleMenu();
    }

    return hook.call_trampoline(hWnd, uMsg, wParam, lParam);
}