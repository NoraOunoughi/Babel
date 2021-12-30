/*
** EPITECH PROJECT 2021
** babel
** File description:
** Packet.cpp
*/

#include "Packet.hpp"

BabelPacket::BabelPacket() : _valid(false), _packetSize(0), _data() {}
BabelPacket::~BabelPacket() {}

size_t BabelPacket::getSize()
{
    return _packetSize;
}

const void *BabelPacket::getData()
{
    return _data.byteArray;
}

uint16_t BabelPacket::getOpCode(const void *data)
{
    const uint16_t *opCodeArray = static_cast<const uint16_t *>(data);
    uint16_t opCode = ntohs(*opCodeArray);
    if (opCode != UserWhantToLogin && opCode != ServerLoginResponse &&
        opCode != UserWhantToCall && opCode != ServerResponse &&
        opCode != ServerCallRequest &&opCode != UserAcceptCall &&
        opCode != LaunchCall && opCode != UserHangUp && opCode != AcceptToLogout)
        return 0;
    return opCode;
}

void BabelPacket::deserialize(const void *data)
{
    // On récupére les 1- premiers bytes pour savoir le type du Packet
    // Donc le code d'opération, on sait maintenant quel union struct ça représente
    uint16_t opCode = getOpCode(data);
    _data.packed.type = opCode;
    if (opCode == UserWhantToLogin)
        deserialize200(data);
    if (opCode == ServerLoginResponse)
        deserialize201(data);
    if (opCode == UserWhantToCall)
        deserialize203(data);
    if (opCode == AcceptToLogout)
        deserialize204(data);
    if (opCode == ServerResponse)
        deserialize100(data);
    if (opCode == ServerCallRequest)
        deserialize101(data);
    if (opCode == UserAcceptCall)
        deserialize102(data);
    if (opCode == LaunchCall)
        deserialize103(data);
    if (opCode == UserHangUp)
        deserialize104(data);
}

void BabelPacket::clear()
{
    _packetSize = 0;
    _valid = false;
    std::memset(_data.byteArray, 0, sizeof(_data.byteArray));
}

bool BabelPacket::isValid()
{
    return _valid;
}

// ServerResponse serialize
void BabelPacket::serialize100(bool &response, const std::string &msg)
{
    _valid = true;
    _packetSize = sizeof(BabelOpCode) + sizeof(_data.packed.data._100.response) +
    sizeof(_data.packed.data._100.msg) + sizeof(_data.packed.data._100.msgLength);
    _data.packed.type = htons(ServerResponse);
    _data.packed.data._100.response = response;
    _data.packed.data._100.msgLength = htonl(msg.size());
    std::memcpy(_data.packed.data._100.msg, msg.c_str(), msg.size());
}

// UserWhantToLogin serialize
void BabelPacket::serialize200(std::string &username, std::size_t &port)
{
    _valid = true;
    _packetSize = sizeof(BabelOpCode) + sizeof(_data.packed.data._200.username) + sizeof(_data.packed.data._200.usernameLength) + sizeof(_data.packed.data._200.port);
    _data.packed.type = htons(UserWhantToLogin);
    _data.packed.data._200.usernameLength = htonl(username.size());
    _data.packed.data._200.port = htonl(port);
    std::memcpy(_data.packed.data._200.username, username.c_str(), sizeof(_data.packed.data._200.username));
}

// ServerLoginResponse serialize
void BabelPacket::serialize201(bool &response, const std::string &msg, std::size_t &userId)
{
    _valid = true;
    _packetSize = sizeof(BabelOpCode) + sizeof(_data.packed.data._201.response) + sizeof(_data.packed.data._201.userId) + sizeof(_data.packed.data._201.msg) + sizeof(_data.packed.data._201.msgLength);
    _data.packed.type = htons(ServerLoginResponse);
    _data.packed.data._201.response = response;
    _data.packed.data._201.userId = htonl(userId);
    _data.packed.data._201.msgLength = htonl(msg.size());
    std::memcpy(_data.packed.data._201.msg, msg.c_str(), msg.size());
}

