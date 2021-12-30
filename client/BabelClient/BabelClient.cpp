/*
** EPITECH PROJECT 2021
** babel_server
** File description:
** BabelClient.cpp
*/

#include "BabelClient.hpp"

BabelClient::BabelClient(std::string &tcpIp, std::size_t portTCP, std::size_t portUdp) : _io_context()
{
    _start = true;
    _callerId = -1;
    whereAmI = "register";
    _portUdp = portUdp;
    _user = new User();
    _clientTCP = new tcp_client(tcpIp, portTCP, _io_context);
}


std::string BabelClient::getWhereAmI()
{
    return whereAmI;
}

void BabelClient::setWhereAmI(std::string &where)
{
    whereAmI = where;
}

void BabelClient::handleLoginResponse(BabelPacket &recBin)
{
    std::string msg;
    bool res = false;
    std::size_t userId;

    recBin.extract201(res, msg, userId);
    if (res == true) {
        _user->setConnected(userId);
        std::cout << "You are now connected " << _user->getPseudo() << " with id: " << _user->getID() << std::endl;
        whereAmI = "home";
    } else
        std::cerr << msg << std::endl;
}

void BabelClient::handleSomeoneCalling(BabelPacket &recBin)
{
    std::size_t fromId;
    std::size_t destId;

    recBin.extractCallRequest(fromId, destId);

    _callerId = fromId;
    std::cout << fromId << " want to call you" << std::endl;
    std::cout << "Use command \"accept\" or \"deny\"" << std::endl;
    whereAmI = "calling";
}

void BabelClient::handleLaunchCall(BabelPacket &recBin)
{
    std::string ipUdpDest;
    std::size_t portUdpDest;
    std::size_t fromId;
    std::size_t destId;

    recBin.extract103(fromId, destId, ipUdpDest, portUdpDest);

    std::cout << "Call from " << fromId << " to " << destId << std::endl;
    std::cout << "Launch call on " << ipUdpDest << ":" << portUdpDest << std::endl;
    _user->setIsInCall(true);
    /// A decmmenter pour audio
    // _call = new Call(ipUdpDest, portUdpDest, _portUdp);
    // _callThread = new(std::nothrow) std::thread([&] { _call->launchCall(); });
    whereAmI = "call";
}

void BabelClient::handleHangUp(BabelPacket &recBin)
{
    std::size_t fromId;
    std::size_t destId;

    recBin.extractHangUp(fromId, destId);
    if (_user->getIsInCall() == true) {
        /// A decmmenter pour audio
        // _call->hangUpCall();
        // delete(_call);
        // _callThread->join();
        // delete(_callThread);
        _user->setIsInCall(false);
        _callerId = -1;
    }
    std::cout << "Call hang up" << std::endl;
    whereAmI = "home";
}

void BabelClient::handleBasicResponse(BabelPacket &recBin)
{
    std::string msg;
    bool res = false;

    recBin.extract100(res, msg);
    if (res == true) {
        std::cout << msg << std::endl;
    } else {
        std::cerr << msg << std::endl;
        whereAmI = "home";
    }
}

void BabelClient::handleResponse(std::string buffer)
{
    unsigned char bin[4096];
    std::size_t bytes_transferred = buffer.size();
    BabelPacket recBin;
    uint16_t opCode;
    
    memcpy(bin, buffer.c_str(), bytes_transferred);
    opCode = recBin.getOpCode(bin);
    recBin.deserialize(bin);

    if (opCode == BabelPacket::ServerLoginResponse) {
        handleLoginResponse(recBin);
    } else if (opCode == BabelPacket::ServerCallRequest) {
        handleSomeoneCalling(recBin);
        // Affichage page accept / deny call...
    } else if (opCode == BabelPacket::ServerResponse) {
        handleBasicResponse(recBin);
    } else if (opCode == BabelPacket::LaunchCall) {
        handleLaunchCall(recBin);
    } else if (opCode == BabelPacket::UserHangUp) {
        handleHangUp(recBin);
    }
    memset(bin, 0, bytes_transferred);
}

void BabelClient::acceptDenyCall(bool request, std::size_t fromId)
{
    if (caugh_sigint == 1)
        return;
    BabelPacket packet;
    bool res = true;
    if (request) {
        packet.serialize102(res, fromId);
        _clientTCP->sendMessageToServer(packet);
        whereAmI = "call";
    } else {
        res = false;
        packet.serialize102(res, fromId);
        _clientTCP->sendMessageToServer(packet);
        _callerId = -1;
        whereAmI = "home";
    }
    packet.clear();
}

