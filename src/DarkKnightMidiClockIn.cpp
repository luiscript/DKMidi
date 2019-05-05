/*
 Copyright (C) 2019 Luis Fernando García [http://luiscript.com]
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include "DarkKnightMidiClockIn.hpp"


void DarkKnightMidiClockIn::setup()
{
    signalLedControl = ofColor::black;
    ticks = 0;
    oneBeatVector = twoBeatVector = oneBarVector = twoBarVector =
    fourBarVector = eightBarVector = 0.0;
}

void DarkKnightMidiClockIn::update()
{
    signalLedControl = fmod(ticks, 24.0) / 24.0 < 0.25
        ? ofColor(232, 181, 54)
        : ofColor::black
    ;
    oneBeatVector = fmod(ticks, 24.0) / 24.0;
    twoBeatVector = fmod(ticks, 48.0) / 48.0;
    oneBarVector = fmod(ticks, 96.0) / 96.0;
    twoBarVector = fmod(ticks, 192.0) / 192.0;
    fourBarVector = fmod(ticks, 384.0) / 384.0;
    eightBarVector = fmod(ticks, 768.0) / 768.0;
}

void DarkKnightMidiClockIn::draw()
{
    ofPushStyle();
    ofSetColor(signalLedControl);
    ofDrawCircle(gui->getPosition().x + gui->getWidth() - 10, gui->getPosition().y + 10, 2);
    ofPopStyle();
    
}

void DarkKnightMidiClockIn::addModuleParameters()
{
    gui->addSlider("1 Beat", 0, 1, 0)->setPrecision(4)->bind(oneBeatVector);
    gui->addSlider("2 Beats", 0, 1, 0)->setPrecision(4)->bind(twoBeatVector);
    gui->addSlider("1 Bar", 0, 1, 0)->setPrecision(4)->bind(oneBarVector);
    gui->addSlider("2 Bars", 0, 1, 0)->setPrecision(4)->bind(twoBarVector);
    gui->addSlider("4 Bars", 0, 1, 0)->setPrecision(4)->bind(fourBarVector);
    gui->addSlider("8 Bars", 0, 1, 0)->setPrecision(4)->bind(eightBarVector);
    
    midiInputList = midiIn.getInPortList();
    ofxDatGuiComponent * component = gui->addDropdown("MIDI Input", midiInputList);
    component->onDropdownEvent(this, &DarkKnightMidiClockIn::onMidiInputListChange);
    
}

void DarkKnightMidiClockIn::onMidiInputListChange(ofxDatGuiDropdownEvent e)
{
    if(midiIn.isOpen())
    {
        midiIn.closePort();
        midiIn.removeListener(this);
    }
    midiIn.openPort(midiInputList[e.child]);
    midiIn.ignoreTypes(false, false, false);
    midiIn.addListener(this);
}


void DarkKnightMidiClockIn::newMidiMessage(ofxMidiMessage& msg)
{
    // MIDI Clock tick standard
    if(msg.bytes[0] == 248)
    {
        ticks++;
    }
    if(msg.status == MIDI_START)
    {
        clockRunning = true;
        ticks = 0;
    }
    if(msg.status == MIDI_STOP)
    {
        clockRunning = false;
        ticks = 0;
    }
}

void DarkKnightMidiClockIn::unMount()
{
    ofRemoveListener(this->sendMidi, this, &DarkKnightMidiClockIn::newMidiMessage);
}


