/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-nora.ounoughi
** File description:
** user
*/

#ifndef USER_HPP_
#define USER_HPP_

#include <fstream>
#include <iostream>

class User {
    public:
        User();
        User(int, std::string&, std::string&, int, std::string &);
        ~User();
        bool checkPasswd(const std::string&);
        int getID();
        int getPort();
        std::string getPseudo();
        std::string getPassword();
        std::string getBirth();
        std::string getNumber();
        bool isConnected();
        void setPseudo(const std::string&);
        void setPasswd(const std::string&);
        void setBirth(const std::string&);
        void setNumber(const std::string&);
        void setConnected(const std::size_t&);
        void setDisconnected();
        void setIsInCall(const bool &);
        bool getIsInCall();

    private:
        int ID;
        std::string pseudo;
        std::string password;
        std::string birth;
        std::string number;
        int Port;
        bool connected;
        bool isIncall;
};

#endif /* !USER_HPP_ */
