/*
        PROJECT:		mod_sa
        LICENSE:		See LICENSE in the top level directory
        COPYRIGHT:		Copyright we_sux, BlastHack

        mod_sa is available from https://github.com/BlastHackNet/mod_s0beit_sa/

        mod_sa is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        mod_sa is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with mod_sa.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef RAKHOOK_INTERFACE_HPP
#define RAKHOOK_INTERFACE_HPP


class c_rakclient_interface {
public:
    c_rakclient_interface(RakClientInterface *rc){};
    virtual ~c_rakclient_interface() = default;

    virtual bool Connect(const char *host, unsigned short serverPort, unsigned short clientPort, unsigned int depreciated, int threadSleepTimer) {
        return rakhook::rakclient->Connect(host, serverPort, clientPort, depreciated, threadSleepTimer);
    }

    virtual void Disconnect(unsigned int blockDuration, unsigned char orderingChannel = 0) {
        rakhook::rakclient->Disconnect(blockDuration, orderingChannel);
    }

    virtual void InitializeSecurity(const char *privKeyP, const char *privKeyQ) {
        rakhook::rakclient->InitializeSecurity(privKeyP, privKeyQ);
    }

    virtual void SetPassword(const char *_password) {
        rakhook::rakclient->SetPassword(_password);
    }

    virtual bool HasPassword(void) const {
        return rakhook::rakclient->HasPassword();
    }

    virtual bool Send(const char *data, const int length, PacketPriority priority, PacketReliability reliability, char orderingChannel) {
        return rakhook::rakclient->Send(data, length, priority, reliability, orderingChannel);
    }

    virtual bool Send(RakNet::BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel) {
        for (auto it = rakhook::on_send_packet.begin(); it != rakhook::on_send_packet.end();) {
            if (auto f = *it) {
                if (!f(bitStream, priority, reliability, orderingChannel))
                    return false;
                it++;
            } else {
                it = rakhook::on_send_packet.erase(it);
            }
        }
        return rakhook::rakclient->Send(bitStream, priority, reliability, orderingChannel);
    }

    virtual Packet *Receive(void) {
        Packet *packet = rakhook::rakclient->Receive();
        if (!packet)
            return nullptr;

        for (auto it = rakhook::on_receive_packet.begin(); it != rakhook::on_receive_packet.end();) {
            if (auto f = *it) {
                if (!f(packet)) {
                    rakhook::rakclient->DeallocatePacket(packet);
                    return nullptr;
                }
                it++;
            } else {
                it = rakhook::on_receive_packet.erase(it);
            }
        }
        return packet;
    }

    virtual void DeallocatePacket(Packet *packet) {
        rakhook::rakclient->DeallocatePacket(packet);
    }

    virtual void PingServer(void) {
        rakhook::rakclient->PingServer();
    }

    virtual void PingServer(const char *host, unsigned short serverPort, unsigned short clientPort, bool onlyReplyOnAcceptingConnections) {
        rakhook::rakclient->PingServer(host, serverPort, clientPort, onlyReplyOnAcceptingConnections);
    }

    virtual int GetAveragePing(void) {
        return rakhook::rakclient->GetAveragePing();
    }

    virtual int GetLastPing(void) const {
        return rakhook::rakclient->GetLastPing();
    }

    virtual int GetLowestPing(void) const {
        return rakhook::rakclient->GetLowestPing();
    }

    virtual int GetPlayerPing(const PlayerID playerId) {
        return rakhook::rakclient->GetPlayerPing(playerId);
    }

    virtual void StartOccasionalPing(void) {
        rakhook::rakclient->StartOccasionalPing();
    }

    virtual void StopOccasionalPing(void) {
        rakhook::rakclient->StopOccasionalPing();
    }

    virtual bool IsConnected(void) const {
        return rakhook::rakclient->IsConnected();
    }

    virtual unsigned int GetSynchronizedRandomInteger(void) const {
        return rakhook::rakclient->GetSynchronizedRandomInteger();
    }

    virtual bool GenerateCompressionLayer(unsigned int inputFrequencyTable[256], bool inputLayer) {
        return rakhook::rakclient->GenerateCompressionLayer(inputFrequencyTable, inputLayer);
    }

    virtual bool DeleteCompressionLayer(bool inputLayer) {
        return rakhook::rakclient->DeleteCompressionLayer(inputLayer);
    };

    virtual void RegisterAsRemoteProcedureCall(int *uniqueID, void (*functionPointer)(RPCParameters *rpcParms)) {
        rakhook::rakclient->RegisterAsRemoteProcedureCall(uniqueID, functionPointer);
    };

    virtual void RegisterClassMemberRPC(int *uniqueID, void *functionPointer) {
        rakhook::rakclient->RegisterClassMemberRPC(uniqueID, functionPointer);
    }

    virtual void UnregisterAsRemoteProcedureCall(int *uniqueID) {
        rakhook::rakclient->UnregisterAsRemoteProcedureCall(uniqueID);
    }

    virtual bool RPC(int *uniqueID, const char *data, unsigned int bitLength, PacketPriority priority, PacketReliability reliability, char orderingChannel,
                     bool shiftTimestamp) {
        return rakhook::rakclient->RPC(uniqueID, data, bitLength, priority, reliability, orderingChannel, shiftTimestamp);
    };

    virtual bool RPC(int *uniqueID, RakNet::BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel,
                     bool shiftTimestamp) {
        if (!uniqueID)
            return rakhook::rakclient->RPC(uniqueID, bitStream, priority, reliability, orderingChannel, shiftTimestamp);

        int _uniqueID = *uniqueID;
        for (auto it = rakhook::on_send_rpc.begin(); it != rakhook::on_send_rpc.end();) {
            if (auto f = *it) {
                if (!f(_uniqueID, bitStream, priority, reliability, orderingChannel, shiftTimestamp))
                    return false;
                it++;
            } else {
                it = rakhook::on_send_rpc.erase(it);
            }
        }
        return rakhook::rakclient->RPC(&_uniqueID, bitStream, priority, reliability, orderingChannel, shiftTimestamp);
    }

    virtual bool RPC_(int *uniqueID, RakNet::BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel,
                      bool shiftTimestamp, NetworkID networkID) {
        return rakhook::rakclient->RPC_(uniqueID, bitStream, priority, reliability, orderingChannel, shiftTimestamp, networkID);
    }

    virtual void SetTrackFrequencyTable(bool b) {
        return rakhook::rakclient->SetTrackFrequencyTable(b);
    }

    virtual bool GetSendFrequencyTable(unsigned int outputFrequencyTable[256]) {
        return rakhook::rakclient->GetSendFrequencyTable(outputFrequencyTable);
    }

    virtual float GetCompressionRatio(void) const {
        return rakhook::rakclient->GetCompressionRatio();
    }

    virtual float GetDecompressionRatio(void) const {
        return rakhook::rakclient->GetDecompressionRatio();
    }

    virtual void AttachPlugin(void *messageHandler) {
        return rakhook::rakclient->AttachPlugin(messageHandler);
    }

    virtual void DetachPlugin(void *messageHandler) {
        return rakhook::rakclient->DetachPlugin(messageHandler);
    }

    virtual RakNet::BitStream *GetStaticServerData(void) {
        return rakhook::rakclient->GetStaticServerData();
    }

    virtual void SetStaticServerData(const char *data, const int length) {
        return rakhook::rakclient->SetStaticServerData(data, length);
    }

    virtual RakNet::BitStream *GetStaticClientData(const PlayerID playerId) {
        return rakhook::rakclient->GetStaticClientData(playerId);
    }

    virtual void SetStaticClientData(const PlayerID playerId, const char *data, const int length) {
        return rakhook::rakclient->SetStaticClientData(playerId, data, length);
    }

    virtual void SendStaticClientDataToServer(void) {
        return rakhook::rakclient->SendStaticClientDataToServer();
    }

    virtual PlayerID GetServerID(void) const {
        return rakhook::rakclient->GetServerID();
    }

    virtual PlayerID GetPlayerID(void) const {
        return rakhook::rakclient->GetPlayerID();
    }

    virtual PlayerID GetInternalID(void) const {
        return rakhook::rakclient->GetInternalID();
    }

    virtual const char *PlayerIDToDottedIP(const PlayerID playerId) const {
        return rakhook::rakclient->PlayerIDToDottedIP(playerId);
    }

    virtual void PushBackPacket(Packet *packet, bool pushAtHead) {
        return rakhook::rakclient->PushBackPacket(packet, pushAtHead);
    }

    virtual void SetRouterInterface(void *routerInterface) {
        return rakhook::rakclient->SetRouterInterface(routerInterface);
    }

    virtual void RemoveRouterInterface(void *routerInterface) {
        return rakhook::rakclient->RemoveRouterInterface(routerInterface);
    }

    virtual void SetTimeoutTime(RakNetTime timeMS) {
        return rakhook::rakclient->SetTimeoutTime(timeMS);
    }

    virtual bool SetMTUSize(int size) {
        return rakhook::rakclient->SetMTUSize(size);
    }

    virtual int GetMTUSize(void) const {
        return rakhook::rakclient->GetMTUSize();
    }

    virtual void AllowConnectionResponseIPMigration(bool allow) {
        return rakhook::rakclient->AllowConnectionResponseIPMigration(allow);
    }

    virtual void AdvertiseSystem(const char *host, unsigned short remotePort, const char *data, int dataLength) {
        return rakhook::rakclient->AdvertiseSystem(host, remotePort, data, dataLength);
    }

    virtual void *const GetStatistics(void) {
        return rakhook::rakclient->GetStatistics();
    }

    virtual void ApplyNetworkSimulator(double maxSendBPS, unsigned short minExtraPing, unsigned short extraPingVariance) {
        return rakhook::rakclient->ApplyNetworkSimulator(maxSendBPS, minExtraPing, extraPingVariance);
    }

    virtual bool IsNetworkSimulatorActive(void) {
        return rakhook::rakclient->IsNetworkSimulatorActive();
    }

    virtual PlayerIndex GetPlayerIndex(void) {
        return rakhook::rakclient->GetPlayerIndex();
    }
};


#endif // RAKHOOK_INTERFACE_HPP