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
const int analogInPin1 = PE_5;  // Analog input pin that the potentiometer is attached to
const int analogInPin2 = PE_2;  // Analog input pin that the potentiometer is attached to
int sensorValue1 = 0;        // value read from the pot
int outputValue1 = 0;        // value output to the PWM (analog out)
int sensorValue2 = 0;        // value read from the pot
int outputValue2 = 0;        // value output to the PWM (analog out)

//control del joystick
int potx1 = 122;
int poty1 = 122;
int potx2 = 122;
int poty2 = 122;

/*int j1_pos_prev;
int j1_pos_new;
int j2_pos_prev;
int j2_pos_new;*/

int x_j1 = 0;
int y_j1  = 0;
int x_j2 = 0;
int y_j2  = 0;

int anim2;
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
<<<<<<< Updated upstream
  LCD_Clear(0x0);  //es el azul
  LCD_Clear(0x0528);
  for (int i = 0; i < 320 - 30; i = i + 32) {
    LCD_Bitmap(i,  207, 32, 32, piedras);
  }

  FillRect(0, 0, 319, 206, 0x421b);

  for (int i = 0; i < 320 - 30; i = i + 32) {
    LCD_Bitmap(i,  0, 31, 24, cloud);
  }
=======
  LCD_Clear(0x3CDB17);  //es el verde
  for (int i = 0; i < 320 - 30; i++) {
    LCD_Bitmap(i,  207, 32, 32, piedras);
  }
  FillRect(0, 0, 319, 206, 0x421b);
  LCD_Bitmap(100,  0, 31, 24, cloud);
  LCD_Bitmap(250,  0, 31, 24, cloud);
>>>>>>> Stashed changes

  String text1 = "Pelea violenta";
  LCD_Print(text1, 20, 100, 2, 0xffff , 0x0528);


}
//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************
void loop() {

  // read the analog in value:
  sensorValue1 = analogRead(analogInPin1);
  // map it to the range of the analog out:
  potx1 = map(sensorValue1, 0, 4095, 0, 255);

  // read the analog in value:
  sensorValue2 = analogRead(analogInPin2);
  // map it to the range of the analog out:
  poty1 = map(sensorValue2, 0, 4095, 0, 255);


  Serial.print("output X = ");
  Serial.print(potx1);

  Serial.print("\t output Y = ");
  Serial.println(poty1);
  delay(10);

  //codigo para controlar la posición según joystick
  //la dimensión de la pantalla es: 320x240 pixeles

  anim2 = (potx1) % 3;
  
  Mov_x(33);
  Mov_y(33);
  
  LCD_Sprite(x_j1, y_j1, 30, 33, tiburonS, 3, anim2, 0, 0 );
  
  V_line(x_j1 - 1, y_j1, y_j1 + 30, 0x421b);
  LCD_Sprite( x_j1, y_j1 + 33, 32, 32, megamanRojoS, 4, anim2, 0, 0 );
  V_line( x_j1 - 1, y_j1, y_j1 + 30, 0x421b);
  delay(15);
}

void Mov_x(int w) {
  if (potx <= 100) {
    aux--;
    if (potx < 0) {
      aux = 0;
    }
<<<<<<< Updated upstream

  }
  if (potx1 >= 155) {
    x++;
=======
  }
  if (potx >= 155) {
    aux++;
>>>>>>> Stashed changes
  }
  if (potx > 320 - w) {
    aux = 320;
  }
<<<<<<< Updated upstream

  if (poty1 <= 100) {
    y++;
  }
  if (y > 240) {
    y = 240;
  }

  if (155 <= poty1) {
    y--;
  }
  if (y < 0) {
    y = 0;
  }

  anim2 = (x) % 3;

  // LCD_Sprite(x,y, 30, 33, tiburonS, 3, anim2, 0, 0 );
  LCD_Sprite(x, y, 32, 33, ataqueMega, 3, anim2, 0, 0 );
  delay(15);
=======
}
void Mov_y(int w) {
  if (poty <= 100) {
    auy++;
  }
  if (poty > 240 - 32 - w) {
    auy = 240;
  }

  if (155 <= poty) {
    auy--;
  }
  if (poty < 0) {
    auy = 0;
  }
>>>>>>> Stashed changes
}
