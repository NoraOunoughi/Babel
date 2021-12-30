/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-nora.ounoughi
** File description:
** SenderUdp
*/

#ifndef SENDER_UDP_HPP__
#define SENDER_UDP_HPP_

#include <asio.hpp>
#include <thread>
#include <iostream>

class SenderUdp {
    public:
        SenderUdp(const std::string &ip, std::size_t port) : _ip(ip), _port(port) {}
        ~SenderUdp() {};

        void open();
        void close();
        void sendMessage(std::vector<unsigned char> in, size_t frameSize);

    private:
        asio::io_context io_context;
        asio::ip::udp::socket _socket{io_context};
        asio::ip::udp::endpoint remote_endpoint;
        std::string _ip;
        std::size_t _port;
};

#endif /* !SENDER_UDP_HPP_ */