// AcceptLogout serialize
void BabelPacket::serialize204(bool &response, std::size_t &destId)
{
    _valid = true;
    _packetSize = sizeof(BabelOpCode) + sizeof(_data.packed.data._204.response) + sizeof(_data.packed.data._204.destId);
    _data.packed.type = htons(AcceptToLogout);
    _data.packed.data._204.response = response;
    _data.packed.data._204.destId = htonl(destId);
}

// UserWhantToCall serialize
void BabelPacket::serialize203(std::size_t &fromId, std::size_t &destId)
{
    _valid = true;
    _packetSize = sizeof(BabelOpCode) + sizeof(_data.packed.data._203.fromId) + sizeof(_data.packed.data._203.destId);
    _data.packed.type = htons(UserWhantToCall);
    _data.packed.data._203.fromId = htonl(fromId);
    _data.packed.data._203.destId = htonl(destId);
}

// ServerCallRequest serialize
void BabelPacket::serialize101(std::size_t &fromId, std::size_t &destId)
{
    _valid = true;
    _packetSize = sizeof(BabelOpCode) + sizeof(_data.packed.data._101.fromId) + sizeof(_data.packed.data._101.destId);
    _data.packed.type = htons(ServerCallRequest);
    _data.packed.data._101.fromId = htonl(fromId);
    _data.packed.data._101.destId = htonl(destId);
}

// UserAcceptCall serialize
void BabelPacket::serialize102(bool &response, std::size_t &destId)
{
    _valid = true;
    _packetSize = sizeof(BabelOpCode) + sizeof(_data.packed.data._102.response) + sizeof(_data.packed.data._102.destId);
    _data.packed.type = htons(UserAcceptCall);
    _data.packed.data._102.response = response;
    _data.packed.data._102.destId = htonl(destId);
}

// LaunchCall serialize
void BabelPacket::serialize103(std::string &ip, std::size_t &port, std::size_t &fromId, std::size_t &destId)
{
    _valid = true;
    _packetSize = sizeof(BabelOpCode) + sizeof(_data.packed.data._103.fromId) + sizeof(_data.packed.data._103.destId)
+ sizeof(_data.packed.data._103.ipLength) + sizeof(_data.packed.data._103.ip);
    _data.packed.type = htons(LaunchCall);
    _data.packed.data._103.fromId = htonl(fromId);
    _data.packed.data._103.destId = htonl(destId);
    _data.packed.data._103.portDest = htonl(port);
    _data.packed.data._103.ipLength = htonl(ip.size());
    std::memcpy(_data.packed.data._103.ip, ip.c_str(), ip.size());
}

// UserHangUp serialize
void BabelPacket::serialize104(std::size_t &fromId, std::size_t &destId)
{
    _valid = true;
    _packetSize = sizeof(BabelOpCode) + sizeof(_data.packed.data._104.fromId) + sizeof(_data.packed.data._104.destId);
    _data.packed.type = htons(UserHangUp);
    _data.packed.data._104.fromId = htonl(fromId);
    _data.packed.data._104.destId = htonl(destId);
}

