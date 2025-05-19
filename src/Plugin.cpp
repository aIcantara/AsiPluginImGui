#include "Plugin.h"
#include <RakHook/rakhook.hpp>
#include <RakNet/StringCompressor.h>
#include <sampapi/CNetGame.h>
#include <sampapi/CChat.h>
#include <sampapi/CInput.h>

namespace samp = sampapi::v037r1;

CPlugin::CPlugin(HMODULE hModule) : hModule(hModule) {
  using namespace std::placeholders;

  hookGameLoop.set_cb(std::bind(&CPlugin::gameLoop, this, _1));
  hookGameLoop.install();

  hookWndProc.set_cb(std::bind(&CPlugin::wndProc, this, _1, _2, _3, _4, _5));
  hookWndProc.install();
}

CPlugin::~CPlugin() {
  rakhook::destroy();
}

void CPlugin::gameLoop(const decltype(hookGameLoop)& hook) {
  static bool initialized = false;
  if (!initialized && samp::RefNetGame() && rakhook::initialize()) {
    StringCompressor::AddReference();

    samp::RefInputBox()->AddCommand("cmd", [](const char* param) -> void {
      samp::RefChat()->AddMessage(-1, "plugin cmd");
    });

    initialized = true;
  }

  return hook.get_trampoline()();
}

HRESULT CPlugin::wndProc(const decltype(hookWndProc)& hook, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (uMsg == WM_KEYDOWN) {
    if (wParam == VK_F9 && (HIWORD(lParam) & KF_REPEAT) != KF_REPEAT)
      render.toggleWindow();
  }

  if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
    return 1;

  return hook.get_trampoline()(hWnd, uMsg, wParam, lParam);
}