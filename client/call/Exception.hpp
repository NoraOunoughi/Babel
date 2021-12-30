/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-nora.ounoughi
** File description:
** Exception
*/

#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include <exception>
#include <string>

class Exception : public std::exception{
    public:
        Exception(const std::string &ctx, const std::string &msg);
        virtual const char* what() const throw();
        virtual ~Exception() throw();
    private:
        std::string _msg;
};

#endif /* !EXCEPTION_HPP_ */