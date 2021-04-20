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

int x2 = 0;
int y2 = 0;


String datos = "";
int anim1;
int anim2;
//***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************
void setup() {
  pinMode(PA_7, INPUT_PULLUP);

  
  Serial5.begin(115200);  //el serial que se comunica con el esp32

  SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
  Serial.begin(9600); //serial como de la pantalla
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
      //Serial.println(datos);
  
      String p1 = getValue(datos, ',', 0);
      String p2 = getValue(datos, ',', 1);
      String p3 = getValue(datos, ',', 2);
      String p4 = getValue(datos, ',', 3);
      
      potx1 = p1.toInt();     
      poty1 = p2.toInt();
      potx2 = p3.toInt();
      poty2 = p4.toInt();

      datos = "";
    }
  }



  //codigo para controlar la posición según joystick
  //la dimensión de la pantalla es: 320*240 pixeles
//PRIMER JOYSTICK
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

  anim1 = (x) % 3;

//SEGUNDO JOYSTICK  
  if (potx2 <= 100) {
    x2--;
    if (x2 < 0) {
      x2 = 0;
    }

  }
  if (potx2 >= 155) {
    x2++;
  }
  if (x2 > 320) {
    x2 = 320;
  }

  if (poty2 <= 100) {
    y2++;
  }
  if (y2 > 240) {
    y2 = 240;
  }

  if (155 <= poty2) {
    y2--;
  }
  if (y2 < 0) {
    y2 = 0;
  }

  anim2 = (x2) % 3;


  LCD_Sprite(x2,y2, 30, 33, tiburonS, 3, anim2, 0, 0 );
  LCD_Sprite(x, y, 32, 33, ataqueMega, 3, anim1, 0, 0 );
  delay(15);

  //aqui va el codigo del ataque 
  char ataque1 = digitalRead(PA_7);
  if(ataque1 == 0){
  String text1 = "ataque1";
  LCD_Print(text1, 20, 100, 2, 0xffff , 0x0528);

    
  }

   

  
}





//funcion para hacer el split de la lectura serial
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