void BabelClient::login(std::string &username)
{
    if (caugh_sigint == 1)
        return;
    BabelPacket packet;
    packet.serialize200(username, _portUdp);
    _clientTCP->sendMessageToServer(packet);
    _user->setPseudo(username);
}

void BabelClient::logout(bool request, std::size_t fromId)
{
    BabelPacket packet;
    bool res = true;

    if (caugh_sigint == 1)
        return;
    if (request) {
        packet.serialize204(res, fromId);
        _clientTCP->sendMessageToServer(packet);
        whereAmI = "start";
        _user->setDisconnected();
    } else {
        res = false;
        packet.serialize204(res, fromId);
        _clientTCP->sendMessageToServer(packet);
    }
    packet.clear();
}

void BabelClient::sendCallRequest(std::size_t destId)
{
    if (caugh_sigint == 1)
        return;
    _callerId = destId;
    std::size_t fromId = _user->getID();
    BabelPacket packet;
    packet.serialize203(fromId, destId);
    _clientTCP->sendMessageToServer(packet);
    whereAmI = "call";
}

void BabelClient::sendHangUp()
{
    if (_user->getIsInCall() == false) {
        whereAmI = "home";
        return;
    }
    _user->setIsInCall(false);
    BabelPacket hangUpPacket;
    std::size_t fromId = _user->getID();
    hangUpPacket.serialize104(fromId, _callerId);
    _clientTCP->getClientSession().get()->sendMessage(hangUpPacket);
    hangUpPacket.clear();
    _callerId = -1;
    whereAmI = "home";
    std::cout << "You hangup" << std::endl;
}

void BabelClient::shell()
{
    while (_start && caugh_sigint == 0) {
        std::string request;
        std::getline(std::cin, request);
        if (request == "login") {
            if (_user->isConnected()) {
                std::cerr << "You are already connected" << std::endl;
            } else {
                std::string us = "Pierre";
                login(us);
            }
        }
        else if (request == "logout") {
            if (_user->isConnected() == true) {
                logout(false, _user->getID());
                std::cout << "Disconnected!" << std::endl;
            } else
                std::cout << "You are already disconnected" << std::endl;
        }
        else if (request == "exit") {
            _start = false;
        }
        else if (request == "call") {
            if (_user->isConnected() == false) {
                    std::cerr << "You are not connected, please use 'login'" << std::endl;
            } else {
                std::size_t destId = 1;
                sendCallRequest(destId);
            }
        } else if (request == "accept" && _callerId != -1 && _user->getIsInCall() == false) {
            acceptDenyCall(true, _callerId);
        } else if (request == "deny" && _callerId != -1 && _user->getIsInCall() == false) {
            acceptDenyCall(false, _callerId);
        } else if (request == "hangup" && _user->getIsInCall() == true) {
            sendHangUp();
        } else {
            std::cout << "Actuals commands are \"login\" or \"call\"" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::nanoseconds(200));
    }
}

void BabelClient::handler()
{
    _clientTCP->launch();
    while (_start && caugh_sigint == 0) {
        if (!_clientTCP->getClientSession().get()->getData().empty()) {
            std::string receive = _clientTCP->getClientSession().get()->getDataClear();
            handleResponse(receive);
        }
        std::this_thread::sleep_for(std::chrono::nanoseconds(200));
    }
}

static void signalHandler(int signum)
{
    std::cerr << "Interrupt signal " << signum << " received" << std::endl;
    caugh_sigint = 1;
}

void BabelClient::run()
{
    signal(SIGINT, signalHandler);
    int ac;
    char **av;
    threadHandler_ = new(std::nothrow) std::thread([&] { handler(); });
}

void BabelClient::stop()
{
    _start = false;
    if (threadHandler_) {
        threadHandler_->join();
        delete(threadHandler_);
    }
    delete(_user);
    delete (_clientTCP);
    std::cout << "Stop finish BabelClient" << std::endl;
}

std::size_t BabelClient::getCallerId()
{
    return _callerId;
}

std::size_t BabelClient::getId()
{
    return _user->getID();
}