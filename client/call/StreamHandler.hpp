/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-nora.ounoughi
** File description:
** StreamHandler
*/

#ifndef STREAMHANDLER_HPP_
#define STREAMHANDLER_HPP_

#include <string>
#include <portaudio.h>
#include "Exception.hpp"
#include <iostream>
#include <vector>

/* #define SAMPLE_RATE  () // Test failure to open with this value. */
#define SAMPLE_RATE  (48000)
#define FRAMES_PER_BUFFER (512)
#define NUM_CHANNELS    (2)
#define PA_SAMPLE_TYPE  paInt16
#define FRAME_SIZE (120)

class StreamHandler {
    public:
        StreamHandler();
        ~StreamHandler();

        void startStream();
        void closeStream();

        void PortAudioErr(PaError Paerr);


        // std::vector<uint16_t> readStream();
        void readStream();
        // void writeStream(std::vector<uint16_t> record);
        void writeStream();
        void restart_all();

        void initInputParameters();
        void initOutputParameters();

        void setDecoded(const std::vector<uint16_t> &newDecoded);
        std::vector<uint16_t> getCatpured();

    private:
        PaStream *_Instream;
        PaStream *_Outstream;
        PaStreamParameters inputParameters;
        PaStreamParameters outputParameters;
        const PaDeviceInfo* inDeviceInfo;
        const PaDeviceInfo* outDeviceInfo;
        std::size_t inChannel;
        std::size_t outChannel;
        std::vector<uint16_t> _captured;
        std::vector<uint16_t> _decoded;
};

#endif /* !STREAMHANDLER_HPP_ */
