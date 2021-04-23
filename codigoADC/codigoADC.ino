// Código para leer los analógicos en el esp32

#include "pitches.h"

const int an1 = 13;
const int an2 = 12;
const int an3 = 14;
const int an4 = 27;

const int bocinaPin = 34;


int sensorValue1 = 0;        // value read from the pot
int outputValue1 = 0;        // value output to the PWM (analog out)

int sensorValue2 = 0;        // value read from the pot
int outputValue2 = 0;        // value output to the PWM (analog out)

int sensorValue3 = 0;        // value read from the pot
int outputValue3 = 0;        // value output to the PWM (analog out)

int sensorValue4 = 0;        // value read from the pot
int outputValue4 = 0;        // value output to the PWM (analog out)


//cosas para la interrupcion

volatile int interruptCounter;
int i;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;


//vector de interrupcion
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);

}





void setup() {

  Serial2.begin(115200);

  timer = timerBegin(0, 1, true);   //prescaler = 1, para que corra a 80 Mhz
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 3040, true); //3040 ticks se necesitan para que pasen 38uS 
  timerAlarmEnable(timer);


}

void loop() {

  if ((interruptCounter > 0) & (interruptCounter < 176328)) {
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
    i++;
    dacWrite(25, rawData[i]);
  }

if(interruptCounter >= 176328){
  interruptCounter = 0;
}



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






  delay(100);
}
