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
// Inicialización
//***************************************************************************************************************************************
void setup() {
  SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Inicio");
  LCD_Init();
  LCD_Clear(0x3CDB17);  //es el verde

  FillRect(0, 0, 319, 206, 0x761E);
   for (int i = 0; i < 320 - 30; i++) {
  LCD_Bitmap(i,  207, 32, 32, piedras);
   }
  String text1 = "Pelea violenta";
  LCD_Print(text1, 20, 100, 2, 0xffff, 0x421b);


}
//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************
void loop() {
  //30 porque es el ancho del tiburon
  for (int i = 0; i < 320 - 30; i++) {
    int anim2 = (i / 35) % 2;
    LCD_Sprite(i, 170, 30, 33, tiburonS, 3, anim2, 0, 0 );
    V_line( i - 1, 170, 33, 0x421b);
    delay(15);    
  }
 for (int i = 0; i < 320 - 32; i++) {
    int anim = (i / 35) % 2;
    LCD_Sprite(i, 138, 32, 32, megamanRojoS, 4, anim, 0, 0 );
    V_line( i - 1, 138, 33, 0x421b);
    delay(15);    
  }

}
