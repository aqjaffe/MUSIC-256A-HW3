/*
  ==============================================================================

    FMVoice.h
    Created: 19 Oct 2016 10:11:15pm
    Author:  Adam

  ==============================================================================
*/

#ifndef FMVOICE_H_INCLUDED
#define FMVOICE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "FMSound.h"
#include "Sine.h"
#include "Smooth.h"

struct FMVoice : public SynthesiserVoice
{
public:
    FMVoice();
    bool canPlaySound (SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound*, int /*currentPitchWheelPosition*/) override;

    void stopNote (float /*velocity*/, bool allowTailOff) override;
    void pitchWheelMoved (int /*newValue*/) override;
    void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override;
    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;
    void setModulatorFreq(double mf);
    void setIndex(double ind);
    double getIndex();

private:
    Sine carrier, modulator;
    Smooth smooth[2];
    double carrierFrequency, modulatorFrequency, index, level, envelope;
    bool onOff, tailOff;
};

#endif  // FMVOICE_H_INCLUDED
