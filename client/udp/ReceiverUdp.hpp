/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-nora.ounoughi
** File description:
** ReceiverUdp
*/

#ifndef RECEIVER_UDP_HPP_
#define RECEIVER_UDP_HPP_

#include <asio.hpp>
#include <thread>
#include <iostream>
#include <utility>
#include <memory>

class ReceiverUdp : public std::enable_shared_from_this<ReceiverUdp> {
public:
    ReceiverUdp(std::size_t port);
    ~ReceiverUdp();

    void handle_receive(const std::error_code& error, size_t bytes_transferred);

    void wait();
    void start_receive();
    void stop_receive();

    std::vector<unsigned char> getBuffer();
    bool getHaveData();
    std::size_t getDataSize();

private:
    std::thread *thread;
    std::size_t port;
    asio::io_service io_service;
    asio::ip::udp::socket socket{io_service};
    std::vector<unsigned char> recv_buffer;
    asio::ip::udp::endpoint remote_endpoint;
    bool haveData;
    std::size_t dataSize;
};

#endif /* !RECEIVER_UDP_HPP_ */