/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-nora.ounoughi
** File description:
** ClientSession
*/

#ifndef CLIENTSESSION_HPP_
#define CLIENTSESSION_HPP_

#include <iostream>
#include <asio.hpp>
#include <chrono>
#include <thread>
#include "../../clientServer/Packet.hpp"

class ClientSession : public std::enable_shared_from_this<ClientSession>
{
    public:
        // Shared ptr abreviation
        typedef std::shared_ptr<ClientSession> pointer;

        // Create shared
        static ClientSession::pointer create(asio::io_context& io_context)
        {
            return pointer(new ClientSession(io_context));
        }

        // Handle the ClientSession
        void start();

        // Return socket
        asio::ip::tcp::socket& socket();

        void sendMessage(BabelPacket &packet);

        std::string getData();

        std::string getDataClear();

        void clearData();

    private:
        // Constructor
        ClientSession(asio::io_context& io_context);

        void do_writePacket(BabelPacket &toSend);

        // Function call when do_write finish
        void handle_write(const std::error_code& error, std::size_t bytes_transferred);

        // Read
        void do_read();

        // Function call when do_write finish
        void handle_read(const std::error_code& error, std::size_t bytes_transferred);

        asio::ip::tcp::socket m_socket;
        enum { max_length = 4096 };
        char m_data[max_length];
        std::size_t m_dataSize = 0;
};

#endif /* !CLIENTSESSION_HPP_ */
