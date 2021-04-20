// Código para leer los analógicos en el esp32


const int an1= 13;
const int an2= 12;
const int an3 = 14;
const int an4 = 27;


int sensorValue1 = 0;        // value read from the pot
int outputValue1 = 0;        // value output to the PWM (analog out)

int sensorValue2 = 0;        // value read from the pot
int outputValue2 = 0;        // value output to the PWM (analog out)

int sensorValue3 = 0;        // value read from the pot
int outputValue3 = 0;        // value output to the PWM (analog out)

int sensorValue4 = 0;        // value read from the pot
int outputValue4 = 0;        // value output to the PWM (analog out)







void setup() {
  // initialize serial communications at 9600 bps:
  Serial2.begin(115200);
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
    
 
  Serial2.print("X = ");
  Serial2.print(outputValue1);
 
  Serial2.print("\tY = ");
  Serial2.println(outputValue2);
  
//joystick 2 
  Serial2.print("X2 = ");
  Serial2.print(outputValue3);

  Serial2.print("\tY2 = ");
  Serial2.println(outputValue4);

  //esto es ya con la tiva, es prueba

while(Serial2.available()){
  char varIN = Serial.read();
  if(varIN == 'A'){

     // print the results to the Serial Monitor:
  Serial2.print("X = ");
  Serial2.print(outputValue1);

  Serial2.print("\tY = ");
  Serial2.println(outputValue2);
  
//joystick 2 
  Serial2.print("X2 = ");
  Serial2.print(outputValue3);

  Serial2.print("\tY2 = ");
  Serial2.println(outputValue4);
  
  }
}


  delay(100);
}
