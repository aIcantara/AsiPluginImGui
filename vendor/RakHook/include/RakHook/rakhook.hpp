#ifndef RAKHOOK_HPP
#define RAKHOOK_HPP

#ifndef MAX_ALLOCA_STACK_ALLOCATION
#define MAX_ALLOCA_STACK_ALLOCATION 1048576
#endif

#include <windows.h>
#include <functional>

#include <memory>
#include <bit>

#include <kthook/kthook.hpp>

#include <RakNet/RakClientInterface.h>
#include <RakNet/PacketEnumerations.h>

#include "samp.hpp"
#include "detail.hpp"
#include "offsets.hpp"

#include "rakmem.hpp"


namespace rakhook {
    template <typename T>
    struct on_event : public std::vector<std::function<T>> {
        on_event &operator+=(std::function<T> func) {
            this->push_back(func);
            return *this;
        }
    };

    using send_t        = bool(RakNet::BitStream *bs, PacketPriority &priority, PacketReliability &reliability, char &ord_channel);
    using receive_t     = bool(Packet *packet);
    using send_rpc_t    = bool(int &id, RakNet::BitStream *bs, PacketPriority &priority, PacketReliability &reliability, char &ord_channel, bool &sh_timestamp);
    using receive_rpc_t = bool(unsigned char &id, RakNet::BitStream *bs);

    inline bool initialized = false;
    inline RakClientInterface* rakclient = nullptr;
    
    inline void* rakpeer = nullptr;
    
    inline PlayerID playerid;

    inline on_event<send_t>        on_send_packet;
    inline on_event<receive_t>     on_receive_packet;
    inline on_event<send_rpc_t>    on_send_rpc;
    inline on_event<receive_rpc_t> on_receive_rpc;

    inline kthook::kthook_simple<void(__cdecl*)(void*)> destroy_ri_hook;
    inline kthook::kthook_simple<bool(__thiscall*)(void*, const char*, int, PlayerID)> handle_rpc_hook;

    bool initialize();
    void destroy();

    bool send(RakNet::BitStream *bs, PacketPriority priority, PacketReliability reliability, char ord_channel);
    bool send_rpc(int id, RakNet::BitStream *bs, PacketPriority priority, PacketReliability reliability, char ord_channel, bool sh_timestamp);

    bool emul_rpc(unsigned char id, RakNet::BitStream &rpc_bs);
    bool emul_packet(RakNet::BitStream &pbs);
}


#endif // RAKHOOK_HPP