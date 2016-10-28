/*
  ==============================================================================

    FMVoice.cpp
    Created: 19 Oct 2016 10:11:15pm
    Author:  Adam

  ==============================================================================
*/

#include "FMVoice.h"

FMVoice::FMVoice()
{
    carrierFrequency = 440.0;
    modulatorFrequency = 1000.0; //set the default value
    index = 0.0;
    level = 0.0;
    envelope = 0.0;
    onOff = false;
    tailOff = true;
    carrier.setSamplingRate(getSampleRate());
    modulator.setSamplingRate(getSampleRate());
}

bool FMVoice::canPlaySound (SynthesiserSound* sound)
{
    return dynamic_cast<FMSound*>(sound) != nullptr;
}

void FMVoice::startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound*, int /*currentPitchWheelPosition*/)
{
    
    // converting MIDI note number into freq
    carrierFrequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    
    // we don't want an ugly sweep when the note begins...
    smooth[0].setSmooth(0);
    smooth[0].tick(carrierFrequency);

    // standard smoothing...
    for(int i = 0; i < 2; i++){
        smooth[i].setSmooth(0.999);
    }

    level = velocity;
    // level = pow(velocity,2); // if we want linear dynamic

    // tells the note to begin!
    onOff = true;

    // These parameters could be controlled with UI elements and could
    // be assigned to specific MIDI controllers. If you do so,
    // don't forget to smooth them!
    modulator.setFrequency(modulatorFrequency);
    index = 150;
    
}

void FMVoice::stopNote (float /*velocity*/, bool allowTailOff)
{
    onOff = false; // end the note
    level = 0; // ramp envelope to 0 if tail off is allowed

    tailOff = allowTailOff;
}

void FMVoice::pitchWheelMoved (int /*newValue*/)
{
    // Pitch wheel is an important standard MIDI feature that should be implemented
}

void FMVoice::controllerMoved (int /*controllerNumber*/, int /*newValue*/)
{
    // Thats where you would change the value of the modulator index and frequency
    // if you wanted to control them with MIDI controllers
}

void FMVoice::renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{    
    // only compute block if note is on!
    if(envelope != 0 || onOff){
        while (--numSamples >= 0){
            envelope = smooth[1].tick(level); // here we use a smoother as an envelope generator
            carrier.setFrequency(smooth[0].tick(carrierFrequency) + modulator.tick() * index);
            const float currentSample = (float) carrier.tick() * envelope;
            
            for (int i = outputBuffer.getNumChannels(); --i >= 0;){
                outputBuffer.addSample (i, startSample, currentSample);
            }
            ++startSample;

            // if tail off is disabled, we end the note right away, otherwise, we wait for envelope
            // to reach a safe value
            if(!onOff && (envelope < 0.001 || !tailOff)){
                envelope = 0;
                clearCurrentNote();
            }
        }
    }
}

void FMVoice::setModulatorFreq(double mf) {
    modulatorFrequency = mf;
}

void FMVoice::setIndex(double ind) {
    index = ind;
}

double FMVoice::getIndex() {
    return index;
}