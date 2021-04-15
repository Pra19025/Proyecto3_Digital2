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


//***************************************************************************************************************************************
// Functions Prototypes
//***************************************************************************************************************************************
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);

void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[], int columns, int index, char flip, char offset);

//***************************************************************************************************************************************
// Variables
//***************************************************************************************************************************************

//lectura del potenciómetro
const int analogInPin1 = PE_5;  // Analog input pin that the potentiometer is attached to
const int analogInPin2 = PE_2;  // Analog input pin that the potentiometer is attached to
int sensorValue1 = 0;        // value read from the pot
int outputValue1 = 0;        // value output to the PWM (analog out)
int sensorValue2 = 0;        // value read from the pot
int outputValue2 = 0;        // value output to the PWM (analog out)

//***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************
void setup() {

  Serial.begin(9600);

  //***************************************************************************************************************************************

  SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Inicio");
  LCD_Init();
  LCD_Clear(0x3CDB17);  //es el verde
  for (int i = 0; i < 320 - 30; i++) {
  LCD_Bitmap(i,  207, 32, 32, piedras);
   }
    FillRect(0, 0, 319, 206, 0x421b);
  LCD_Bitmap(100,  0, 31, 24, cloud);
  LCD_Bitmap(250,  0, 31, 24, cloud);
 
  String text1 = "Pelea violenta";
  LCD_Print(text1, 20, 100, 2, 0xffff, 0x421b);


}
//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************
void loop() {
  //30 porque es el ancho del tiburon
  for (int x = 0; x < 320 - 30; x++) {

    int anim2 = (x / 35) % 2;
    LCD_Sprite(x, 170, 30, 33, tiburonS, 3, anim2, 0, 0 );
    V_line( x - 1, 170, 33, 0x421b);
    delay(15);


    //***************************************************************************************************************************************


    // read the analog in value:
    sensorValue1 = analogRead(analogInPin1);
    // map it to the range of the analog out:
    outputValue1 = map(sensorValue1, 0, 4095, 0, 255);

    // read the analog in value:
    sensorValue2 = analogRead(analogInPin2);
    // map it to the range of the analog out:
    outputValue2 = map(sensorValue2, 0, 4095, 0, 255);


    // print the results to the serial monitor:
//    Serial.print("sensor X = " );
//    Serial.print(sensorValue1);
    Serial.print("output X = ");
    Serial.print(outputValue1);
    
//    Serial.print(" sensor Y = " );
//    Serial.print(sensorValue2);
    Serial.print("\t output Y = ");
    Serial.println(outputValue2);

    delay(20);
  }

}
