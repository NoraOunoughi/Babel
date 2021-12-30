/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-nora.ounoughi
** File description:
** StreamHandler
*/

#include "StreamHandler.hpp"

StreamHandler::StreamHandler() : _captured(FRAME_SIZE * NUM_CHANNELS), _decoded(FRAME_SIZE * NUM_CHANNELS)
{
    _Instream = nullptr;
    _Outstream = nullptr;

    PaError p_err = Pa_Initialize();
    if (p_err != paNoError)
        PortAudioErr(p_err);
}

StreamHandler::~StreamHandler()
{
    PaError p_err;

    closeStream();
    p_err = Pa_Terminate();
    if (p_err != paNoError)
        PortAudioErr(p_err);
}

void StreamHandler::startStream()
{
    initInputParameters();
    /* Record some audio. -------------------------------------------- */
    PaError p_err = Pa_OpenStream(
        &_Instream,
        &inputParameters,
        nullptr,                  /* &outputParameters, */
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        paClipOff,      /* we won't output out of range samples so don't bother clipping them */
        nullptr,
        nullptr);
    if (p_err != paNoError)
        PortAudioErr(p_err);

    initOutputParameters();
    // printf("\n=== Now playing back. ===\n"); fflush(stdout);
    p_err = Pa_OpenStream(
        &_Outstream,
        nullptr, /* no input */
        &outputParameters,
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        paClipOff,      /* we won't output out of range samples so don't bother clipping them */
        nullptr,
        nullptr);
    if (p_err != paNoError)
        PortAudioErr(p_err);

    p_err = Pa_StartStream(_Instream);
    if (p_err != paNoError)
        PortAudioErr(p_err);

    p_err = Pa_StartStream(_Outstream);
    if (p_err != paNoError)
        PortAudioErr(p_err);    
}

void StreamHandler::closeStream()
{
    PaError p_err = Pa_StopStream(_Instream);
    if (p_err != paNoError)
        PortAudioErr(p_err);
    
    p_err = Pa_StopStream(_Outstream);
    if (p_err != paNoError)
        PortAudioErr(p_err);

    p_err = Pa_CloseStream(_Instream);
    if (p_err != paNoError)
        PortAudioErr(p_err);

    p_err = Pa_CloseStream(_Outstream);
    if (p_err != paNoError)
        PortAudioErr(p_err);
}

void StreamHandler::PortAudioErr(PaError Paerr)
{
    throw Exception("PortAdio Error", Pa_GetErrorText(Paerr));
}

// void StreamHandler::writeStream(std::vector<uint16_t> record)
void StreamHandler::writeStream()
{
    PaError paerr;

    // long streamWriteAvailable = Pa_GetStreamWriteAvailable(_Outstream);
    // while (streamWriteAvailable < (long)record.size());
    // paerr = Pa_WriteStream(_Outstream, record.data(), (unsigned long)record.size());

    paerr = Pa_WriteStream(_Outstream, _decoded.data(), FRAME_SIZE);
    if (paerr != paNoError) {
        std::cerr << "Write stream error" << std::endl;
        PortAudioErr(paerr);
        restart_all();
    }

}

// std::vector<uint16_t> StreamHandler::readStream()
void StreamHandler::readStream()
{
    PaError paerr;
    std::vector<uint16_t> record(FRAME_SIZE);
    long streamReadAvailable = Pa_GetStreamReadAvailable(_Instream);

    if (streamReadAvailable < (long)FRAME_SIZE)
        paerr = Pa_ReadStream(_Instream, record.data(), (unsigned long)streamReadAvailable);
    else
        paerr = Pa_ReadStream(_Instream, record.data(), FRAME_SIZE);
    // paerr = Pa_ReadStream(_Instream, _captured.data(), FRAME_SIZE);
    if (paerr != paNoError) {
        std::cerr << "Read stream error" << std::endl;
        PortAudioErr(paerr);
        restart_all();
    }
    // return record;
}

void StreamHandler::restart_all()
{
    closeStream();
    startStream();
}

void StreamHandler::initInputParameters()
{
    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default input device.\n");
        return;
    }
    inDeviceInfo = Pa_GetDeviceInfo(inputParameters.device);
    if (inDeviceInfo == nullptr) {
        std::cerr << "PortAudio (Error) : Get default info" << std::endl;
        return;
    }
    inChannel = (inDeviceInfo->maxOutputChannels < 2 ? inDeviceInfo->maxOutputChannels : 2);
    inputParameters.channelCount = inChannel;                    /* stereo input */
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = nullptr;
}

void StreamHandler::initOutputParameters()
{
    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");
        return;
    }
    outDeviceInfo = Pa_GetDeviceInfo(outputParameters.device);
    if (outDeviceInfo== nullptr) {
        std::cerr << "PortAudio (Error) : Get default info" << std::endl;
        return;
    }
    outChannel = (outDeviceInfo->maxOutputChannels < 2 ? outDeviceInfo->maxOutputChannels : 2);
    outputParameters.channelCount = outChannel;                     /* stereo output */
    outputParameters.sampleFormat =  PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = outDeviceInfo->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = nullptr;
}

void StreamHandler::setDecoded(const std::vector<uint16_t> &newDecoded)
{
    _decoded = newDecoded;
}

std::vector<uint16_t> StreamHandler::getCatpured()
{
    return _captured;
}