/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-nora.ounoughi
** File description:
** Exception
*/

#include "Exception.hpp"

Exception::Exception(const std::string &ctx, const std::string &msg)
{
    _msg = ctx + " : " + msg;
}

Exception::~Exception() throw()
{
}

const char* Exception::what() const throw()
{
    return _msg.c_str();
}