// ServerResponse deserialize
void BabelPacket::deserialize100(const void *data)
{
    clear();
    std::size_t neededSize = sizeof(uint16_t) + sizeof(_data.packed.data._100.response) +
    sizeof(_data.packed.data._100.msgLength) + sizeof(_data.packed.data._100.msg);
    std::memcpy(_data.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
}

// UserWhantToLogin deserialize
void BabelPacket::deserialize200(const void *data)
{
    clear();
    std::size_t neededSize = sizeof(uint16_t) + sizeof(_data.packed.data._200.port) + sizeof(_data.packed.data._200.usernameLength) + sizeof(_data.packed.data._200.username);
    std::memcpy(_data.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
}

// ServerLoginResponse deserialize
void BabelPacket::deserialize201(const void *data)
{
    clear();
    std::size_t neededSize = sizeof(uint16_t) + sizeof(_data.packed.data._201.response) + sizeof(_data.packed.data._201.userId) + sizeof(_data.packed.data._201.msgLength) + sizeof(_data.packed.data._201.msg);
    std::memcpy(_data.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;

}

// AcceptLogout deserialize
void BabelPacket::deserialize204(const void *data)
{
    clear();
    std::size_t neededSize = sizeof(uint16_t) + sizeof(_data.packed.data._204.response) + sizeof(_data.packed.data._204.destId);
    std::memcpy(_data.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
}

// UserWhantToCall deserialize
void BabelPacket::deserialize203(const void *data)
{
    clear();
    std::size_t neededSize = sizeof(uint16_t) + sizeof(_data.packed.data._203.fromId) + sizeof(_data.packed.data._203.destId);
    std::memcpy(_data.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
}

// ServerCallRequest deserialize
void BabelPacket::deserialize101(const void *data)
{
    clear();
    std::size_t neededSize = sizeof(uint16_t) + sizeof(_data.packed.data._101.fromId) + sizeof(_data.packed.data._101.destId);
    std::memcpy(_data.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
}

// UserAcceptcall deserialize
void BabelPacket::deserialize102(const void *data)
{
    clear();
    std::size_t neededSize = sizeof(uint16_t) + sizeof(_data.packed.data._102.response) + sizeof(_data.packed.data._102.destId);
    std::memcpy(_data.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
}

// LaunchCall deserialize
void BabelPacket::deserialize103(const void *data)
{
    clear();
    std::size_t neededSize = sizeof(uint16_t) + sizeof(_data.packed.data._103.fromId) + sizeof(_data.packed.data._103.destId)
+ sizeof(_data.packed.data._103.ipLength) + sizeof(_data.packed.data._103.ip);
    std::memcpy(_data.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
}

// UserHangUp deserialize
void BabelPacket::deserialize104(const void * data)
{
    clear();
    std::size_t neededSize = sizeof(uint16_t) + sizeof(_data.packed.data._104.fromId) + sizeof(_data.packed.data._104.destId);
    std::memcpy(_data.byteArray, data, neededSize);
    _valid = true;
    _packetSize = neededSize;
}






// ServerResponse extract
void BabelPacket::extract100(bool &response, std::string &msg)
{
    response = _data.packed.data._100.response;
    std::size_t msgLength = ntohl(_data.packed.data._100.msgLength);
    msg.assign(_data.packed.data._100.msg, msgLength);
}

// UserWhantToLogin extract
void BabelPacket::extractUsername(std::string &username, std::size_t &port)
{
    std::size_t usernameLength = ntohl(_data.packed.data._200.usernameLength);
    port = ntohl(_data.packed.data._200.port);
    username.assign(_data.packed.data._200.username, usernameLength);
}

// ServerLoginResponse extract
void BabelPacket::extract201(bool &response, std::string &msg, std::size_t &userId)
{
    response = _data.packed.data._201.response;
    std::size_t msgLength = ntohl(_data.packed.data._201.msgLength);
    msg.assign(_data.packed.data._201.msg, msgLength);
    userId = ntohl(_data.packed.data._201.userId);
}

// AcceptLogout extract
void BabelPacket::extract204(bool &response, std::size_t &destId)
{
    response = _data.packed.data._204.response;
    destId = ntohl(_data.packed.data._204.destId);
}

// UserWhantToCall extract
void BabelPacket::extractCallInfos(std::size_t &fromId, std::size_t &destId)
{
    fromId = ntohl(_data.packed.data._203.fromId);
    destId = ntohl(_data.packed.data._203.destId);
}

// ServerCallRequest extract
void BabelPacket::extractCallRequest(std::size_t &fromId, std::size_t &destId)
{
    fromId = ntohl(_data.packed.data._101.fromId);
    destId = ntohl(_data.packed.data._101.destId);
}

// UserAcceptcall extract
void BabelPacket::extract102(bool &response, std::size_t &destId)
{
    response = _data.packed.data._102.response;
    destId = ntohl(_data.packed.data._102.destId);
}

// LaunchCall extract
void BabelPacket::extract103(std::size_t &fromId, std::size_t &destId, std::string &ip, std::size_t &port)
{
    fromId = ntohl(_data.packed.data._103.fromId);
    destId = ntohl(_data.packed.data._103.destId);
    port = ntohl(_data.packed.data._103.portDest);
    std::size_t ipLength = ntohl(_data.packed.data._103.ipLength);
    ip.assign(_data.packed.data._103.ip, ipLength);
}

// UserHangUp extract
void BabelPacket::extractHangUp(std::size_t &fromId, std::size_t &destId)
{
    fromId = ntohl(_data.packed.data._104.fromId);
    destId = ntohl(_data.packed.data._104.destId);
}