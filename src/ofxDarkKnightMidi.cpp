//
//  ofxDarkKnightMidi.cpp
//  luiscript
//
//  Created by luiscript on 12/01/18.
//
//

#include "ofxDarkKnightMidi.hpp"


void ofxDarkKnightMidi::setup()
{
     signalLedControl = signalLedPitch = ofColor::black;
}

void ofxDarkKnightMidi::update()
{
    signalLedControl = ofColor::black;
}

void ofxDarkKnightMidi::draw()
{
    ofPushStyle();
    ofSetColor(signalLedControl);
    ofDrawCircle(gui->getPosition().x + gui->getWidth() - 10, gui->getPosition().y + 10, 2);
    ofSetColor(signalLedPitch);
    ofDrawCircle(gui->getPosition().x + gui->getWidth() - 16, gui->getPosition().y + 10, 2);
    ofPopStyle();
    
}

void ofxDarkKnightMidi::addModuleParameters()
{
    midiInputList = midiIn.getInPortList();
    ofxDatGuiComponent * component = gui->addDropdown("MIDI Input", midiInputList);
    component->onDropdownEvent(this, &ofxDarkKnightMidi::onMidiInputListChange);
}

void ofxDarkKnightMidi::drawMasterInput()
{
    
}

void ofxDarkKnightMidi::drawMasterOutput()
{
   
}

void ofxDarkKnightMidi::onMidiInputListChange(ofxDatGuiDropdownEvent e)
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


void ofxDarkKnightMidi::newMidiMessage(ofxMidiMessage& msg)
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
                gui->setTheme(this->getGuiTheme());
                gui->setWidth(ofGetWidth()/5);
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

