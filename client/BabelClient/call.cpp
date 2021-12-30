/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-nora.ounoughi
** File description:
** call.cpp
*/

#include "call.hpp"

Call::Call(const std::string &ip, std::size_t portS, std::size_t portR) : _ip(ip), _portSender(portS), _portReceive(portR)
{
    _sender = new SenderUdp(ip, portS);
    _receiver = std::shared_ptr<ReceiverUdp> (new ReceiverUdp(portR));
    _streamHandler = new StreamHandler();
    _streamEncoder = new StreamEncoder(SAMPLE_RATE, NUM_CHANNELS);
}

bool Call::getIsActive()
{
    return isActive;
}

void Call::launchCall()
{
    isActive = true;
    _sender->open();
    _receiver->start_receive();

    _streamHandler->startStream();

    while (isActive) {
        if (_receiver->getHaveData()) {
            /// RECEIVE UDP have data Encoded taille bytes_transferred
            auto streamReceive = _receiver->getBuffer();
            std::size_t bytes_transferred = _receiver->getDataSize();
            //std::cout << "Received: '" << std::string(recv_buffer.begin(), recv_buffer.begin()+bytes_transferred) << "'" << std::endl;
            
            /// DECODE OPUS
            _streamEncoder->setToDecode(streamReceive, bytes_transferred);
            _streamEncoder->decode();

            /// POUR LIRE AUDIO
            _streamHandler->setDecoded(_streamEncoder->getDecoded());
            _streamHandler->writeStream();
        }
        /// ENREGISTREMENT VOIX AUDIO
        _streamHandler->readStream();
        
        /// ENCODE OPUS
        _streamEncoder->setCaptured(_streamHandler->getCatpured());
        _streamEncoder->encode();

        /// SEND UDP Encoded taille EncBytes
        _sender->sendMessage(_streamEncoder->getEncoded(), _streamEncoder->getEncBytes());
    }
}

void Call::hangUpCall()
{
    isActive = false;
    _receiver->stop_receive();
    _sender->close();
    delete (_sender);
    std::cout << "Call is ending" << std::endl;
}