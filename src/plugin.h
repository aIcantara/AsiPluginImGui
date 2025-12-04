#ifndef PLUGIN_H
#define PLUGIN_H

#include "render.h"

#include <kthook/kthook.hpp>

class CPlugin
{
public:
    CPlugin(HMODULE hModule);

    HMODULE hModule;

private:
    CRender render;

    kthook::kthook_simple<HRESULT(__stdcall*)(HWND, UINT, WPARAM, LPARAM)> hookWndProc{ 0x747EB0 };
    HRESULT onWndProc(const decltype(hookWndProc)& hook, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif // PLUGIN_H