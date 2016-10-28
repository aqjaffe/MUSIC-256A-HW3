/*
  ==============================================================================

    FMSound.cpp
    Created: 19 Oct 2016 10:05:52pm
    Author:  Adam

  ==============================================================================
*/

#include "FMSound.h"

FMSound::FMSound()
{
}

bool FMSound::appliesToNote(int /*midiNoteNumber*/)
{
    return true;
}

bool FMSound::appliesToChannel (int /*midiChannel*/)
{
    return true;
}
