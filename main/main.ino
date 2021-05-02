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
//estas variables sirven para controlar la memoria de partidas ganadas por los distintos jugadores
int barVidaJ1;
int barVidaJ2;
String J1;
String J2;
String cadena = "";
//estas variables sirven para controlar el movimiento de los jugadores
String datos = "";
//J1
int x;
int y;
int m1 = 0;
int anim1;
//J2
int x2;
int y2;
int k1 = 0;
int anim2;
//***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************
void setup() {
  pinMode(PA_7, INPUT_PULLUP); //Ataque del J1
  pinMode(PF_1, INPUT_PULLUP);//Ataque del J2

  Serial5.begin(115200);  //el serial que se comunica con el esp32

  SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
  Serial.begin(9600); //serial como de la pantalla
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Inicio");

  //************************de la Pantalla LCD*******************
  LCD_Init();
  //Se colaca el fondo en el que se encuentran los jugadores
  LCD_Clear(0x2AAD);
  //Se colaca el "piso" en el que se encuentran los jugadores
  for (int i = 0; i < 320 - 30; i = i + 32) {
    LCD_Bitmap(i,  207, 32, 32, piedras);
  }
  //Se colaca las decoraciones que estan en el aire
  for (int i = 0; i < 320 - 30; i = i + 32) {
    LCD_Bitmap(i,  0, 31, 24, cloud);
  }
  //Se muestra el titulo del juego
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




}
//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************
void loop() {
  //******************* codigo sd*******************
  //Primero se leen los resultados previos
  scores = SD.open("/res.txt");
  if (scores) {
    //se lee el documento hasta que ya no haya más datos
    while (scores.available()) {
      // se lee colocando una coma en medio de las lecturas
      //esto se realiza de este modo, debido a que es más fácil leer los datos con una coma en medio de estos
      cadena = cadena + "," + scores.read();
    }
    //se imprime en el serial los datos actualizados, aunque estos están codificados
    Serial.println(cadena);
    // close the file:
    scores.close();
    //se resta el 48 debido a que se obtiene el valor en ASCII
    J1 = getValue(cadena, ',', 1).toInt() - 48;
    J2 = getValue(cadena, ',', 3).toInt() - 48;
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening res.txt");
  }
  //se limpia cadena para reobtener los datos la próxima vez que corra el programa
  cadena = "";
  //*****************Configuraciones iniciales del juego********************
  //Se resetean los contadores de la vida
  barVidaJ2 = barVidaJ1 = 0;
  LCD_Sprite(0, 32, 130, 26, vida, 3, 0, 0, 0);
  LCD_Sprite(190, 32, 130, 26, vida, 3, 0, 1, 0);
  //Se colocan en la posicion inicial a los jugadores
  x = 0;
  y = 175;
  x2 = 290;
  y2 = 170;
  flipJ1 = 0;
  flipJ2 = 1;
  //Se borra titulo o ultima posicion de los personajes al momento de comenzar una nueva partida
  delay(1000);
  FillRect(0, 58, 320, 149, 0x2AAD);
  //*******************Inicio del juego*************************************
  //El juego solo acaba hasta que alguno de los jugadores reciba 3 disparos.
  while (barVidaJ1 < 3 & barVidaJ2 < 3 ) {
    //--------Recepcion de datos del esp32 ( se reciben en el serial 5 )----------
    while (Serial5.available()) {
      char inByte = Serial5.read();
      if (inByte != '\n') {
        //convierto todo a datos
        // los datos son mandados en el esp32 separados por comas
        datos.concat(inByte);
      } else {
        //Se utiliza la función getValue para separarlos según las comas, debido a que los datos no tienen la misma longitud siempre
        String p1 = getValue(datos, ',', 0);
        String p2 = getValue(datos, ',', 1);
        String p3 = getValue(datos, ',', 2);
        String p4 = getValue(datos, ',', 3);

        //se convierten los valores leídos a int para poder operarlos
        potx1 = p1.toInt();
        poty1 = p2.toInt();
        potx2 = p3.toInt();
        poty2 = p4.toInt();

        //se limpia la cadena para la proxima lectura
        datos = "";
      }
    }
    //-----------------------------------------------------------
    //------------Control de la pos. según joystick--------------
    //Nota: Las dimensiones de la pantalla son 320 X 240 pixeles
    //PRIMER JOYSTICK
    if (potx1 <= 100) {
      //se utiliza el valor 100, debido a que el joystick centrado se encuentra en aproximadamente 126
      //se utilizan valores poco más grandes, y poco más pequeños para confirmar que se deba dar movimiento en una dirección.
      // los mismos comentarios aplican para las otras direcciones de los joysticks.
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
    //Control de la vel. de movimiento de los personajes
    anim2 = (x2) % 2;
    anim1 = (x) % 3;
    LCD_Sprite(x2, y2, 30, 33, tiburonS, 3, anim2, flipJ2, 0 );
    LCD_Sprite(x, y, 28, 30, megaman, 3, anim1, flipJ1, 0 );
    //-----------------------------------------------------------
    //------Correciones de imperfecciones al momento de moverse los personajes-------------------
    //Mov. horizontal
    //personajes viendo a la derecha
    if (flipJ1 == 0)V_line(x - 1, y, 32, 0x2AAD);
    if (flipJ2 == 0)V_line(x2 - 1, y2, 32, 0x2AAD);
    //personajes viendo a la izquierda
    if (flipJ1 == 1)V_line(x + 32, y, 32, 0x2AAD);
    if (flipJ2 == 1)V_line(x2 + 32, y2, 32, 0x2AAD);
    //Mov. vertical
    H_line(x, y + 32, 32, 0x2AAD);
    H_line(x, y - 1, 32, 0x2AAD);
    H_line(x2, y2 + 32, 32, 0x2AAD);
    H_line(x2, y2 - 1, 32, 0x2AAD);
    //-----------------------------------------------------------
    delay(20);
    //-----Control del ataque por medio de los botones-----------
    //Para cuando J1 ataca
    char ataque1 = digitalRead(PA_7);
    if (ataque1 == 0) {//Los botones estan config. como Pull-Ups
      int yataque = y;
      if (flipJ1 == 0) {//Se comprueba para que lado apunta el jugador:
        //Apunta a la derecha
        for (m1 = x + 30; m1 < 320; m1++) {//se utiliza un ciclo for para que el ataque vaya en linea recta
          //desde donde se disparo
          int anim3 = (m1 / 32) % 3;
          LCD_Sprite(m1, yataque, 32, 29, ataqueMega, 3, anim3, flipJ1, 0);
          V_line(m1 - 1, yataque, 32, 0x2AAD);
          // a través de Vline se borra el rastro que deja el ataque
        }
      } else {//Apunta a la izquierda
        for (m1 = x - 30; m1 > 0; m1--) {//misma logica de arriba
          int anim3 = (m1 / 32) % 3;
          LCD_Sprite(m1, yataque, 32, 29, ataqueMega, 3, anim3, flipJ1, 0);
          V_line(m1 + 32, yataque, 32, 0x2AAD);
        }
      }
    }
    //Para cuando J2 ataca
    //Es la misma logica que en J1
    char ataque2 = digitalRead(PF_1);
    if (ataque2 == 0) {
      int yataque2 = y2;
      if (flipJ2 == 1) {
        for (k1 = x2 - 32; k1 > 0; k1--) {
          int anim4 = (k1 / 32) % 3;
          LCD_Sprite(k1, yataque2, 32, 21, ataqueTib, 3, anim4, flipJ2, 0);
          V_line(k1 + 32, yataque2, 32, 0x2AAD);
          //delay(5);
        }
      } else {
        for (k1 = x2 + 30; k1 < 320; k1++) {
          int anim4 = (k1 / 32) % 3;
          LCD_Sprite(k1, yataque2, 32, 21, ataqueTib, 3, anim4, flipJ2, 0);
          V_line(k1 - 1, yataque2, 32, 0x2AAD);
          //delay(5);
        }
      }
    }
    //----------------------------------------------------------------------------------------
    //--------------Rutinas para comprobar que el ataque acerto-------------------------------
    //Para el ataque del J2
    if (ataque2 == 0 ) {
      FillRect(k1, y2, 32, 21, 0x2AAD);
      int h = y2 - y; //se obtiene la posicion en el eje Y de J2 respecto a J1
      int d = x - x2; //se obtiene la posicion en el eje X de J1 respecto a J2
      int r = k1 - x; //se obtiene la posicion en el eje X de J1 respecto al Ataque
      int hit = 0; //sirve para saber si el ataque impacto o no en J1
      if ((flipJ2 == 0) & ((r >= 0) & (d >= 0)))hit++; //para acertar el ataque el jugador viendo hacia la derecha
      //ambas distancias tienen que ser positivas.
      if ((flipJ2 == 1) & ((r <= 0) & (d <= 0)))hit++; //para acertar el ataque el jugador viendo hacia la izquierda
      //ambas distancias tienen que ser negativas.
      if ((((h < 30) & (h >= 0)) | (h <= 0) & (h > -21)) & (hit == 1))barVidaJ1++; //para acertar el personaje atacado
      //tiene que estar como máximo 29 unidades arriba del ataque y 21 por debajo del ataque
      LCD_Sprite(0, 32, 130, 26, vida, 3, barVidaJ1, 0, 0); //el sprite de la vida cambia solo si se cumplen la condiciones ant.
    }
    //Para el ataque del J1
    //la logica es la misma que de J2
    if (ataque1  == 0 ) {
      FillRect(m1, y, 33, 29, 0x2AAD);
      int h = y2 - y;
      int d = x2 - x;
      int r = m1 - x2;
      int hit = 0;
      if ((flipJ1 == 0) & ((r >= 0) & (d >= 0)))hit++;
      if ((flipJ1 == 1) & ((r <= 0) & (d <= 0)))hit++;
      if ((((h < 30) & (h >= 0)) | (h <= 0) & (h > -25)) & (hit == 1))barVidaJ2++;
      LCD_Sprite(190, 32, 130, 26, vida, 3, barVidaJ2, 1, 0);
    }
  //----------------------------------------------------------------------------------------
  }
  //*********************************************************************
  //*******************Fin del juego*************************************
  //En estos if's se escribe quien es el ganador
  if (barVidaJ2 == 3) {//Si el contador de J2 tiene un valor de 3 entonces J1 gano.
    String text1 = "J1 Wins";//se muestra en pantalla al ganador
    J1 = J1.toInt() + 1;//Se incrementa el valor de victorias en el archivo .txt
    LCD_Print(text1, 90, 100, 2, 0xffff, 0x2AAD);
    delay(1000);
    String text2 = "Fatality";
    LCD_Print(text2, 90, 132, 2, 0xffff , 0x2AAD);
  }
  if (barVidaJ1 == 3) {//misma logica de arriba
    String text1 = "J2 Wins";
    J2 = J2.toInt() + 1;
    LCD_Print(text1, 90, 100, 2, 0xffff, 0x2AAD);
    delay(1000);
    String text2 = "Fatality";
    LCD_Print(text2, 90, 132, 2, 0xffff , 0x2AAD);
  }
  //*********************************************************************
  //********************* codigo tarjeda SD*******************************
  //reabriendo para escribir
  scores = SD.open("res.txt", FILE_WRITE);
  if (scores) {
    Serial.println("probando");

    scores.seek(0); //se coloca en la posición 0 para reescribir sobre los valores viejos
    scores.print(J1 + "," + J2);  //se imprimen los nuevos resultados separados por una coma
    // close the file:
    scores.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    //Serial.println("error opening test.txt");
    Serial.println("F de F");
  }
  //*********************************************************************
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
