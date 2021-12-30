/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-nora.ounoughi
** File description:
** BabelUser
*/

#include "BabelUser.hpp"

BabelUser::BabelUser(std::size_t id, std::shared_ptr<session> sessionPtr) : _id(id), _sessionTCP(sessionPtr), _connected(false)
{
}

BabelUser::~BabelUser()
{
}

std::shared_ptr<session> BabelUser::getSession()
{
    return _sessionTCP;
}

std::size_t BabelUser::getId()
{
    return _id;
}

bool BabelUser::isConnected()
{
    return _connected;
}

void BabelUser::setUdpPort(std::size_t &port)
{
    _portUdp = port;
}

std::size_t BabelUser::getUdpPort()
{
    return _portUdp;
}

void BabelUser::connected()
{
    _connected = true;
}

void BabelUser::disconnected()
{
    _connected = false;
}