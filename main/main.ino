//Proyecto 3 Digital2
//Daniel Mundo19508
//Noel Prado19025
//8/04/2021

#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"
#include "pantalla.h"


//color de fondo  0x2AAD
//***************************************************************************************************************************************
// Variables
//***************************************************************************************************************************************

//lectura del potenciómetro
//const int analogInPin1 = PE_5;  // Analog input pin that the potentiometer is attached to
//const int analogInPin2 = PE_2;  // Analog input pin that the potentiometer is attached to
//int sensorValue1 = 0;        // value read from the pot
//int outputValue1 = 0;        // value output to the PWM (analog out)
//int sensorValue2 = 0;        // value read from the pot
//int outputValue2 = 0;        // value output to the PWM (analog out)

//control del joystick
//int potx1 = 122;
//int poty1 = 122;
//int potx2 = 122;
//int poty2 = 122;

int inByte = 0;
int x = 0;
int y = 0;


int anim2;
//***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************
void setup() {

  Serial.begin(115200);



  //***************************************************************************************************************************************

  SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Inicio");
  LCD_Init();
  LCD_Clear(0x0);  //es el azul
  LCD_Clear(0x0528);
  for (int i = 0; i < 320 - 30; i = i + 32) {
    LCD_Bitmap(i,  207, 32, 32, piedras);
  }

  FillRect(0, 0, 319, 206, 0x421b);

  for (int i = 0; i < 320 - 30; i = i + 32) {
    LCD_Bitmap(i,  0, 31, 24, cloud);
  }

  String text1 = "Pelea violenta";
  LCD_Print(text1, 20, 100, 2, 0xffff , 0x0528);


}
//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************
void loop() {


  //  // read the analog in value:
  //  sensorValue1 = analogRead(analogInPin1);
  //  // map it to the range of the analog out:
  //  potx1 = map(sensorValue1, 0, 4095, 0, 255);
  //
  //  // read the analog in value:
  //  sensorValue2 = analogRead(analogInPin2);
  //  // map it to the range of the analog out:
  //  poty1 = map(sensorValue2, 0, 4095, 0, 255);


  // ESTO ES LO DE PRUEBA DE LECTURA DEL ESP32
  Serial.print("hola");

  while (Serial.available() <= 0) {
    Serial.println('A'); // send a capital A
    delay(300);
  }
  
  if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
    Serial.print(inByte);
  }

  //  Serial.print("output X = ");
  //  Serial.print(potx1);
  //
  //  Serial.print("\t output Y = ");
  //  Serial.println(poty1);
  //  delay(10);
  //

  //codigo para controlar la posición según joystick
  //la dimensión de la pantalla es: 320*240 pixeles

  //  if (potx1 <= 100) {
  //    x--;
  //    if (x < 0) {
  //      x = 0;
  //    }
  //
  //  }
  //  if (potx1 >= 155) {
  //    x++;
  //  }
  //  if (x > 320) {
  //    x = 320;
  //  }
  //
  //  if (poty1 <= 100) {
  //    y++;
  //  }
  //  if (y > 240) {
  //    y = 240;
  //  }
  //
  //  if (155 <= poty1) {
  //    y--;
  //  }
  //  if (y < 0) {
  //    y = 0;
  //  }
  //
  //  anim2 = (x) % 3;
  //
  //  // LCD_Sprite(x,y, 30, 33, tiburonS, 3, anim2, 0, 0 );
  //  LCD_Sprite(x, y, 32, 33, ataqueMega, 3, anim2, 0, 0 );
  delay(15);
}
