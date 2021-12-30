/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-nora.ounoughi
** File description:
** TCP_client
*/

#ifndef TCP_CLIENT_HPP_
#define TCP_CLIENT_HPP_

#include "ClientSession.hpp"

class tcp_client
{
    public:
        // Constructor
        tcp_client(std::string &ip, int port, asio::io_context& io_context);

        void launch();

        void sendMessageToServer(BabelPacket &packet);

        ClientSession::pointer getClientSession();

    private:
        // Async accept
        void start_connect();

        // This function call when async_accept success
        // When a client connect
        void handle_connect(ClientSession::pointer clientSession, std::error_code error);

        ClientSession::pointer new_connection;
        asio::io_context& io_context_;
        std::thread *thread_;
        std::size_t portServerTCP;
        std::string ipServerTCP;
};

#endif /* !TCP_CLIENT_HPP_ */