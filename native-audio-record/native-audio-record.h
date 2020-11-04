// Copyright 2008 The Android Open Source Project

#ifndef AUDIORECORDTEST_H_
#define AUDIORECORDTEST_H_

namespace android {

class AudioRecordTest{
    public:
        AudioRecordTest(void);
        ~AudioRecordTest() {};

        void Execute(int sample_rate, int channels, char *fileName);
};

};


#endif /*AUDIORECORDTEST_H_*/
