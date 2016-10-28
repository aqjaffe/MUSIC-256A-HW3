/*
  ==============================================================================

    SynthAudioSource.h
    Created: 19 Oct 2016 10:19:41pm
    Author:  Adam

  ==============================================================================
*/

#ifndef SYNTHAUDIOSOURCE_H_INCLUDED
#define SYNTHAUDIOSOURCE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "FMVoice.h"
#include "FMSound.h"

struct SynthAudioSource : public AudioSource
{
public:
    SynthAudioSource (MidiKeyboardState& keyState);
    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void setModulatorFreq(double mf);
    void setIndex(double ind);
    double getIndex(int i); //TOOD: delete this

    // this collects real-time midi messages from the midi input device, and
    // turns them into blocks that we can process in our audio callback
    MidiMessageCollector midiCollector;

    // this represents the state of which keys on our on-screen keyboard are held
    // down. When the mouse is clicked on the keyboard component, this object also
    // generates midi messages for this, which we can pass on to our synth.
    MidiKeyboardState& keyboardState;

    // the synth itself!
    Synthesiser synth;
private:
    int nVoices;
    double modulatorFreq;
};

#endif  // SYNTHAUDIOSOURCE_H_INCLUDED
