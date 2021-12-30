/*
** EPITECH PROJECT 2021
** babel
** File description:
** Packet.hpp
*/

#ifndef PACKET
#define PACKET

#include <iostream>
#include <map>
#include <cstdint>
#include <cstring>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #include <ws2tcpip.h>
    #include <winsock2.h>

    #define NTOHLL(x) (ntohll(x))
    #define PACKED(__DECLARATION__) __pragma(pack(push, 1)) __DECLARATION__ __pragma(pack(pop))
#else
    #include <sys/types.h>
    #include <netinet/in.h>

    #define NTOHLL(x) ((1==ntohl(1)) ? (x) : (((uint64_t)ntohl((x) & 0xFFFFFFFFUL)) << 32) | ntohl((uint32_t)((x) >> 32)))

    #define PACKED(__DECLARATION__) __DECLARATION__ __attribute__((packed))
#endif

class BabelPacket {
    public:
        BabelPacket();
        ~BabelPacket();

        enum BabelOpCode : uint16_t {
            UserWhantToLogin = 200,
            ServerLoginResponse = 201,
            UserWhantToCall = 203,
            AcceptToLogout = 204,
            ServerResponse = 100,
            ServerCallRequest = 101,
            UserAcceptCall = 102,
            LaunchCall = 103,
            UserHangUp = 104,
        };

        enum {
            max_length = 4096
        };

        size_t getSize();
        const void *getData();
        uint16_t getOpCode(const void *data);

        void deserialize(const void *data);
        void clear();
        bool isValid();

        void serialize100(bool &response, const std::string &msg);
        void serialize200(std::string &username, std::size_t &port);
        void serialize201(bool &response, const std::string &msg, std::size_t &userId);
        void serialize203(std::size_t &fromId, std::size_t &destId);
        void serialize101(std::size_t &fromId, std::size_t &destId);
        void serialize104(std::size_t &fromId, std::size_t &destId);
        void serialize102(bool &response, std::size_t &destId);
        void serialize204(bool &response, std::size_t &destId);
        void serialize103(std::string &ip, std::size_t &port, std::size_t &fromId, std::size_t &destId);

        void deserialize104(const void * data);
        void deserialize102(const void *data);
        void deserialize204(const void *data);
        void deserialize100(const void *data);
        void deserialize201(const void *data);
        void deserialize101(const void *data);
        void deserialize200(const void *data);
        void deserialize103(const void *data);
        void deserialize203(const void *data);

        void extractUsername(std::string &username, std::size_t &port);
        void extract204(bool &response, std::size_t &destId);
        void extract100(bool &response, std::string &msg);
        void extractCallRequest(std::size_t &fromId, std::size_t &destId);
        void extract102(bool &response, std::size_t &destId);
        void extractCallInfos(std::size_t &fromId, std::size_t &destId);
        void extractHangUp(std::size_t &fromId, std::size_t &destId);
        void extract201(bool &response, std::string &msg, std::size_t &userId);
        void extract103(std::size_t &fromId, std::size_t &destId, std::string &ip, std::size_t &port);


    private:
        // struct __attribute__ ((__packed__)) ==> limit the size of the struct
        // |***DATA****|***|**DATA**| no packed
        // |DATA|DATA|**************| packed
        union binaryPacket{
            PACKED(struct {
                // the type of packed
                uint16_t type;
                // union is use to define multiple type of data in one struct
                // here it is to define multiple packets
                union {
                    PACKED(struct {
                        uint64_t port;
                        uint64_t usernameLength;
                        char username[max_length - sizeof(uint16_t) - sizeof(uint64_t) - sizeof(uint64_t)];
                    }) _200;

                    PACKED(struct {
                        uint8_t response;
                        uint64_t userId;
                        uint64_t msgLength;
                        char msg[max_length - sizeof(uint16_t) - sizeof(uint8_t) - sizeof(uint64_t) - sizeof(uint64_t)];
                    }) _201;

                    PACKED(struct {
                        uint64_t fromId;
                        uint64_t destId;
                    }) _203;

                    PACKED(struct {
                        uint8_t response;
                        uint64_t destId;
                    }) _204;

                    PACKED(struct {
                        uint8_t response;
                        uint64_t msgLength;
                        char msg[max_length - sizeof(uint16_t) - sizeof(uint8_t) - sizeof(uint64_t)];
                    }) _100;

                    PACKED(struct {
                        uint64_t fromId;
                        uint64_t destId;
                    }) _101;

                    PACKED(struct {
                        uint8_t response;
                        uint64_t destId;
                    }) _102;

                    PACKED(struct {
                        uint64_t fromId;
                        uint64_t destId;
                        uint64_t portDest;
                        uint64_t ipLength;
                        char ip[max_length - sizeof(uint16_t) - sizeof(uint64_t) - sizeof(uint64_t) - sizeof(uint64_t) - sizeof(uint64_t)];
                    }) _103;

                    PACKED(struct {
                        uint64_t fromId;
                        uint64_t destId;
                    }) _104;
                } data;
            }) packed;
            unsigned char byteArray[max_length];
        };
        bool _valid;
        std::size_t _packetSize;
        binaryPacket _data;
};

#endif /* PACKET_HPP_ */
