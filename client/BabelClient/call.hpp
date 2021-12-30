/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-nora.ounoughi
** File description:
** call
*/

#ifndef CALL_HPP_
#define CALL_HPP_

#include "../udp/SenderUdp.hpp"
#include "../udp/ReceiverUdp.hpp"
#include "../call/StreamHandler.hpp"
#include "../call/StreamEncoder.hpp"

class Call {
    public:
        Call(const std::string &ip, std::size_t portS, std::size_t portR);
        ~Call() = default;
        bool getIsActive();
        void launchCall();
        void hangUpCall();

    private:
        SenderUdp *_sender;
        std::shared_ptr<ReceiverUdp> _receiver;
        std::string _ip;
        std::size_t _portReceive;
        std::size_t _portSender;
        bool isActive;
        StreamHandler *_streamHandler;
        StreamEncoder *_streamEncoder;
};

#endif /* !CALL_HPP_ */
