/*
** EPITECH PROJECT 2021
** babel_server
** File description:
** TCP_server.hpp
*/

#ifndef TCP_SERVER
#define TCP_SERVER

#include "session.hpp"

class tcp_server
{
    public:
        tcp_server(asio::io_service& io_service, int port);
        ~tcp_server();
        void launch();
        void stop();
        void handleClientDisconnect(std::size_t &);
        std::vector<std::size_t> getAllNewIds();
        void clearNewIds();
        std::shared_ptr<session> getClientById(std::size_t &);
        std::map<int, std::shared_ptr<session>> _clientUserMap;
    
    private:
        void start_accept();
        void handle_accept(session::pointer client_session, std::error_code error);

        asio::io_service& io_service_;
        asio::ip::tcp::acceptor m_acceptor;
        std::size_t _max_id;
        std::thread *thread_;
        std::vector<std::size_t> _newIds;
};

#endif /* TCP_SERVER_HPP_ */