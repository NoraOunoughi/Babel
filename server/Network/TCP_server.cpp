/*
** EPITECH PROJECT 2021
** babel_server
** File description:
** TCP_server.cpp
*/

#include "TCP_server.hpp"

tcp_server::tcp_server(asio::io_service& io_service, int port) :
    io_service_(io_service),
    m_acceptor(io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
    _max_id(0),
    _clientUserMap()
{
}

tcp_server::~tcp_server()
{
}

void tcp_server::launch()
{
    start_accept();
    thread_ = new(std::nothrow) std::thread([&] { io_service_.run(); });
}

void tcp_server::stop()
{
    io_service_.stop();
    thread_->join();
    delete(thread_);
}

void tcp_server::start_accept()
{
    session::pointer new_connection = session::create(io_service_);
    m_acceptor.async_accept(
        new_connection->socket(),
        std::bind(&tcp_server::handle_accept, this, new_connection, std::placeholders::_1)
    );
}

void tcp_server::handle_accept(session::pointer client_session, std::error_code error)
{
    if (!error) {
        std::cout << "New client connected" << std::endl;
        client_session->start();
        _max_id++;
        _clientUserMap.emplace(_max_id, client_session);
        _newIds.push_back(_max_id);
        start_accept();
    } else {
        std::cerr << "Handle accept error: " << error.message() << std::endl;
    }
}

std::vector<std::size_t> tcp_server::getAllNewIds()
{
    return _newIds;
}

void tcp_server::clearNewIds()
{
    _newIds.clear();
}

std::shared_ptr<session> tcp_server::getClientById(std::size_t &id)
{
    for (auto it = _clientUserMap.begin(); it != _clientUserMap.end(); ++it) {
        if (it->first == id)
            return it->second;
    }
    return nullptr;
}

void tcp_server::handleClientDisconnect(std::size_t &id)
{
    auto cliSess = _clientUserMap.find(id);
    if (cliSess != _clientUserMap.end()) {
        _clientUserMap.erase(cliSess);
    }
}