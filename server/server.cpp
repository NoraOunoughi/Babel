/*
** EPITECH PROJECT 2021
** babel_server
** File description:
** server.cpp
*/

#include "BabelServer/BabelServer.hpp"

int main(int ac, char **av)
{
    std::size_t portServer = 5000;

    try {
        if (ac != 2) {
            std::cerr << "Usage ./babelserver [portServerTCP]" << std::endl;
            std::cerr << "Hosting PC: ./babel_server 5000" << std::endl;
            return 84;
        }
        portServer = std::atoi(av[1]);
        BabelServer server(portServer);
        server.run();
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}