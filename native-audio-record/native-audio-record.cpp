// Copyright 2008, The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
#define LOG_NDEBUG 0
#define LOG_TAG "native_audio_test"

#include <stdlib.h>
#include <stdio.h>
#include <cutils/properties.h>
#include <media/AudioSystem.h>
#include <media/AudioRecord.h>
#include <math.h>

#include "native-audio-record.h"
#include <binder/MemoryDealer.h>
#include <binder/MemoryHeapBase.h>
#include <binder/MemoryBase.h>
#include <binder/ProcessState.h>


#include <utils/Log.h>

#include <fcntl.h>

#define PACKAGE_NAME  "AudioRecord test"

namespace android {

/************************************************************
*
*    Constructor
*
************************************************************/
AudioRecordTest::AudioRecordTest(void) {

}


/************************************************************
*
*
************************************************************/
void AudioRecordTest::Execute(int sample_rate, int channels, char *filename) {
    FILE *file;
    uint32_t sampleRate;
    audio_format_t format = AUDIO_FORMAT_PCM_16_BIT;
    audio_channel_mask_t channelMask;
    audio_source_t inputSource = AUDIO_SOURCE_MIC;
    size_t frameCount;
    char *buffer;
    size_t bufferSize;
    int writeDataCount = 0; 

    ALOGD("record filename is %s", filename);
    file = fopen(filename, "wb+");
    if (!file) {
        ALOGD( "Unable to open file '%s'\n", filename);
        return;
    }

    sampleRate = sample_rate;
    if (channels == 1)
        channelMask = AUDIO_CHANNEL_IN_MONO;
    else
        channelMask = AUDIO_CHANNEL_IN_STEREO;
    ALOGD("sample_rate = %d, num_channels = 0x%x, bits_per_sample = 0x%x", 
            sampleRate, channelMask, format);

    status_t status = AudioRecord::getMinFrameCount(&frameCount, sampleRate, format, channelMask);    
    if(status != NO_ERROR) {
        ALOGE("%s  AudioRecord.getMinFrameCount fail \n", __FUNCTION__);
        return;
    }
    bufferSize = 2 * channels * frameCount;
    buffer = new char[bufferSize];
    sp<AudioRecord> record = new AudioRecord(inputSource,
                                             sampleRate,
                                             format,
                                             channelMask,
                                             String16(PACKAGE_NAME)
    );

    status = record->initCheck();
    if(status != NO_ERROR) {
        record.clear();
        ALOGD("Failed for initCheck()");
        return;
    }
    ALOGD("start");
    record->start();

    while(1) {
        int readLen = record->read(buffer, bufferSize);        
        int writeResult = -1;
        
        if(readLen > 0) {
            writeDataCount += readLen;
            if(NULL != file) {
                writeResult = fwrite(buffer, 1, readLen, file);                
                if(writeResult < readLen) {
                    ALOGE("Write Audio Record Stream error");
                }
            }             
            ALOGD("readLen = %d  writeResult = %d  writeDataCount = %d", readLen, writeResult, writeDataCount);            
        } else {
            ALOGE("Audio Record->read  readLen = 0");
        }
    }

    ALOGD("stop");
    record->stop();

    delete[] buffer;
    return;
}

/************************************************************
*
*    main in name space
*
************************************************************/
int main(int argc, char **argv) {    
    ProcessState::self()->startThreadPool();
    AudioRecordTest *test;
    if (argc != 4) {
        ALOGE("input param num error:%d\n", argc);
        return -1;
    }

    test = new AudioRecordTest();
    test->Execute(strtol(argv[1], NULL, 0), strtol(argv[2], NULL, 0), argv[3]);
    delete test;

    return 0;
}

}
/************************************************************
*
*    global main
*
************************************************************/
int main(int argc, char **argv) {
    if (argc != 4) {
        printf("Usage:\n");
        printf("%s <sample_rate> <channels> <out_file>\n", argv[0]);
        return -1;
    }
    return android::main(argc, argv);
}
