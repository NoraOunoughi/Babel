/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-nora.ounoughi
** File description:
** StreamEncoder
*/

#include "StreamEncoder.hpp"

StreamEncoder::StreamEncoder(uint32_t sampleRate, uint32_t channel) : encoded(FRAME_SIZE * channel * 2),
               captured(FRAME_SIZE * channel),
               decoded(FRAME_SIZE * channel)
{
    encoder_ = nullptr;
    decoder_ = nullptr;
    encBytes = 0;
    int opusError;

    encoder_ = opus_encoder_create(sampleRate, channel, OPUS_APPLICATION_AUDIO, &opusError);
    if (opusError != OPUS_OK)
        OpusErr();
    decoder_ = opus_decoder_create(sampleRate, channel, &opusError);
    if (opusError != OPUS_OK)
        OpusErr();
}

StreamEncoder::~StreamEncoder()
{
    if (encoder_)
        opus_encoder_destroy(encoder_);
    if (decoder_)
        opus_decoder_destroy(decoder_);
}

void StreamEncoder::encode()
{
    // std::vector<uint16_t> encoded(record_sample.size());
    // int nbbytes = opus_encode(encoder_, (opus_int16 const*)record_sample.data(),
    //                             (int)record_sample.size(), (unsigned char *)(encoded.data()),
    //                             (int)encoded.size());
    // if (nbbytes < 0)
    //     OpusErr();
    // return encoded;
    encBytes = opus_encode(encoder_, reinterpret_cast<opus_int16 const *>(captured.data()), FRAME_SIZE, encoded.data(), static_cast<opus_int32>(encoded.size()));
    if (encBytes < 0)
        OpusErr();
}

void StreamEncoder::decode()
{
    // std::vector<uint16_t> decoded(encoded.size());
    // int nbbytes = opus_decode(decoder_, (unsigned char *)encoded.data(),
    //                             (int)encoded.size(), (opus_int16 *)decoded.data(), (int)encoded.size(), 0);
    // if (nbbytes < 0)
    //     OpusErr();
    // return decoded;
    if (opus_decode(decoder_, toDecode.data(), static_cast<opus_int32>(toDecBytes), reinterpret_cast<opus_int16 *>(decoded.data()), FRAME_SIZE, 0) < 0)
        OpusErr();
}

void StreamEncoder::OpusErr()
{
    throw Exception("Opus Error", "Error occured");
}

std::vector<unsigned char> StreamEncoder::getEncoded() const
{
    return this->encoded;
}

void StreamEncoder::setToDecode(const std::vector<unsigned char> &inToDecode, const size_t &encBytesFromUDP)
{
    this->toDecode = inToDecode;
    this->toDecBytes = encBytesFromUDP;
}

void StreamEncoder::setCaptured(const std::vector<unsigned short> &inCaptured)
{
    this->captured = inCaptured;
}

std::vector<unsigned short> StreamEncoder::getDecoded() const
{
    return this->decoded;
}

size_t StreamEncoder::getEncBytes() const
{
    return this->encBytes;
}