/*
** EPITECH PROJECT 2021
** babel_server
** File description:
** BabelServer.hpp
*/

#ifndef BABEL_SERVER
#define BABEL_SERVER

#include "../Network/Network.hpp"
#include "../clientServer/Packet.hpp"
#include "BabelUser.hpp"

static int caugh_sigint = 0;

class BabelServer {
    public:
        BabelServer(const std::size_t &port);
        ~BabelServer();

        void handleRequest(BabelUser &user);
        void handleLoginRequest(BabelUser &user, BabelPacket &receive);
        void handleCallRequest(BabelUser &user, BabelPacket &receive);
        void handleAcceptCallRequest(BabelUser &user, BabelPacket &receive);
        void handleHangUpCallRequest(BabelUser &user, BabelPacket &receive);
        void handleLogoutRequest(BabelUser &user, BabelPacket &receive);
        
        void handleConnection();
        void handleDisconnection(std::vector<std::size_t> disconnectedIds);

        void run();
        void close();

    private:
        Network *_network;
        tcp_server *_serverTCP;
        std::map<std::size_t, BabelUser> _allUsers;
};

#endif /* BABEL_SERVER_HPP_ */
