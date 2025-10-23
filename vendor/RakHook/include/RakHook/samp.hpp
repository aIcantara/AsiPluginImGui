#ifndef RAKHOOK_SAMP_HPP
#define RAKHOOK_SAMP_HPP


namespace rakhook {
#ifdef UNICODE
    inline constexpr auto module_name = L"samp.dll";
#else
    inline constexpr auto module_name = "samp.dll";
#endif

    enum class samp_ver {
        unknown = -1,

        v037r1 = 0,
        v037r2,
        v037r31,
        v037r4,
        v037r5,
        v03dlr1
    };


    inline std::uintptr_t samp_addr(std::uintptr_t offset = 0) {
        static auto samp_module = reinterpret_cast<std::uintptr_t>(GetModuleHandle(module_name));
        return samp_module + offset;
    }


    inline samp_ver samp_version() {
        static bool initialized = false;
        static samp_ver version = samp_ver::unknown;
        
        if (!initialized) {
            std::uintptr_t base = samp_addr();

            auto* nt = reinterpret_cast<IMAGE_NT_HEADERS*>(base + reinterpret_cast<IMAGE_DOS_HEADER*>(base)->e_lfanew);
            std::uintptr_t ep = nt->OptionalHeader.AddressOfEntryPoint;

            switch (ep) {
                case 0x31DF13: version = samp_ver::v037r1;  break;
                case 0x3195DD: version = samp_ver::v037r2;  break;
                case 0xCC4D0:  version = samp_ver::v037r31; break;
                case 0xCBCB0:  version = samp_ver::v037r4;  break;
                case 0xFDB60:  version = samp_ver::v03dlr1; break;
                case 0xCBC90:  version = samp_ver::v037r5;  break;
                default: break;
            }
            initialized = true;
        }
        return version;
    }
}


#endif // RAKHOOK_SAMP_HPP