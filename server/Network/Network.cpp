/*
** EPITECH PROJECT 2021
** babel_server
** File description:
** Network.cpp
*/

#include "Network.hpp"

Network::Network() : _io_context(), _tcpServers() {
    std::cout << "Created newtork" << std::endl;
}

Network::~Network()
{
    for (auto &ptr : _tcpServers) delete(ptr);
    std::cout << "Destroy network" << std::endl;
}

tcp_server *Network::createServer(std::size_t port)
{
    tcp_server *srv = new tcp_server(_io_context, port);
    _tcpServers.push_back(srv);
    std::cout << "Server TCP created" << std::endl;
    return srv;
}

void Network::destroyServer(tcp_server *server)
{
    auto it = std::find(_tcpServers.begin(), _tcpServers.end(), server);
    if (it != _tcpServers.end()) {
        _tcpServers.erase(it);
        delete server;
    }
}