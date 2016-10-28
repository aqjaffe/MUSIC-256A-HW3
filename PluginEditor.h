/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "SynthAudioSource.h"
#include <vector>


//==============================================================================
/**
*/
class MyAudioPluginAudioProcessorEditor :
public AudioProcessorEditor,
private ToggleButton::Listener,
private Slider::Listener,
private ComboBox::Listener,
private MidiInputCallback
{
public:
    MyAudioPluginAudioProcessorEditor (MyAudioPluginAudioProcessor&);
    ~MyAudioPluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized();
    void buttonClicked(Button *b) override;
    void sliderValueChanged(Slider *s) override;
    void comboBoxChanged (ComboBox* box) override;

private:
    MyAudioPluginAudioProcessor& processor;
    TextButton resetButton;
    Slider indexSlider;
    Label indexLabel;
    
    AudioDeviceManager audioDeviceManager;
    MidiKeyboardState keyboardState;
    MidiKeyboardComponent keyboardComponent;
    AudioSourcePlayer audioSourcePlayer;
    SynthAudioSource synthAudioSource;

    ComboBox midiInputList;
    Label midiInputListLabel;
    MidiInput* input;

    int lastMidiInputIndex;
    
    int nFreqs;
    std::vector<double> freqs;
    double avgFreq;
    
    void setMidiInput(int index);
    void handleIncomingMidiMessage(MidiInput*, const MidiMessage& message) override;
    void resetAvgFreq();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyAudioPluginAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
