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


//control del joystick
int potx1 = 122;
int poty1 = 122;
int potx2 = 122;
int poty2 = 122;

int x = 0;
int y = 0;

String datos = "";

int anim2;
//***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************
void setup() {

  Serial5.begin(115200);



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

  //aqui se reciben los datos del esp32 ( se reciben en el serial 5 )
  while (Serial5.available()) {
    char inByte = Serial5.read();
    if (inByte != '\n') {
      //convierto todo a datos
      datos.concat(inByte);

    }

    else {
      Serial.println(datos);
     
      potx1 = (datos.substring(0, 3)).toInt();
      poty1 = (datos.substring(4, 7)).toInt();
      potx2 = (datos.substring(8, 11)).toInt();
      poty2 = (datos.substring(12, 15)).toInt();
    
      //Serial.println((datos.substring(4, 7)));
      
      datos = "";
    }
  }




 


  //codigo para controlar la posición según joystick
  //la dimensión de la pantalla es: 320*240 pixeles

  if (potx1 <= 100) {
    x--;
    if (x < 0) {
      x = 0;
    }

  }
  if (potx1 >= 155) {
    x++;
  }
  if (x > 320) {
    x = 320;
  }

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
}
