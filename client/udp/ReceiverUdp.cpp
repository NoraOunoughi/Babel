/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-nora.ounoughi
** File description:
** ReceiverUdp
*/

#include "ReceiverUdp.hpp"

ReceiverUdp::ReceiverUdp(std::size_t port) : port(port)
{
    haveData = false;
}

ReceiverUdp::~ReceiverUdp() {}

void ReceiverUdp::handle_receive(const std::error_code& error, size_t bytes_transferred)
{
    if (error) {
        std::cout << "Receive failed: " << error.message() << "\n";
        return;
    }
    haveData = true;
    dataSize = bytes_transferred;
    wait();
}

void ReceiverUdp::wait()
{
    auto self(shared_from_this());
    auto myBoostBind =
    [this, self](std::error_code ec, std::size_t length) {
        handle_receive(ec, length);
    };
    socket.async_receive_from(asio::buffer(recv_buffer),
        remote_endpoint,
        myBoostBind
    );
}

void ReceiverUdp::start_receive()
{
    std::cout << "Start receive" << std::endl;
    socket.open(asio::ip::udp::v4());
    socket.bind(asio::ip::udp::endpoint(asio::ip::address::from_string("0.0.0.0"), port));

    wait();

    std::cout << "Receiving\n";
    thread = new(std::nothrow) std::thread([&] { io_service.run(); });
    std::cout << "Receiver exit\n";
}

void ReceiverUdp::stop_receive()
{
    io_service.stop();
    socket.close();

}

std::vector<unsigned char> ReceiverUdp::getBuffer()
{
    return recv_buffer;
}

bool ReceiverUdp::getHaveData()
{
    return haveData;
}
    
std::size_t ReceiverUdp::getDataSize()
{
    return dataSize;
}