#include "plugin.h"

#include <memory>

std::unique_ptr<CPlugin> plugin;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            DisableThreadLibraryCalls(hModule);
        
            plugin = std::make_unique<CPlugin>(hModule);

            break;
        }

        case DLL_PROCESS_DETACH:
        {
            plugin.reset();

            break;
        }
    }

    return TRUE;
}