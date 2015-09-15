#include "mbed.h"
#include "USBMIDI.h"
#define bufflength 10

DigitalOut myled(LED1);

USBMIDI midi;
AnalogIn light0(A0);
AnalogIn light1(A1);

AnalogIn knob0(A2);
AnalogIn knob1(A3);
AnalogIn knob2(A4);
AnalogIn knob3(A5);

DigitalIn button0(D0, PullUp);
DigitalIn button1(D1, PullUp);
DigitalIn button2(D2, PullUp);
DigitalIn button3(D3, PullUp);

int button0last;
int button1last;
int button2last;
int button3last;

float knob0last;
float knob1last;
float knob2last;
float knob3last;

DigitalIn leftSwitch(D11, PullUp);
DigitalIn rightSwitch(D12, PullUp);

int buff1[bufflength];
int index1 = 0;

int buff2[bufflength];
int index2 = 0;

int knobBuff0[bufflength];
int knobIndex0 = 0;
int knobBuff1[bufflength];
int knobIndex1 = 0;
int knobBuff2[bufflength];
int knobIndex2 = 0;
int knobBuff3[bufflength];
int knobIndex3 = 0;

void lightSensor(float light, int holdSwitch, int *buffptr, int &buffIndex, int cc){
        if (holdSwitch == 0) {
            int val = (int)(light*127);
        buffptr[buffIndex] = val;
        printf("%d\n", buffptr[buffIndex]);
        int sum = 0;
        buffIndex++;
        if (buffIndex >= bufflength){
            buffIndex = 0;
        }
        for (int i =0; i < bufflength; i++){
            sum += buffptr[i];
        }
        uint8_t avg = sum / bufflength;
        // printf("%u\n", avg);
        if (val != avg){
            midi.write(MIDIMessage::ControlChange(cc, avg));
        }
    }
} // end lightSensor

void knob(float knob, int last, int *buffptr, int &buffIndex, int cc){
        int val = (int)(knob*127);
        buffptr[buffIndex] = val;
        printf("%d\n", buffptr[buffIndex]);
        int sum = 0;
        buffIndex++;
        if (buffIndex >= bufflength){
            buffIndex = 0;
        }
        for (int i =0; i < bufflength; i++){
            sum += buffptr[i];
        }
        uint8_t avg = sum / bufflength;
        // printf("%u\n", avg);
        
        if(val != avg){
            midi.write(MIDIMessage::ControlChange(cc, avg));
        }
} // end knob

void button(int button, int &last, int cc){
    int buttonPress;
    
    if (button == 0){
        buttonPress = 127;
    }
    else {
        buttonPress = 0;   
    }
    if (buttonPress != last){
        midi.write(MIDIMessage::ControlChange(cc, buttonPress));
    }
    last = buttonPress;
} // end button

int main() { 
        
        while (1){ 
            wait(0.02);       
            lightSensor(light0.read(), leftSwitch.read(), buff1, index1, 20);  
            lightSensor(light1.read(), rightSwitch.read(), buff2, index2, 21);   
        
            button(button0.read(), button0last, 22);
            button(button1.read(), button1last, 23);
            button(button2.read(), button2last, 24);
            button(button3.read(), button3last, 25);
            
            knob(knob0.read(), knob0last, knobBuff0, knobIndex0, 26);
            knob(knob1.read(), knob1last, knobBuff1, knobIndex1, 27);
            knob(knob2.read(), knob2last, knobBuff2, knobIndex2, 28);
            knob(knob3.read(), knob3last, knobBuff3, knobIndex3, 29);
            
        }
} // end main
