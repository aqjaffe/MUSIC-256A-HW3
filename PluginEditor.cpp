/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"
#include "Sine.h"

#define DEFAULT_FREQ 440

//==============================================================================
MyAudioPluginAudioProcessorEditor::MyAudioPluginAudioProcessorEditor (MyAudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard),
    synthAudioSource (keyboardState), lastMidiInputIndex (0)
{
    audioDeviceManager.initialise (0, 2, nullptr, true, String(), nullptr);

    audioSourcePlayer.setSource (&synthAudioSource);

    audioDeviceManager.addAudioCallback (&audioSourcePlayer);
    audioDeviceManager.addMidiInputCallback (String(), &(synthAudioSource.midiCollector));

    // reset button
    addAndMakeVisible(resetButton);
    resetButton.setButtonText("Reset");
    resetButton.addListener(this);
    
    // MIDI Inputs
    addAndMakeVisible(midiInputListLabel);
    midiInputListLabel.setText ("MIDI Input:", dontSendNotification);
    midiInputListLabel.attachToComponent (&midiInputList, true);

    // menu listing all the available MIDI inputs
    addAndMakeVisible(midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable ("No MIDI Inputs Enabled");
    const StringArray midiInputs (MidiInput::getDevices());
    midiInputList.addItemList (midiInputs, 1);
    midiInputList.setSelectedId(0); // default
    midiInputList.addListener (this);

    // by default we use the first MIDI input
    //setMidiInput(0);

    // The MIDI keyboard
    addAndMakeVisible(keyboardComponent);
        
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 300);
    
    addAndMakeVisible(indexSlider);
    indexSlider.setRange(0.0, 1.0);
    indexSlider.setValue(0.5);
    indexSlider.addListener(this);
    
    //addAndMakeVisible(gainLabel);
    indexLabel.setText("Index", dontSendNotification);
    indexLabel.attachToComponent(&indexSlider, true);
    
    nFreqs = 0;
    avgFreq = DEFAULT_FREQ;
}

MyAudioPluginAudioProcessorEditor::~MyAudioPluginAudioProcessorEditor()
{
    audioSourcePlayer.setSource (nullptr);
    audioDeviceManager.removeMidiInputCallback (String(), &(synthAudioSource.midiCollector));
    audioDeviceManager.removeAudioCallback (&audioSourcePlayer);
    audioDeviceManager.removeMidiInputCallback (MidiInput::getDevices()[midiInputList.getSelectedItemIndex()], this);
    midiInputList.removeListener (this);
}

//==============================================================================
void MyAudioPluginAudioProcessorEditor::resized()
{
    int buttonSize = 60;
    int margin = 60;
    resetButton.setBounds((getWidth() - buttonSize) / 2, margin, buttonSize, buttonSize);
    indexSlider.setBounds(margin, 2 * margin, getWidth() - margin, buttonSize);
    midiInputList.setBounds (margin, 0, getWidth() - margin, 20);
    keyboardComponent.setBounds (0, 3 * margin, getWidth(), 64);
}

void MyAudioPluginAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);
    g.setColour (Colours::black);
    g.setFont (Font("Courier", 15.0f, 0));
}

void MyAudioPluginAudioProcessorEditor::buttonClicked(Button *b)
{
    if (b == &resetButton) {
        resetAvgFreq();
        std::cout << "reset button pressed" << std::endl;
    }
}

void MyAudioPluginAudioProcessorEditor::sliderValueChanged (Slider* s)
{
    if (s == &indexSlider) synthAudioSource.setIndex(indexSlider.getValue());
    std::cout << synthAudioSource.getIndex(0) << std::endl;
}

void MyAudioPluginAudioProcessorEditor::comboBoxChanged (ComboBox* box)
{
    if (box == &midiInputList) setMidiInput (midiInputList.getSelectedItemIndex());
}

void MyAudioPluginAudioProcessorEditor::setMidiInput (int index)
{
    const StringArray list (MidiInput::getDevices());
    audioDeviceManager.removeMidiInputCallback (list[lastMidiInputIndex], this);
    const String newInput (list[index]);
    if (! audioDeviceManager.isMidiInputEnabled (newInput))
        audioDeviceManager.setMidiInputEnabled (newInput, true);
    audioDeviceManager.addMidiInputCallback (newInput, this);
    midiInputList.setSelectedId (index + 1, dontSendNotification);
    lastMidiInputIndex = index;
    
    input = MidiInput::openDevice(midiInputList.getSelectedId(), this);
    input->start();
}

void MyAudioPluginAudioProcessorEditor::handleIncomingMidiMessage(MidiInput* input, const MidiMessage& message)
{
    std::cout << "calling function: handleIncomingMidiMessage(...)" << std::endl;
    freqs.push_back(MidiMessage::getMidiNoteInHertz(message.getNoteNumber()));
    nFreqs++;
    
    double totalFreq = 0.0;
    for(std::vector<double>::iterator it = freqs.begin(); it != freqs.end(); ++it) {
        totalFreq += *it;
    }
    avgFreq = totalFreq / (double) nFreqs;
    synthAudioSource.setModulatorFreq(avgFreq);
}

void MyAudioPluginAudioProcessorEditor::resetAvgFreq() {
    freqs.clear();
    nFreqs = 0;
    avgFreq = DEFAULT_FREQ;
}
