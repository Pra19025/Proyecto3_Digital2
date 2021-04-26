//Proyecto 3 Digital2
//Daniel Mundo19508
//Noel Prado19025
//8/04/2021




#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>

// para la tarjeta SD
#include <SPI.h>
#include <SD.h>

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

//Para la SD
File scores;

//control del joystick
int potx1 = 122;
int poty1 = 122;
int potx2 = 122;
int poty2 = 122;
int flipJ1 = 0;
int flipJ2 = 1;


int buzzerPin = PE_3;

int x;
int y;

int x2;
int y2;

int m1 = 0;
int k1 = 0;
int barVidaJ1;
int barVidaJ2;

//estas variables sirven para controlar la memoria de partidas ganadas por los distintos jugadores
String J1;
String J2;
String cadena = "";


String datos = "";
int anim1;
int anim2;
//***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************
void setup() {
  pinMode(PA_7, INPUT_PULLUP);
  pinMode(PF_1, INPUT_PULLUP);


  Serial5.begin(115200);  //el serial que se comunica con el esp32

  SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
  Serial.begin(9600); //serial como de la pantalla
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Inicio");
  LCD_Init();
  LCD_Clear(0x2AAD);

  for (int i = 0; i < 320 - 30; i = i + 32) {
    LCD_Bitmap(i,  207, 32, 32, piedras);
  }
  LCD_Sprite(0, 32, 130, 26, vida, 3, 0, 0, 0);
  LCD_Sprite(190, 32, 130, 26, vida, 3, 0, 1, 0);
  for (int i = 0; i < 320 - 30; i = i + 32) {
    LCD_Bitmap(i,  0, 31, 24, cloud);
  }

<<<<<<< Updated upstream
  //  String text1 = "Pelea violenta";
  //  LCD_Print(text1, 20, 100, 2, 0xffff , 0x0528);
=======
  String text1 = "Pelea violenta";
  LCD_Print(text1, 20, 100, 2, 0xffff , 0x2AAD);


  //************************de la tarjeta SD*******************
  Serial.begin(115200);
  SPI.setModule(0);

  pinMode(PA_3, OUTPUT);

  if (!SD.begin(PA_3)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");


>>>>>>> Stashed changes


}
//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************
void loop() {
<<<<<<< Updated upstream
  barVidaJ2 = barVidaJ1 = 0 ;
=======


  // codigo sd
  //Primer se leen los resultados previos

  scores = SD.open("/res.txt");
  if (scores) {
    //Serial.println("res.txt:");

    // read from the file until there's nothing else in it:
    while (scores.available()) {
      cadena = cadena + "," + scores.read();

    }

    Serial.println(cadena);
    // close the file:
    scores.close();
    //se resta el 48 debido a que se obtiene el valor en ASCII 
    
    J1 = getValue(cadena, ',', 1).toInt() - 48;
    J2 = getValue(cadena, ',', 3).toInt() - 48;

    //Serial.println(J1);
    //Serial.print(J2);

  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening res.txt");
  }

  cadena = "";





  barVidaJ2 = barVidaJ1 = 0;
  LCD_Sprite(0, 32, 130, 26, vida, 3, 0, 0, 0);
  LCD_Sprite(190, 32, 130, 26, vida, 3, 0, 1, 0);
>>>>>>> Stashed changes
  x = 0;
  y = 175;
  x2 = 290;
  y2 = 170;
  FillRect(90, 100, 150, 60, 0x2AAD);
  while (barVidaJ1 < 3 & barVidaJ2 < 3 ) {
<<<<<<< Updated upstream
=======

>>>>>>> Stashed changes
    //aqui se reciben los datos del esp32 ( se reciben en el serial 5 )
    while (Serial5.available()) {
      char inByte = Serial5.read();
      if (inByte != '\n') {
        //convierto todo a datos
        datos.concat(inByte);
<<<<<<< Updated upstream

      }

      else {
        //Serial.println(datos);
=======
      } else {

>>>>>>> Stashed changes
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
      flipJ1 = 1;
      if (x < 0)x = 0;
    }
    if (potx1 >= 155) {
      x++;
      flipJ1 = 0;
    }
    if (x > 287)x = 287;
    if (poty1 <= 100)y++;
    if (y > 170)y = 170;
    if (155 <= poty1)y--;
    if (y < 58)y = 58;

    anim1 = (x) % 3;

    //SEGUNDO JOYSTICK
    if (potx2 <= 100) {
      x2--;
      flipJ2 = 1;
      if (x2 < 0)x2 = 0;
    }
    if (potx2 >= 155) {
      x2++;
      flipJ2 = 0;
    }
    if (x2 > 287)x2 = 287;
    if (poty2 <= 100)y2++;
    if (y2 > 170)y2 = 170;
    if (155 <= poty2)y2--;
    if (y2 < 58) y2 = 58;

    anim2 = (x2) % 3;

    LCD_Sprite(x2, y2, 30, 33, tiburonS, 3, anim2, flipJ2, 0 );
    LCD_Sprite(x, y, 28, 30, megaman, 3, anim1, flipJ1, 0 );

    if (flipJ1 == 0)V_line(x - 1, y, 32, 0x2AAD);
    if (flipJ1 == 1)V_line(x + 32, y, 32, 0x2AAD);
    if (flipJ2 == 0)V_line(x2 - 1, y2, 32, 0x2AAD);
    if (flipJ2 == 1)V_line(x2 + 32, y2, 32, 0x2AAD);
    H_line(x, y + 32, 32, 0x2AAD);
    H_line(x, y - 1, 32, 0x2AAD);
    H_line(x2, y2 + 32, 32, 0x2AAD);
    H_line(x2, y2 - 1, 32, 0x2AAD);

    delay(20);
    //aqui va el codigo del ataque
    char ataque1 = digitalRead(PA_7);
    if (ataque1 == 0) {
      int yataque = y;
      if (flipJ1 == 0) {
        for (m1 = x + 30; m1 < 288; m1++) {
          int anim3 = (m1 / 32) % 3;
          LCD_Sprite(m1, yataque, 32, 29, ataqueMega, 3, anim3, flipJ1, 0);
          V_line(m1 - 1, yataque, 32, 0x2AAD);
          delay(5);
        }
      } else {
        for (m1 = x - 30; m1 > 0; m1--) {
          int anim3 = (m1 / 32) % 3;
          LCD_Sprite(m1, yataque, 32, 29, ataqueMega, 3, anim3, flipJ1, 0);
          V_line(m1 + 32, yataque, 32, 0x2AAD);
        }
      }
    }
    char ataque2 = digitalRead(PF_1);
    if (ataque2 == 0) {
      int yataque2 = y2;
      if (flipJ2 == 1) {
        for (k1 = x2 - 32; k1 > 0; k1--) {
          int anim4 = (k1 / 32) % 3;
          LCD_Sprite(k1, yataque2, 32, 21, ataqueTib, 3, anim4, 1, 0);
          V_line(k1 + 32, yataque2, 32, 0x2AAD);
          delay(5);
        }
      } else {
        for (k1 = x2 + 30; k1 < 288; k1++) {
          int anim4 = (k1 / 32) % 3;
          LCD_Sprite(k1, yataque2, 32, 21, ataqueTib, 3, anim4, flipJ2, 0);
          V_line(k1 - 1, yataque2, 32, 0x2AAD);
          delay(5);
        }
      }
    }
    //La condicional expresa:
    //si: El boton se presiono y La distancia entre el bitmap de pos. más grande es menor a:
    //              -> 30 (aprox. el largo del personaje) si el ataque pasa por debajo del personaje.
<<<<<<< Updated upstream
   //              -> 21 (largo del ataque) si el ataque pasa por encima del personaje.
=======
    //              -> 21 (largo del ataque) si el ataque pasa por encima del personaje.
>>>>>>> Stashed changes
    if (ataque2 == 0 & (((y2 - y < 30) & (y2 - y >= 0)) | (y - y2 >= 0) & (y - y2 < 21))) {//si las condiciones se cumplen:
      barVidaJ1++; //Se aumenta el valor de la variable valores posibles 1,2 o 3 (este ultimo rompe el while)
      LCD_Sprite(0, 32, 130, 26, vida, 3, barVidaJ1, 0, 0); //se varia el parametro index (1 = vida amarilla y 2 = vida roja)
    }
    //Misma logica de arriba solo que: -> 29 (aprox. el largo del personaje) si el ataque pasa por debajo del personaje.
    //                                  -> 21 (aprox. largo del ataque) si el ataque pasa por encima del personaje.
    if (ataque1 == 0 & (((y2 - y < 29) & (y2 - y >= 0)) | (y - y2 >= 0) & (y - y2 < 25))) {
      barVidaJ2++;
      LCD_Sprite(190, 32, 130, 26, vida, 3, barVidaJ2, 1, 0);
    }
<<<<<<< Updated upstream
=======

>>>>>>> Stashed changes
  }//Fin del While
  //En estos if's se escribe quien es el ganador
  if (barVidaJ2 == 3) {
    String text1 = "J1 Wins";
    J1 = J1.toInt()+1;
    LCD_Print(text1, 90, 100, 2, 0xffff, 0x2AAD);
    delay(1000);
    String text2 = "Fatality";
    LCD_Print(text2, 90, 132, 2, 0xffff , 0x2AAD);
  }
  if (barVidaJ1 == 3) {
    String text1 = "J2 Wins";
    J2 = J2.toInt() + 1;
    LCD_Print(text1, 90, 100, 2, 0xffff, 0x2AAD);
    delay(1000);
    String text2 = "Fatality";
    LCD_Print(text2, 90, 132, 2, 0xffff , 0x2AAD);
  }

  //******************* codigo tarjeda SD****************************



  //reabriendo para escribir

    scores = SD.open("res.txt", FILE_WRITE);
    if (scores) {
      Serial.println("probando");
   
      scores.seek(0);
      scores.print(J1 + "," + J2);
      // close the file:
      scores.close();
      Serial.println("done.");
    } else {
      // if the file didn't open, print an error:
      //Serial.println("error opening test.txt");
        Serial.println("F de F");
    }


}



//funcion para hacer el split de la lectura serial
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
