/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-nora.ounoughi
** File description:
** user
*/

#include "user.hpp"

User::User() 
{
    isIncall = false;
}

User::User(int id, std::string &_pseudo, std::string &passw, int _port, std::string &_birth) 
{
    ID = id;
    pseudo = _pseudo;
    password = passw;
    Port = _port;
    connected = false;
    birth = _birth;
}

User::~User() 
{
}

int User::getID() 
{
    return ID;
}

int User::getPort() 
{
    return Port;
}

std::string User::getPassword() 
{
    return password;
}

std::string User::getPseudo() 
{
    return pseudo;
}

std::string User::getBirth() 
{
    return birth;
}

std::string User::getNumber() 
{
    return number;
}

void User::setPasswd(const std::string &pass) 
{
    password = pass;
}

void User::setNumber(const std::string &nb) 
{
    number = nb;
}

void User::setBirth(const std::string &brth) 
{
    birth = brth;
}

void User::setPseudo(const std::string &pse) 
{
    pseudo = pse;
}

bool User::checkPasswd(const std::string &pas) 
{
    if (password.compare(pas))
        return true;
    else
        return false;
}

void User::setConnected(const std::size_t &id)
{
    connected = true;
    ID = id;
}

void User::setDisconnected()
{
    connected = false;
    ID = -1;
}

bool User::isConnected()
{
    return connected;
}

bool User::getIsInCall()
{
    return isIncall;
}

void User::setIsInCall(const bool &s)
{
    isIncall = s;
}
