/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-nora.ounoughi
** File description:
** session
*/

#ifndef SESSION_HPP_
#define SESSION_HPP_

#include <iostream>
#include <asio.hpp>
#include <map>
#include "../clientServer/Packet.hpp"

class session : public std::enable_shared_from_this<session>
{
    public:
        typedef std::shared_ptr<session> pointer;

        static pointer create(asio::io_service& io_service)
        {
            return pointer(new session(io_service));
        }

        void start();
        asio::ip::tcp::socket& socket();
        char *getData();
        void clearData();
        void sendMessage(BabelPacket &packet);
        const bool hasDataValid();
        const std::size_t getDataSize();
        void close();
        bool isClosed();
        std::string getIp();

    private:
        session(asio::io_service& io_service);
        void do_write(char *message, std::size_t messageSize);
        void handle_write(const std::error_code& error, std::size_t bytes_transferred);
        void do_read();
        void handle_read(const std::error_code& error, std::size_t bytes_transferred);

        asio::ip::tcp::socket m_socket;
        std::string m_message;
        enum { max_length = 4096 };
        char m_data[max_length];
        std::size_t m_dataSize = 0;
        bool m_dataValid = false;
        bool _closed;
        std::string _ip;
};

#endif /* !SESSION_HPP_ */
