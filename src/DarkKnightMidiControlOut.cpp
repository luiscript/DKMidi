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

#include "DarkKnightMidiControlOut.hpp"


void DarkKnightMidiControlOut::setup()
{
    channel = 1;
    control = 1;
    value = 0;
    lastValue = 0;
}

void DarkKnightMidiControlOut::update()
{
    if(value != lastValue)
    {
        midiMessage.channel = channel;
        midiMessage.control = control;
        midiMessage.value = value;
        sendMidiMessage(&midiMessage);
        lastValue = value;
    }
}

void DarkKnightMidiControlOut::draw()
{
    
}

void DarkKnightMidiControlOut::addModuleParameters()
{
    gui->addSlider("Value", 0, 127, 0)->bind(value);
    ofxDatGuiFolder * config = gui->addFolder("CONFIG");
    ofxDatGuiTextInput * channelComponent = config->addTextInput("Channel", "1");
    channelComponent->onTextInputEvent(this, &DarkKnightMidiControlOut::onOutputChannelChange);
    
    ofxDatGuiTextInput * controlComponent = config->addTextInput("Control", "0");
    controlComponent->onTextInputEvent(this, &DarkKnightMidiControlOut::onOuputControlChange);
}

void DarkKnightMidiControlOut::onOutputChannelChange(ofxDatGuiTextInputEvent e)
{
    channel = ofToInt(e.target->getText());
}

void DarkKnightMidiControlOut::onOuputControlChange(ofxDatGuiTextInputEvent e)
{
    control = ofToInt(e.target->getText());
}
