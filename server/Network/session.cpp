/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-nora.ounoughi
** File description:
** session
*/

#include "session.hpp"

session::session(asio::io_service& io_service) : m_socket(io_service), _closed(false)
{
}

void session::start()
{
    _ip = m_socket.remote_endpoint().address().to_string();
    do_read();
}

asio::ip::tcp::socket& session::socket()
{
    return m_socket;
}

void session::sendMessage(BabelPacket &packet)
{
    char tmp[4096];
    memcpy(tmp, packet.getData(), packet.getSize());
    do_write(tmp, packet.getSize());
}

void session::do_write(char *message, std::size_t messageSize)
{
    auto self(shared_from_this());
    auto myBoostBind =
    [this, self](std::error_code ec, std::size_t length) {
        handle_write(ec, length);
    };
    asio::async_write(m_socket,
    asio::buffer(message, messageSize),
    myBoostBind
    );
}

void session::handle_write(const std::error_code& error, std::size_t bytes_transferred)
{
    if (!error) {
        std::cout << "Async_write Success! " << bytes_transferred << " Bytes transferred" << std::endl;
        //do_read();
    } else {
        std::cerr <<"Async_write error: " << error.message() << std::endl;
    }
}

void session::do_read()
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

void session::handle_read(const std::error_code& error, std::size_t bytes_transferred)
{
    if (!error) {
        std::cout << "Async_read_some Success!" << std::endl;
        m_dataSize = bytes_transferred;
        m_dataValid = true;
    } else if (error.message() == "End of file") {
        std::cerr << "Client disconnect" << std::endl;
        close();
        return;
    } else {
        std::cerr << "Async_read_some error: " << error.message() << std::endl;
    }
    do_read();
}

char *session::getData()
{
    return m_data;
}

void session::clearData()
{
    m_dataSize = 0;
    m_dataValid = false;
    memset(m_data, '\0', sizeof(m_data));
}

const bool session::hasDataValid()
{
    return m_dataValid;
}

const std::size_t session::getDataSize()
{
    return m_dataSize;

}

void session::close()
{
    _closed = true;
}

bool session::isClosed()
{
    return _closed;
}

std::string session::getIp()
{
    return _ip;
}