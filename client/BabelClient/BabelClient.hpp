/*
** EPITECH PROJECT 2021
** babel_server
** File description:
** BabelClient.hpp
*/

#ifndef BABEL_CLIENT
#define BABEL_CLIENT

#include "../tcp/TCP_client.hpp"
#include "../../clientServer/Packet.hpp"
#include "user.hpp"
#include <signal.h>
#include "call.hpp"

static int caugh_sigint = 0;

class BabelClient {
    public:
        BabelClient(std::string &tcpIp, std::size_t portTCP, std::size_t portUdp);
        ~BabelClient() = default;;

        void handleResponse(std::string buffer);
        void handleLoginResponse(BabelPacket &packet);
        void handleSomeoneCalling(BabelPacket &packet);
        void handleLaunchCall(BabelPacket &recBin);
        void handleHangUp(BabelPacket &recBin);
        void handleBasicResponse(BabelPacket &recBin);

        void acceptDenyCall(bool request, std::size_t fromId);
        void login(std::string &username);
        void logout(bool request, std::size_t fromId);
        void sendCallRequest(std::size_t destId);
        void sendHangUp();

        void shell();
        void run();
        void handler();
        void stop();

        std::string getWhereAmI();
        void setWhereAmI(std::string &where);

        std::size_t getCallerId();
        std::size_t getId();
    private:
        asio::io_context _io_context;
        //std::thread * thread_;
        tcp_client *_clientTCP;
        User *_user;
        bool _start;
        std::size_t _portUdp;
        std::size_t _callerId;
        Call *_call;
        std::thread *_callThread;
        std::thread *threadHandler_;
        std::string whereAmI;
};

#endif /* BABEL_CLIENT_HPP_ */
