#include <RakHook/rakhook.hpp>
#include <RakHook/rakclient_interface.hpp>

c_rakclient_interface* rakclient_hooked = nullptr;


void destory_ri_hooked(const decltype(rakhook::destroy_ri_hook)& hook, void* rakclient_interface) {
    if (rakclient_interface == rakclient_hooked) {
        rakclient_interface = rakhook::rakclient;
        delete rakclient_hooked;
    }
    return hook.call_trampoline(rakclient_interface);
}


bool handle_rpc_hooked(const decltype(rakhook::handle_rpc_hook)& hook, void* rakpeer, const char* data, int length, PlayerID playerid) {
    rakhook::rakpeer = rakpeer;
    rakhook::playerid = playerid;

    RakNet::BitStream bs { std::bit_cast<unsigned char*>(const_cast<char*>(data)), static_cast<unsigned int>(length), true };

    unsigned char id = 0;
    unsigned char* input = nullptr;
    unsigned int bits_data = 0;

    std::shared_ptr<RakNet::BitStream> callback { std::make_shared<RakNet::BitStream>() };

    bs.IgnoreBits(8);

    if (data[0] == ID_TIMESTAMP)
        bs.IgnoreBits(8 * (sizeof(RakNetTime) + sizeof(unsigned char)));

    auto offset = bs.GetReadOffset();

    bs.Read(id);

    if (!bs.ReadCompressed(bits_data))
        return false;

    if (bits_data) {
        bool used_alloca = false;

        if (BITS_TO_BYTES(bs.GetNumberOfUnreadBits()) < MAX_ALLOCA_STACK_ALLOCATION) {
            input = std::bit_cast<unsigned char*>(alloca(BITS_TO_BYTES(bs.GetNumberOfUnreadBits())));
            used_alloca = true;
        }
        else
            input = new unsigned char[BITS_TO_BYTES(bs.GetNumberOfUnreadBits())];

        if (!bs.ReadBits(input, bits_data, false)) {
            if (!used_alloca)
                delete[] input;

            return false;
        }

        callback = std::make_shared<RakNet::BitStream>(input, BITS_TO_BYTES(bits_data), true);

        if (!used_alloca)
            delete[] input;
    }

    for (auto it = rakhook::on_receive_rpc.begin(); it != rakhook::on_receive_rpc.end();) {
        if (auto f = *it) {
            if (!f(id, callback.get()))
                return false;

            it++;
        }
        else {
            it = rakhook::on_receive_rpc.erase(it);
        }
    }

    bs.SetWriteOffset(offset);
    bs.Write(id);

    bits_data = BYTES_TO_BITS(callback->GetNumberOfBytesUsed());

    bs.WriteCompressed(bits_data);

    if (bits_data)
        bs.WriteBits(callback->GetData(), bits_data, false);
    
    return hook.call_trampoline(rakpeer, std::bit_cast<char*>(bs.GetData()), bs.GetNumberOfBytesUsed(), playerid);
}


bool rakhook::initialize() {
    if (initialized)
        return true;

    if (!samp_addr())
        return false;

    const uintptr_t samp_info = *std::bit_cast<uintptr_t*>(offsets::samp_info(true));

    if (!samp_info)
        return false;

    auto** rakclient_interface = std::bit_cast<RakClientInterface**>(samp_info + offsets::rakclient_interface());

    if (!*rakclient_interface)
        return false;

    rakclient = *rakclient_interface;
    rakclient_hooked = new c_rakclient_interface(rakclient);

    *rakclient_interface = std::bit_cast<RakClientInterface*>(rakclient_hooked);

    destroy_ri_hook.set_cb(&destory_ri_hooked);
    destroy_ri_hook.set_dest(offsets::destroy_interface(true));
    destroy_ri_hook.install();

    handle_rpc_hook.set_cb(&handle_rpc_hooked);
    handle_rpc_hook.set_dest(offsets::handle_rpc_packet(true));
    handle_rpc_hook.install();
    
    return (initialized = true);
}


bool rakhook::send(RakNet::BitStream* bs, PacketPriority priority, PacketReliability reliability, char ord_channel) {
    if (!initialized)
        return false;

    return rakclient->Send(bs, priority, reliability, ord_channel);
}


bool rakhook::send_rpc(int id, RakNet::BitStream* bs, PacketPriority priority, PacketReliability reliability, char ord_channel, bool sh_timestamp) {
    if (!initialized)
        return false;

    return rakclient->RPC(&id, bs, priority, reliability, ord_channel, sh_timestamp);
}


bool rakhook::emul_packet(RakNet::BitStream& pbs) {
    if (!initialized || !rakpeer)
        return false;

    Packet* send_packet = std::bit_cast<Packet * (*)(size_t)>(samp_addr(offsets::alloc_packet()))(pbs.GetNumberOfBytesUsed());
    memcpy(send_packet->data, pbs.GetData(), send_packet->length);

    char* packets = static_cast<char*>(rakpeer) + offsets::offset_packets();
    auto  write_lock = std::bit_cast<Packet * *(__thiscall*)(void*)>(samp_addr(offsets::write_lock()));
    auto  write_unlock = std::bit_cast<void(__thiscall*)(void*)>(samp_addr(offsets::write_unlock()));

    *write_lock(packets) = send_packet;
    write_unlock(packets);

    return true;
}


bool rakhook::emul_rpc(unsigned char id, RakNet::BitStream& rpc_bs) {
    if (!initialized || !rakpeer)
        return false;

    RakNet::BitStream bs;
    
    bs.Write<unsigned char>(ID_RPC);
    bs.Write(id);
    bs.WriteCompressed<unsigned int>(BYTES_TO_BITS(rpc_bs.GetNumberOfBytesUsed()));
    bs.WriteBits(rpc_bs.GetData(), BYTES_TO_BITS(rpc_bs.GetNumberOfBytesUsed()), false);

    return handle_rpc_hook.call_trampoline(rakpeer, std::bit_cast<char*>(bs.GetData()), bs.GetNumberOfBytesUsed(), rakhook::playerid);
}


void rakhook::destroy() {
    if (!initialized)
        return;

    const uintptr_t samp_info = *std::bit_cast<uintptr_t*>(samp_addr(offsets::samp_info()));

    if (!samp_info)
        return;

    auto** rakclient_interface = std::bit_cast<RakClientInterface**>(samp_info + offsets::rakclient_interface());
    *rakclient_interface = rakclient;
    
    destroy_ri_hook.remove();
    handle_rpc_hook.remove();
    
    delete rakclient_hooked;
}