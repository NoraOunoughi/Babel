/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-nora.ounoughi
** File description:
** StreamEncoder
*/

#ifndef STREAMENCODER_HPP_
#define STREAMENCODER_HPP_

#include <opus/opus.h>
#include <string>
#include <vector>
#include "Exception.hpp"

/* #define SAMPLE_RATE  () // Test failure to open with this value. */
#define FRAMES_PER_BUFFER (512)
#define PA_SAMPLE_TYPE  paInt16
#define FRAME_SIZE (120)

class StreamEncoder {
    public:
        StreamEncoder(uint32_t sampleRate, uint32_t channel);
        ~StreamEncoder();

        void encode();
        void decode();

        void setCaptured(const std::vector<unsigned short> &);
        void setToDecode(const std::vector<unsigned char> &, const size_t &);
        std::vector<unsigned char> getEncoded() const;
        std::vector<unsigned short> getDecoded() const;
        size_t getEncBytes() const;

        void OpusErr();

    private:
        std::vector<unsigned char> encoded;
        std::vector<unsigned short> captured;
        std::vector<unsigned short> decoded;
        std::vector<unsigned char> toDecode;
        OpusEncoder *encoder_;
        OpusDecoder *decoder_;
        size_t encBytes;
        size_t toDecBytes;
};

#endif /* !STREAMENCODER_HPP_ */
