/*
** EPITECH PROJECT 2021
** babel_server
** File description:
** Network.hpp
*/

#ifndef NETWORK_HPP_
#define NETWORK_HPP_

#include <iostream>
#include <asio.hpp>
#include <map>
#include "TCP_server.hpp"

class Network {
    public:
        Network();
        ~Network();

        tcp_server *createServer(std::size_t port);
        void destroyServer(tcp_server *server);

    private:
        static void networkingThreadFunc(Network *);

        asio::io_context _io_context;
        std::vector<tcp_server *> _tcpServers;
};

#endif /* NETWORK_HPP_ */