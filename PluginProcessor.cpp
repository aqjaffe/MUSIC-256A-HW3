/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define DEFAULT_FREQ 1000


//==============================================================================
MyAudioPluginAudioProcessor::MyAudioPluginAudioProcessor() : onOff (0.0), gain (1.0), samplingRate(0.0)
{
    nChans = getTotalNumOutputChannels();
    audioBuffer = new float*[nChans];
    
    // set default values for guitar string synthesizer
    //guitarControl.setParamValue("/pluck/freq",440);
    //guitarControl.setParamValue("/pluck/res",0.5);
    //guitarControl.setParamValue("/pluck/onOff",0);
}

MyAudioPluginAudioProcessor::~MyAudioPluginAudioProcessor()
{
    delete [] audioBuffer;
}

//==============================================================================
const String MyAudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MyAudioPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MyAudioPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double MyAudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MyAudioPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MyAudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MyAudioPluginAudioProcessor::setCurrentProgram (int index)
{
}

const String MyAudioPluginAudioProcessor::getProgramName (int index)
{
    return String();
}

void MyAudioPluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void MyAudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    samplingRate = sampleRate;
    sine.setSamplingRate(sampleRate);
    sine.setFrequency(DEFAULT_FREQ);
}

void MyAudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MyAudioPluginAudioProcessor::setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet)
{
    // Reject any bus arrangements that are not compatible with your plugin

    const int numChannels = preferredSet.size();

   #if JucePlugin_IsMidiEffect
    if (numChannels != 0)
        return false;
   #elif JucePlugin_IsSynth
    if (isInput || (numChannels != 1 && numChannels != 2))
        return false;
   #else
    if (numChannels != 1 && numChannels != 2)
        return false;

    if (! AudioProcessor::setPreferredBusArrangement (! isInput, bus, preferredSet))
        return false;
   #endif

    return AudioProcessor::setPreferredBusArrangement (isInput, bus, preferredSet);
}
#endif

void MyAudioPluginAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{       
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for(int channel = 0; channel < nChans; ++channel)
    {
        audioBuffer[channel] = buffer.getWritePointer(channel);
    }
    
    // computing one block
    for(int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        audioBuffer[0][sample] = sine.tick() * onOff * gain;
        for (int channel = 1; channel < nChans; ++channel)
        {
            audioBuffer[channel][sample] = audioBuffer[0][sample];
        }
    }
}

//==============================================================================
bool MyAudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MyAudioPluginAudioProcessor::createEditor()
{
    return new MyAudioPluginAudioProcessorEditor (*this);
}

//==============================================================================
void MyAudioPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MyAudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MyAudioPluginAudioProcessor();
}
