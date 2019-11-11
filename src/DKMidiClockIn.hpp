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

#ifndef DKMidiClockIn_hpp
#define DKMidiClockIn_hpp


#include "DKModule.hpp"
#include "ofxMidi.h"

class DKMidiClockIn : public DKModule, public ofxMidiListener
{
private:
    ofxMidiIn midiIn;
    ofColor signalLedControl;
    float oneBeatVector;
    float twoBeatVector;
    float oneBarVector;
    float twoBarVector;
    float fourBarVector;
    float eightBarVector;
    bool clockRunning = false;
    int ticks;
    vector<string> midiInputList;
public:
    ofEvent<ofxMidiMessage&> sendMidi;
    void setup();
    void update();
    void draw();
    void addModuleParameters();
    void onMidiInputListChange(ofxDatGuiDropdownEvent);
    void newMidiMessage(ofxMidiMessage& eventArgs);
    void unMount();
};

#endif /* DKMidiClockIn_hpp */
