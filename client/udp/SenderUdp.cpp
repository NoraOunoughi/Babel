/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-nora.ounoughi
** File description:
** SenderUdp
*/

#include "SenderUdp.hpp"

void SenderUdp::open()
{
    remote_endpoint = asio::ip::udp::endpoint(asio::ip::address::from_string(_ip), _port);
    _socket.open(asio::ip::udp::v4());
}

void SenderUdp::sendMessage(std::vector<unsigned char> in, size_t frameSize)
{
    std::error_code err;
    auto sent = _socket.send_to(asio::buffer(in, frameSize), remote_endpoint, 0, err);
}

void SenderUdp::close()
{
    _socket.close();
}