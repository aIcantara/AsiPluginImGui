#ifndef RAKHOOK_OFFSETS_HPP
#define RAKHOOK_OFFSETS_HPP


namespace rakhook::offsets {
    inline constexpr std::uintptr_t samp_info_offsets[] = { 0x21a0f8, 0x21A100, 0x26e8dc, 0x26ea0c, 0x26EB94, 0x2aca24 };
    inline constexpr std::uintptr_t rakclient_interface_offsets[] = { 0x3c9, 0x18, 0x2c, 0x2c, 0x0, 0x2c };
    inline constexpr std::uintptr_t destroy_interface_offsets[] = { 0x342d0, 0x343B0, 0x37680, 0x37d70, 0x37DC0, 0x37880 };
    inline constexpr std::uintptr_t handle_rpc_packet_offsets[] = { 0x372f0, 0x373D0, 0x3a6a0, 0x3ad90, 0x3ADE0, 0x3a8a0 };
    inline constexpr std::uintptr_t alloc_packet_offsets[] = { 0x347e0, 0x348C0, 0x37b90, 0x38280, 0x382D0, 0x37d90 };
    inline constexpr std::uintptr_t offset_packets_offsets[] = { 0xdb6, 0xdb6, 0xdb6, 0xdb6, 0xdb6, 0xdb6 };
    inline constexpr std::uintptr_t write_lock_offsets[] = { 0x35b10, 0x35BF0, 0x38ec0, 0x395b0, 0x39600, 0x390c0 };
    inline constexpr std::uintptr_t write_unlock_offsets[] = { 0x35b50, 0x35C30, 0x38f00, 0x395f0, 0x39640, 0x39100 };


    inline std::uintptr_t get_offset(const std::uintptr_t addr[], bool base) {
        const rakhook::samp_ver version = rakhook::samp_version();

        if (version == rakhook::samp_ver::unknown)
            return 0;

        std::uintptr_t result = addr[rakhook::detail::to_underlying(version)];
        
        if (base)
            result += rakhook::samp_addr();

        return result;
    }

    
    inline std::uintptr_t samp_info(bool base = false) { 
        return get_offset(samp_info_offsets, base); 
    }

    inline std::uintptr_t rakclient_interface(bool base = false) { 
        return get_offset(rakclient_interface_offsets, base); 
    }

    inline std::uintptr_t destroy_interface(bool base = false) { 
        return get_offset(destroy_interface_offsets, base); 
    }

    inline std::uintptr_t handle_rpc_packet(bool base = false) { 
        return get_offset(handle_rpc_packet_offsets, base); 
    }

    inline std::uintptr_t alloc_packet(bool base = false) { 
        return get_offset(alloc_packet_offsets, base); 
    }

    inline std::uintptr_t offset_packets(bool base = false) { 
        return get_offset(offset_packets_offsets, base); 
    }

    inline std::uintptr_t write_lock(bool base = false) { 
        return get_offset(write_lock_offsets, base); 
    }

    inline std::uintptr_t write_unlock(bool base = false) { 
        return get_offset(write_unlock_offsets, base); 
    }
}


#endif
