/*
** EPITECH PROJECT 2021
** babel_server
** File description:
** BabelServer.hpp
*/

#include "BabelServer.hpp"

BabelServer::BabelServer(const std::size_t &port) : _allUsers()
{
    _network = new Network();
    _serverTCP = _network->createServer(port);
}

BabelServer::~BabelServer()
{
}

void BabelServer::handleLoginRequest(BabelUser &user, BabelPacket &receive)
{
    std::size_t fromId = user.getId();
    std::string username;
    std::size_t port;
    bool s = true;
    BabelPacket sendBin;

    receive.extractUsername(username, port);
    std::cout << username << " want to login" << " / portUdp: " << port << std::endl;

    std::string t = "Login sucess";
    if (user.isConnected() == false) {
        sendBin.serialize201(s, t, fromId);
        user.getSession().get()->sendMessage(sendBin);
        sendBin.clear();
        user.connected();
        user.setUdpPort(port);
    } else {
        s = false;
        t = "You are already connected";
        sendBin.serialize201(s, t, fromId);
        user.getSession().get()->sendMessage(sendBin);
        sendBin.clear();
    }
}

void BabelServer::handleCallRequest(BabelUser &user, BabelPacket &receive)
{
    std::size_t fromId = user.getId();
    std::size_t destId;
    std::string ipSrc = user.getSession().get()->getIp();
    std::size_t portUdpSrc = user.getUdpPort();
    std::string ipDest;
    std::size_t portUdpDest;
    BabelPacket basicResponse;
    bool s;
    std::string msg;

    receive.extractCallInfos(fromId, destId);
    std::cout << fromId << " want to call " << destId << std::endl;

    auto dest = _allUsers.find(destId);
    if (dest == _allUsers.end() || dest->second.isConnected() == false) {
        s = false;
        msg = "User " + std::to_string(destId) + " is not online";
        basicResponse.serialize100(s, msg);
        user.getSession().get()->sendMessage(basicResponse);
        basicResponse.clear();
    } else if (destId == fromId) {
        s = false;
        msg = "You can't call yourself";
        basicResponse.serialize100(s, msg);
        user.getSession().get()->sendMessage(basicResponse);
        basicResponse.clear();
    } else {
        s = true;
        msg = "We are calling " + std::to_string(destId);
        basicResponse.serialize100(s, msg);
        user.getSession().get()->sendMessage(basicResponse);
        basicResponse.clear();

        ipDest = dest->second.getSession().get()->getIp();
        portUdpDest = dest->second.getUdpPort();

        std::cout << ipSrc << ":" << portUdpSrc << " want to call " << ipDest << ":" << portUdpDest << std::endl;

        msg = std::to_string(fromId) + " want to call you";
        basicResponse.serialize101(fromId, destId);
        dest->second.getSession().get()->sendMessage(basicResponse);
        basicResponse.clear();
    }
}

void BabelServer::handleAcceptCallRequest(BabelUser &user, BabelPacket &receive)
{
    bool response = false;
    std::size_t destId;
    BabelPacket packet;
    bool res = true;
    std::string msg = "Your call is accepted";
    BabelPacket launchPacket;
    std::size_t lPdestId = user.getId();
    std::string ipDest = user.getSession().get()->getIp();
    std::size_t portUdpDest = user.getUdpPort();
    std::size_t lPfromId;
    std::string ipSrc;
    std::size_t portUdpSrc;

    receive.extract102(response, destId);
    if (response == true) {
        std::cout << "User accept call" << std::endl;
        
        auto dest = _allUsers.find(destId);
        if (dest != _allUsers.end()) {
            packet.serialize100(res, msg);
            dest->second.getSession().get()->sendMessage(packet);
            packet.clear();
        }

        lPfromId = dest->second.getId();
        ipSrc = dest->second.getSession().get()->getIp();
        portUdpSrc = dest->second.getUdpPort();

        launchPacket.serialize103(ipDest, portUdpDest, lPfromId, lPdestId);
        dest->second.getSession().get()->sendMessage(launchPacket);
        launchPacket.clear();

        launchPacket.serialize103(ipSrc, portUdpSrc, lPfromId, lPdestId);
        user.getSession().get()->sendMessage(launchPacket);
        launchPacket.clear();
    } else {
        std::cout << "User deny call" << std::endl;
        res = false;
        msg = "Your call has been deny";
        auto dest = _allUsers.find(destId);
        if (dest != _allUsers.end()) {
            packet.serialize100(res, msg);
            dest->second.getSession().get()->sendMessage(packet);
            packet.clear();
        }
    }
}

