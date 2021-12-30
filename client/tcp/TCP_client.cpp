/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-nora.ounoughi
** File description:
** TCP_client
*/

#include "TCP_client.hpp"

tcp_client::tcp_client(std::string &ip, int port, asio::io_context& io_context) :
    io_context_(io_context)
{
    ipServerTCP = ip;
    portServerTCP = port;
    //start_connect();
}

void tcp_client::launch()
{
    start_connect();
    thread_ = new std::thread([&] { io_context_.run(); });
}

void tcp_client::sendMessageToServer(BabelPacket &packet)
{
    new_connection->sendMessage(packet);
}

ClientSession::pointer tcp_client::getClientSession()
{
    return new_connection;
}
    
// Async accept
void tcp_client::start_connect()
{
    new_connection = ClientSession::create(io_context_);
    new_connection->socket().async_connect(asio::ip::tcp::endpoint(asio::ip::address::from_string(ipServerTCP), portServerTCP),
        std::bind(&tcp_client::handle_connect, this, new_connection, std::placeholders::_1)
    );
}

// This function call when async_accept success
// When a client connect
void tcp_client::handle_connect(ClientSession::pointer clientSession, std::error_code error)
{
    if (!error) {
        //std::cout << "Connected" << std::endl;
        //clientSession->start();
        //_clientSession.get()->start();
        new_connection->start();
    } else {
        std::cerr << "Handle accept error: " << error.message() << std::endl;
    }
}