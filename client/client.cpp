/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-nora.ounoughi
** File description:
** client.cpp
*/

#include <QtWidgets>
#include "Qt_interface/mainwindow.h"
#include <QApplication>

int main(int argc, char **argv)
{
    try {
        if (argc != 3) {
            std::cerr << "Usage ./babel_client [ipServerTCP] [portServerTCP]" << std::endl;
            std::cerr << "Hosting PC: ./babel_server" << std::endl;
            std::cerr << "Hosting PC: ./babel_client 127.0.0.1 5000" << std::endl;
            std::cerr << "Other PC: ./babel_client [HostingPc ip] 5000" << std::endl;
            return 84;
        }
        std::srand(std::time(nullptr));
        std::size_t portUdp = (std::rand() % 1000) + 5000;
        std::string ipServerTCP;
        std::size_t portServerTCP;
        ipServerTCP = std::string(argv[1]);
        portServerTCP = std::atoi(argv[2]);
        QApplication a(argc, argv);
        MainWindow w(ipServerTCP, portServerTCP, portUdp);
        w.show();
        a.exec();
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