void BabelServer::handleHangUpCallRequest(BabelUser &user, BabelPacket &receive)
{
    std::size_t fromId;
    std::size_t destId;
    BabelPacket hangUpPacket;

    std::cout << "close" << std::endl;
    receive.extractHangUp(fromId, destId);
    std::cout << fromId << " want to close the call with " << destId << std::endl;

    auto dest = _allUsers.find(destId);
    if (dest != _allUsers.end()) {
        hangUpPacket.serialize104(destId, fromId);
        dest->second.getSession().get()->sendMessage(hangUpPacket);
        hangUpPacket.clear();
    }
}

void BabelServer::handleLogoutRequest(BabelUser &user, BabelPacket &receive)
{
    bool res;
    size_t id;

    receive.extract204(res, id);
    std::cout << id << " want to logout" << std::endl;
    user.disconnected();
}

void BabelServer::handleRequest(BabelUser &user)
{
    std::size_t fromId = user.getId();
    unsigned char data[4096];
    BabelPacket receive;
    memcpy(data, user.getSession().get()->getData(), user.getSession().get()->getDataSize());
    receive.deserialize(data);
    uint16_t type = receive.getOpCode(data);
    if (type != 0) {
        std::cout << "Get Request" << std::endl;
        std::cout << "OPCODE " << type << std::endl;
    }

    if (type == BabelPacket::UserWhantToLogin) {
        handleLoginRequest(user, receive);
    }
    if (user.isConnected() == false) return;

    if (type == BabelPacket::AcceptToLogout) {
        handleLogoutRequest(user, receive);
    }
    if (type == BabelPacket::UserWhantToCall) {
        handleCallRequest(user, receive);
    }
    if (type == BabelPacket::UserAcceptCall) {
        handleAcceptCallRequest(user, receive);
    }
    if (type == BabelPacket::UserHangUp) {
        handleHangUpCallRequest(user, receive);
    }
    user.getSession().get()->clearData();
    memset(data, 0, sizeof(data));
    receive.clear();
}

void BabelServer::handleConnection()
{
    if (!_serverTCP->getAllNewIds().empty()) {
        std::vector<std::size_t> newIds = _serverTCP->getAllNewIds();
        for (std::size_t id : newIds) {
            std::cout << "One new user " << id << std::endl;
            auto client = _serverTCP->getClientById(id);
            std::cout << client->getIp() << std::endl;
            BabelUser user(id, client);
            _allUsers.emplace(id, user);
        }
        _serverTCP->clearNewIds();
    }
}

void BabelServer::handleDisconnection(std::vector<std::size_t> disconnectedIds)
{
    for (auto toDelId : disconnectedIds) {
        auto toDel = _allUsers.find(toDelId);
        if (toDel != _allUsers.end())
            _allUsers.erase(toDel);
    }
}

static void signalHandler(int signum)
{
    std::cerr << "Interrupt signal " << signum << " received" << std::endl;
    caugh_sigint = 1;
}

void BabelServer::run()
{
    std::vector<std::size_t> disconnectedIds;

    signal(SIGINT, signalHandler);
    _serverTCP->launch();
    while (caugh_sigint == 0) {
        handleConnection();
        for (auto it = _allUsers.begin(); it != _allUsers.end(); ++it) {
            if (it->second.getSession().get()->isClosed()) {
                std::size_t userIdDisconnect = it->first;
                std::cout << "BabelServer knows " << userIdDisconnect << " want to disconnect" << std::endl;
                _serverTCP->handleClientDisconnect(userIdDisconnect);
                disconnectedIds.push_back(userIdDisconnect);
                it->second.disconnected();
            }
            if (it->second.getSession().get()->hasDataValid()) {
                // std::cout << "Get request" << std::endl;
                handleRequest(it->second);
            }
        }
        handleDisconnection(disconnectedIds);
    }
    close();
}

void BabelServer::close()
{
    _serverTCP->stop();
    _network->destroyServer(_serverTCP);
    delete _network;
}
