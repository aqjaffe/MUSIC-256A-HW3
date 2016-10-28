/*
  ==============================================================================

    FMSound.h
    Created: 19 Oct 2016 10:05:52pm
    Author:  Adam

  ==============================================================================
*/

#ifndef FMSOUND_H_INCLUDED
#define FMSOUND_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class FMSound : public SynthesiserSound
{
public:
    FMSound();
    bool appliesToNote (int /*midiNoteNumber*/) override;
    bool appliesToChannel (int /*midiChannel*/) override;
};

#endif  // FMSOUND_H_INCLUDED
