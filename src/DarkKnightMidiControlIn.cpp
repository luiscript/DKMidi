//
//  DarkKnightMidiControlIn.cpp
//  luiscript
//
//  Created by luiscript on 12/01/18.
//
//

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

void DarkKnightMidiControlIn::drawMasterInput()
{
    
}

void DarkKnightMidiControlIn::drawMasterOutput()
{
    
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


