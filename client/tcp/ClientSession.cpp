/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-nora.ounoughi
** File description:
** ClientSession
*/

#include "ClientSession.hpp"

// Handle the ClientSession
void ClientSession::start()
{
    //do_write();
    do_read();
}

// Return socket
asio::ip::tcp::socket& ClientSession::socket()
{
    return m_socket;
}

void ClientSession::sendMessage(BabelPacket &packet)
{
    do_writePacket(packet);
}

std::string ClientSession::getData()
{
    std::string tmp(m_data, m_dataSize);
    return tmp;
}

std::string ClientSession::getDataClear()
{
    std::string tmp = getData();
    clearData();
    return tmp;
}

void ClientSession::clearData()
{
    m_dataSize = 0;
    memset(m_data, 0, max_length);
}

// Constructor
ClientSession::ClientSession(asio::io_context& io_context) : m_socket(io_context)
{
}

void ClientSession::do_writePacket(BabelPacket &toSend)
{
    auto self(shared_from_this());
    char request[max_length];
    std::size_t request_length;
    auto myBoostBind =
    [this, self](std::error_code ec, std::size_t length) {
        handle_write(ec, length);
    };

    memcpy(request, toSend.getData(), toSend.getSize());
    request_length = toSend.getSize();
    
    asio::async_write(m_socket,
        asio::buffer(request, request_length),
        myBoostBind
    );
    memset(request, 0, max_length);
}

// Function call when do_write finish
void ClientSession::handle_write(const std::error_code& error, std::size_t bytes_transferred)
{
    if (!error) {
        std::cout << "Async_write Success! " << bytes_transferred << " Bytes transferred" << std::endl;
    } else {
        std::cerr <<"Async_write error: " << error.message() << std::endl;
    }
}

// Read
void ClientSession::do_read()
{
    auto self(shared_from_this());
    auto myBoostBind =
    [this, self](std::error_code ec, std::size_t length) {
        handle_read(ec, length);
    };

    m_socket.async_read_some(asio::buffer(m_data, max_length),
        myBoostBind
    );
}

// Function call when do_write finish
void ClientSession::handle_read(const std::error_code& error, std::size_t bytes_transferred)
{
    if (!error) {
        std::cout << "Async_read_some Success!" << std::endl;
        m_dataSize = bytes_transferred;
        do_read();
    } else {
        std::cerr <<"Async_read_some error: " << error.message() << std::endl;
    }
}
