# MUSIC-256A-HW3

This program is a simple Polyphonic FM Synthesizer implemented in a VST plugin for use in a DAW like Ardour.

A few plans for the project went unfinished: an index slider to change the index of modulation, an frequency memory buffer to update the modulator frequency as the average of all previous carriers played, and a reset button to clear this buffer and set the average to 0. The goal of these components was to make the listener feel like the instrument was "droning along" after it is played for a long time (as the modulator frequency would converge), then the reset button would be a refreshing moment for their ears.

Created using JUCE, VisualStudio, Adour, and Faust.

Created as Homework 3 for MUSIC 256A at CCRMA at Stanford.
