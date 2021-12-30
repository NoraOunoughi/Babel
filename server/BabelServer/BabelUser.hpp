/*
** EPITECH PROJECT 2021
** babel_server
** File description:
** BabelUser.hpp
*/

#ifndef BABEL_USER
#define BABEL_USER

#include "../Network/Network.hpp"

class BabelUser {
public:
    BabelUser(std::size_t id, std::shared_ptr<session> sessionPtr);
    ~BabelUser();

    std::shared_ptr<session> getSession();
    std::size_t getId();
    bool isConnected();
    void setUdpPort(std::size_t &port);
    std::size_t getUdpPort();
    void connected();
    void disconnected();

private:
    std::size_t _portUdp;
    std::size_t _id;
    std::shared_ptr<session> _sessionTCP;
    bool _connected;
};

#endif /* BABEL_USER_HPP_ */
