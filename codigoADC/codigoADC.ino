// Código para leer los analógicos en el esp32

#include "pitches.h"
#define NOTE_C4_1 260

const int an1= 13;
const int an2= 12;
const int an3 = 14;
const int an4 = 27;

const int buzzerPin = 34;


int sensorValue1 = 0;        // value read from the pot
int outputValue1 = 0;        // value output to the PWM (analog out)

int sensorValue2 = 0;        // value read from the pot
int outputValue2 = 0;        // value output to the PWM (analog out)

int sensorValue3 = 0;        // value read from the pot
int outputValue3 = 0;        // value output to the PWM (analog out)

int sensorValue4 = 0;        // value read from the pot
int outputValue4 = 0;        // value output to the PWM (analog out)


int melody[] = {
   NOTE_C4_1,NOTE_C4, NOTE_D4, NOTE_C4,NOTE_F4,NOTE_E4,
   NOTE_C4_1,NOTE_C4,NOTE_D4,NOTE_C4,NOTE_G4,NOTE_F4,
   NOTE_C4_1,NOTE_C4,NOTE_C5,NOTE_A4,NOTE_F4,NOTE_F4, NOTE_E4,NOTE_D4,
   NOTE_AS4,NOTE_AS4,NOTE_A4,NOTE_F4,NOTE_G4,NOTE_F4};

int noteDurations[] = {
  4, 4, 2, 2,2,1,
  4, 4, 2, 2,2,1,
  4, 4, 2, 2,4,4,2,1, 
  4, 4, 2, 2,2,1};



void setup() {
  // initialize serial communications at 9600 bps:
  Serial2.begin(115200);
  pinMode(buzzerPin,OUTPUT);
}

void loop() {
  // read the analog in value:
  sensorValue1 = analogRead(an1);
  sensorValue2 = analogRead(an2);
  sensorValue3 = analogRead(an3);
  sensorValue4 = analogRead(an4);
  
  // map it to the range of the analog out:
  outputValue1 = map(sensorValue1, 0, 4095, 0, 255);
  outputValue2 = map(sensorValue2, 0, 4095, 0, 255);
  outputValue3 = map(sensorValue3, 0, 4095, 0, 255);
  outputValue4 = map(sensorValue4, 0, 4095, 0, 255);
    
  Serial2.print(outputValue1);
  Serial2.print(",");
  Serial2.print(outputValue2);
  Serial2.print(",");
  
//joystick 2 
  Serial2.print(outputValue3);
  Serial2.print(",");

  Serial2.println(outputValue4);

  
  for (int thisNote = 0; thisNote < 26; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote],noteDuration);

    int pauseBetweenNotes = noteDuration + 50;      //delay between pulse
    delay(pauseBetweenNotes);
    
    noTone(buzzerPin);                // stop the tone playing
  }

  




  delay(100);
}
