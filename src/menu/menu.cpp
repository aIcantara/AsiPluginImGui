#include "menu.h"

#include <imgui.h>

#include <string>

void CMenu::init()
{
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    ImGui::GetIO().IniFilename = nullptr;

    #pragma warning(push)
    #pragma warning(disable: 4996)
    std::string font = getenv("WINDIR"); font += "\\Fonts\\Arialbd.TTF";
    #pragma warning(pop)

    ImGui::GetIO().Fonts->AddFontFromFileTTF(font.c_str(), 15.f, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
}

void CMenu::draw()
{
    ImGui::SetNextWindowPos(ImVec2(100.f, 100.f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300.f, 300.f), ImGuiCond_FirstUseEver);

    ImGui::Begin("AsiPluginImGui", nullptr);
    {
        ImGui::Text("Text");
    }
    ImGui::End();
}