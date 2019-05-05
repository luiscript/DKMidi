/*
 Copyright (C) 2018 Luis Fernando García [http://luiscript.com]
 
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

#include "DarkKnightMidiControlIn.hpp"


void DarkKnightMidiControlIn::setup()
{
    signalLedControl = signalLedPitch = ofColor::black;
}

void DarkKnightMidiControlIn::update()
{
    signalLedControl = ofColor::black;
}

void DarkKnightMidiControlIn::draw()
{
    ofPushStyle();
    ofSetColor(signalLedControl);
    ofDrawCircle(gui->getPosition().x + gui->getWidth() - 10, gui->getPosition().y + 10, 2);
    ofSetColor(signalLedPitch);
    ofDrawCircle(gui->getPosition().x + gui->getWidth() - 16, gui->getPosition().y + 10, 2);
    ofPopStyle();
    
}

void DarkKnightMidiControlIn::addModuleParameters()
{
    midiInputList = midiIn.getInPortList();
    ofxDatGuiComponent * component = gui->addDropdown("MIDI Input", midiInputList);
    component->onDropdownEvent(this, &DarkKnightMidiControlIn::onMidiInputListChange);
}

void DarkKnightMidiControlIn::onMidiInputListChange(ofxDatGuiDropdownEvent e)
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


void DarkKnightMidiControlIn::newMidiMessage(ofxMidiMessage& msg)
{
    ofNotifyEvent(sendMidi, msg, this);
    string mapping;
    if(msg.control > 0)
    {
        mapping = ofToString(msg.channel) + "/" + ofToString(msg.control);
        if(this->getModuleMidiMapMode()){
            unordered_map<string, int *>::iterator it;
            it = poolMidiMappings.find(mapping);
            if(it != poolMidiMappings.end())
            {
                //poolMidiMappings.erase(it);
                
            } else {
                int * scale = new int;;
                poolMidiMappings.insert({mapping, scale});
                gui->setWidth(450);
                ofxDatGuiSlider * component = gui->addSlider(mapping, 0, 128);
                
                component->bind(*scale);
                component->toggleMidiMode();
            }
            
        } else {
            unordered_map<string, int *>::iterator it;
            it = poolMidiMappings.find(mapping);
            if(it != poolMidiMappings.end())
            {
                *it->second = msg.value;
            }
        }
        signalLedControl = ofColor::green;
        
    } else {
        mapping = ofToString(msg.channel) + "/" + ofToString(msg.pitch);
        signalLedPitch = msg.status == MIDI_NOTE_ON ? ofColor(232, 181, 54) : ofColor(0);
    }
}

void DarkKnightMidiControlIn::unMount()
{
    ofRemoveListener(this->sendMidi, this, &DarkKnightMidiControlIn::newMidiMessage);
}


