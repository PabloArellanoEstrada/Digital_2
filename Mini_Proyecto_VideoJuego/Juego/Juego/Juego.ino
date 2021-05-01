
//--------------------------------------------------------------------------------------------------------------------------------------
// Librerias
//--------------------------------------------------------------------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "pitches.h"
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
#include <SPI.h>                                           
#include <SD.h>
#define NOTE_C4_1 260
#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
#define OUT_MUSICA PF_2
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};


//--------------------------------------------------------------------------------------------------------------------------------------
// Variables 
//--------------------------------------------------------------------------------------------------------------------------------------

int anim = 0;                          // sprite
int aumentar = 0;
int anim2 = 0;
int aumentar2 = 0;

int buzzerPin = 40;                    // musica
int ledblue= 19;
 
const int buttonPush1 = PUSH1;         // controles
const int buttonPush2 = PUSH2;
int buttonState1 = 0;
int buttonState2 = 0;

int x;                                 // pilares uno
int y;
int xP = 239;
int yP = 66;  

int yB = 25;                           // ave uno
int movingRate = 3;
int fallRateInt = 0;
float fallRate = 0;

int x2;                                // pilares dos
int y2;
int xP2 = 239; 
int yP2 = 226;  

int yB2 = 185;                         // ave dos
int movingRate2 = 3;
int fallRateInt2 = 0;
float fallRate2 = 0;

int conta = 0;                         // contador punteo
int conta2 = 0;
int control_conta = 1;
int control_conta2 = 1;
int control_choque = 1;
int control_choque2 = 1;


//--------------------------------------------------------------------------------------------------------------------------------------
// Functions Prototypes
//--------------------------------------------------------------------------------------------------------------------------------------

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
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
//void LCD_SD_Sprite(int x, int y, int width, int height,int columns, int index, char flip, char offset,char * direccion);
void LCD_SD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, char * direccion);
void initiateGame();
void gameover();
void drawBird(int yB, int xP, int yP);
void drawPilars(int xP, int yP);
void drawBird2(int yB2, int xP2, int yP2);
void drawPilars2(int xP2, int yP2);
void gameover();


//--------------------------------------------------------------------------------------------------------------------------------------
// Setup: Inicializa la LCD, la SD, se imprime caratula con datos, y dos imagenes
//--------------------------------------------------------------------------------------------------------------------------------------

void setup() 
  {
  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Inicio");
  LCD_Init();
  LCD_Clear(0x00);
  FillRect(0, 0, 239, 319, 0x1513); 
  
  String PA7 = "Pablo Arellano 151379";                  //  Caratula
  String RL = "Raul Aguilar 17581";
  String U = "--Universidad del Valle--";
  String depa = "DEPTO. MECATRONICA";
  String curso = "Electronica Digital II";
  String pablo = "Ing. Pablo Mazariegos";
  String SC = "Score";
  String sep = "---------------------------------------";
  int h = 10;
  LCD_Print(U, 21, h, 1, 0xffff, 0x421b);
  LCD_Print(depa, 43, h + 15, 1, 0xffff, 0x1513);
  LCD_Print(curso, 28, h + 30, 1, 0xffff, 0x1513);
  LCD_Print(pablo, 30, h + 45, 1, 0xffff, 0x1513);
  LCD_Print(sep, 4, h + 60, 1, 0xffff, 0x1513);
  LCD_Print(PA7, 30, 100, 1, 0xffff, 0x1513);
  LCD_Print(RL, 37, 115, 1, 0xffff, 0x1513); 

  pinMode(buttonPush1, INPUT_PULLUP);                     //   Botones
  pinMode(buttonPush2, INPUT_PULLUP);
  pinMode(OUT_MUSICA, OUTPUT);

  Serial.begin(9600);                                     //    Empezar comunicacion serial y esperar a que el puerto se abra  
  SPI.setModule(0);                                       //    Empezar modulo SPI-0

  Serial.print("Initializing SD card...");                //    Inicializar SD
  pinMode(PA_3, OUTPUT);                                  //    Salida por default para que libreria SD funcione

  if (!SD.begin(PA_3))                                    //    No se ha iniciado la comunicacion al mandar el SS?
  {
     Serial.println("initialization failed!");            //    Fallo la inicializacion
     return;
  }
  Serial.println("initialization done.");                 //    Ya se inicializo

  LCD_SD_Bitmap(77, 200, 85, 45, "intr.txt");              //    Impresion Angry Bird

  delay (5000);
  
  LCD_SD_Bitmap(5, 160, 40, 40, "tw0.txt");               //    Impresion ave grande en imagenes pequeñas
  LCD_SD_Bitmap(45, 160, 40, 40, "tw1.txt");
  LCD_SD_Bitmap(85, 160, 40, 40, "tw2.txt");
  LCD_SD_Bitmap(125, 160, 40, 40, "tw3.txt");
  LCD_SD_Bitmap(165, 160, 40, 40, "tw4.txt");
  LCD_SD_Bitmap(205, 160, 29, 40, "tw5.txt");
  
  LCD_SD_Bitmap(5, 200, 40, 40, "tw6.txt");
  LCD_SD_Bitmap(45, 200, 40, 40, "tw7.txt");
  LCD_SD_Bitmap(85, 200, 40, 40, "tw14.txt");
  LCD_SD_Bitmap(125, 200, 40, 40, "tw9.txt");
  LCD_SD_Bitmap(165, 200, 40, 40, "tw10.txt");
  LCD_SD_Bitmap(205, 200, 29, 40, "tw11.txt");
  
  LCD_SD_Bitmap(5, 240, 40, 40, "tw12.txt");
  LCD_SD_Bitmap(45, 240, 40, 40, "tw13.txt");
  LCD_SD_Bitmap(85, 240, 40, 40, "tw14.txt");
  LCD_SD_Bitmap(125, 240, 40, 40, "tw15.txt");
  LCD_SD_Bitmap(165, 240, 40, 40, "tw16.txt");
  LCD_SD_Bitmap(205, 240, 29, 40, "tw17.txt");
  
  LCD_SD_Bitmap(5, 280, 40, 35, "tw18.txt");
  LCD_SD_Bitmap(45, 280, 40, 35, "tw19.txt");
  LCD_SD_Bitmap(85, 280, 40, 35, "tw20.txt");
  LCD_SD_Bitmap(125, 280, 40, 35, "tw21.txt");
  LCD_SD_Bitmap(165, 280, 40, 35, "tw22.txt");
  LCD_SD_Bitmap(205, 280, 29, 35, "tw23.txt");
  
  delay(5000);
      
  LCD_Clear(0x0000);                            //  Limpieza
  FillRect(0, 0, 240, 160, 0x9EDDb);            //  Impresion de rectangulos para juego
  FillRect(0, 161, 240, 320, 0x051Db);
}

//--------------------------------------------------------------------------------------------------------------------------------------
// Loop: En esta se dibujan las aves y los pilares. La posicion del ave cambia de acuerdo a los push y los pilares con el xP
//--------------------------------------------------------------------------------------------------------------------------------------

void loop() 
  {
  digitalWrite(OUT_MUSICA, HIGH);              //  Pin para musica
  H_line(0, 160, 240, 0x0000b);
  if (conta == 9 || conta2 == 9){              //  Si hay ganador, se termina el juego
      gameover();
  }  

  // ------------------------------------------ Ave 1 y Pilares 1 ----------------------------------------------------------------------

  xP = xP - movingRate;                        // coordenada de los pilares 
  drawPilars(xP, yP);                          // dibujar pilares  
  drawBird(yB, xP, yP);                        // dibujar ave
  yB+=fallRateInt;                             // caida
  fallRate = fallRate+0.5;                     // aceleracion
  fallRateInt = int(fallRate); 

  if (xP<=-51)                
  {
     xP=239;                                   // Resets xP to 239
     yP = random(15,89);                       // Numeros random para altura de columnas
     control_conta = 1;
     control_choque = 1;
     movingRate = conta + 3;
  }
   
  buttonState2 = digitalRead(buttonPush2);     // Boton para volar
  if (buttonState2 == LOW)
  {
    fallRate = -5;
  }

  if( ((xP>50) && (xP<68)) && ((yB < yP) || ((yB+13)>(yP+60) ))) 
  {  
    if (conta >= 1)
    {
      conta = conta - 1; 
    }
    else if (conta < 1)
    {
      conta = 0; 
    }
    yB = yP + 30;
    control_choque = 0;
  } 

  // ----------------------------------------- Ave 2 y Pilares 2 ----------------------------------------------------------------------

  xP2 = xP2 - movingRate2;                       // coordenada de los pilares 
  drawPilars2(xP2, yP2);                         // dibujar pilares  
  drawBird2(yB2, xP2, yP2);
  yB2+=fallRateInt2;                             // caida
  fallRate2 = fallRate2+0.5;                     // aceleracion
  fallRateInt2 = int(fallRate2);
   
  if (xP2<=-51) 
  {
     xP2=239;                                    // Resets xP to 239
     yP2 = random(10,89) +180;                   // Numeros random para altura de columnas
     control_conta2 = 1;
     control_choque2 = 1;
     movingRate2 = conta2 + 3;    
  }

  buttonState1 = digitalRead(buttonPush1);       // Boton para volar
  if (buttonState1 == LOW)
  {
    fallRate2 = -5;
  }   

  if( ((xP2>50) && (xP2<68)) && ((yB2 < yP2) || ((yB2+13)>(yP2+60) ))) 
  {  
    if (conta2 >= 1)
    {
      conta2 = conta2 - 1; 
    }
    else if (conta2 < 1)
    {
      conta2 = 0; 
    }
      yB2 = yP2 + 30;
      control_choque2 = 0;
   }
}

//--------------------------------------------------------------------------------------------------------------------------------------
// bird Uno: se ingresa altura del ave yB, posicion xP de pilar, y altura de abertura de pilar yp
//--------------------------------------------------------------------------------------------------------------------------------------

void drawBird(int yB, int xP, int yP) 
{
  aumentar = aumentar + 1;
  if (aumentar > 2)
  {
    anim = anim + 1;                           // anim para el sprite y que mueva alas
    aumentar = 0;
    if (anim > 2)
    {
      anim = 0;
    }
  } 

  // ------------------------------------------------------------------------------------------------------------------------------------
  
  if ((xP < 0) || (xP > 68))                  // Posicion fuera de columnas
  {
    if((yB > 12)&&(yB < 147))                 // Dentro del rango de espacio para volar
    {
      LCD_Sprite(50, yB, 18, 13, ave1,3, anim, 0, 0);
      FillRect(50, 12, 18, yB-12, 0x9EDDb);
      FillRect(50, yB+13, 18, 160-(yB+13), 0x9EDDb);
    }
    else if (yB <= 12)                        // Sube demasiado
    {
      yB = 12;
      fallRate = 10;     
    }
    else                                      // Baja demasiado
    {
      yB = 147;
      fallRate = -10;  
    }      
  }

  // --------------------------------------------------------------------------------------------------------------------------------------
  
  if ((xP >= 0) && (xP <= 68))                // Posicion dentro de columnas
  {
    if( (yB > yP) && ( (yB+13) < (yP+60)) )   // Fuera del rango de espacio para volar
    {
      LCD_Sprite(50, yB, 18, 13, ave1,3, anim, 0, 0);
      FillRect(50, yP, 18, abs(yB-yP), 0x9EDDb);
      FillRect(50, yB+13, 18, abs((yP+60)-(yB+13)), 0x9EDDb);  
      if ((xP < 10) && (control_conta == 1) && (control_choque == 1))
      {
         conta++;
         control_conta = 0;
         control_choque = 0;
      }
    }
    else if (yB < yP)                          // Sube demasiado
    {
      yB = yB;
      fallRate = 5;     
    }
    else if ((yB+13)>(yP+60))                  // Baja demasiado
    {
      yB = yB;
      fallRate = -5;
    }      
  }  
}

//--------------------------------------------------------------------------------------------------------------------------------------
// bird Dos: se ingresa altura del ave yB, posicion xP de pilar, y abertura de pilar yp
//--------------------------------------------------------------------------------------------------------------------------------------
void drawBird2(int yB2, int xP2, int yP2) 
{
  aumentar2 = aumentar2 + 1;
  if (aumentar2 > 2)
  {
    anim2 = anim2 + 1;                          // anim para el sprite y que mueva alas
    aumentar2 = 0; 
    if (anim2 > 2)
    {
      anim2 = 0;
    }
  }

  // ---------------------------------------------------------------------------------------------------------------------------------
 
  if ((xP2 < 0) || (xP2 > 68))                   // Posicion fuera de columnas
  {
    if((yB2 > 172)&&(yB2< 307))                  // Dentro del rango de espacio
    {
      LCD_Sprite(50, yB2, 18, 13, ave2,3, anim2, 0, 0);
      FillRect(50, 173, 18, yB2-173, 0x051Db);
      FillRect(50, yB2+13, 18, 320-(yB2+13), 0x051Db);
    }
    else if (yB2 < 172)                          // Sube demasiado
    {
      yB2 = 172;
      fallRate2 = 10;
    }
    else                                         // Baja demasiado
    {
      yB2 = 307;
      fallRate2 = -10;
    } 
  }  

  // -------------------------------------------------------------------------------------------------------------------------------------
    
  if ((xP2 >= 0) && (xP2 <= 68))                 // Posicion dentro de columnas
  {
    if( (yB2 > yP2) && ( (yB2+13) < (yP2+60)) )  // Fuera del rango de espacio para volar
    {
      LCD_Sprite(50, yB2, 18, 13, ave2,3, anim2, 0, 0);
      FillRect(50, yP2, 18, abs(yB2-yP2), 0x051Db);
      FillRect(50, yB2+13, 18, abs((yP2+60)-(yB2+13)), 0x051Db);  
       if ((xP2 < 10) && (control_conta2 == 1) && (control_choque2 == 1))
      {
         conta2++;
         control_conta2 = 0;
         control_choque2 = 0;
      }
    }
    else if (yB2 < yP2)                           // Sube demasiado
    {
      yB2 = yB2;
      fallRate2 = 5;
    }
    else if ((yB2+13)>(yP2+60))                   // Baja demasiado
    {
      yB2 = yB2;
      fallRate2 = -5;
    }      
  }    
}

//--------------------------------------------------------------------------------------------------------------------------------------
// Pilares 1: mueve pilares a diferentes velocidades, se ingresa posicion en X y altura de abertura de pilar Y
//--------------------------------------------------------------------------------------------------------------------------------------

void drawPilars(int x, int y) 
  {
  String text1 = " Score:";                              // Puntuacion
  String stringOne =  String(conta);
  FillRect(0, 0, 240, 12, 0x1513);
  LCD_Print(text1, 170, 0, 1, 0xffff, 0x1513);
  LCD_Print(stringOne, 225, 0, 1, 0xffff, 0x1513);

  if (x>=190)                                            // Entra lado izquierdo??                   
  {
     FillRect(x, 12, 240-x, y-12, 0x0642b);              // pilar verde arriba
     FillRect(x, y+60, 240-x, 160-(y+60), 0x0642b);      // pilar verde abajo
     FillRect(0, 12, 10, 160-12, 0x9EDDb);
  }
  else if((x<=189) && (x > 0))                           // En medio de pantalla??
  {    
     FillRect(x, 12, 50, y-12, 0x0642b);                 // pilar verde arriba
     FillRect(x, y+60, 50, 160-(y+60), 0x0642b);         // pilar verde abajo
     FillRect(x+50, 12, 15, y-12, 0x9EDDb);              // limpieza arriba
     FillRect(x+50, y+60, 15, 160-(y+60), 0x9EDDb);      // limpieza abajo
  }
  else if ((x>-51) && (x < 0))                           // Sale lado derecho??
  {
     FillRect(0, 12, 50+x, y-12, 0x0642b);               // pilar verde arriba
     FillRect(0, y+60, 50+x, 160-(y+60), 0x0642b);       // pilar verde abajo
     FillRect(50+x, 12, 15, y-12, 0x9EDDb);              // limpieza arriba
     FillRect(50+x, y+60, 15, 160-(y+60), 0x9EDDb);      // limpieza abajo
  }

}

//--------------------------------------------------------------------------------------------------------------------------------------
// Pilares 2: mueve pilares a diferentes velocidades, se ingresa posicion en X y avertura de pilar Y
//--------------------------------------------------------------------------------------------------------------------------------------

void drawPilars2(int x, int y) 
{  
  String text2 = " Score:";                              // Puntuacion
  String stringtwo =  String(conta2);  
  FillRect(0, 161, 240, 12, 0x1513);
  LCD_Print(text2, 170, 161, 1, 0xffff, 0x1513);
  LCD_Print(stringtwo, 225, 161, 1, 0xffff, 0x1513);  

  if (x>=190)                                            // Entra lado izquierdo??   
  {
     FillRect(x, 173, 240-x, y-173, 0x0642b);            // pilar verde arriba
     FillRect(x, y+60, 240-x, 320-(y+60), 0x0642b);      // pilar verde abajo
     FillRect(0, 173, 10, 160-12, 0x051Db);
  }
  else if ((x<=189) && (x > 0))                          // En medio de pantalla??
  {    
     FillRect(x, 173, 50, y-173, 0x0642b);               // pilar verde arriba
     FillRect(x, y+60, 50, 320-(y+60), 0x0642b);         // pilar verde abajo
     FillRect(x+50, 173, 15, y-173, 0x051Db);            // limpieza arriba
     FillRect(x+50, y+60, 15, 320-(y+60), 0x051Db);      // limpieza abajo
  }

    else if ((x>-51) && (x < 0))                         // Sale lado derecho??
  {
     FillRect(0, 173, 50+x, y-173, 0x0642b);             // pilar verde arriba
     FillRect(0, y+60, 50+x, 320-(y+60), 0x0642b);       // pilar verde abajo
     FillRect(50+x, 173, 15, y-173, 0x051Db);            // limpieza arriba
     FillRect(50+x, y+60, 15, 320-(y+60), 0x051Db);      // limpieza abajo
  }
}


//--------------------------------------------------------------------------------------------------------------------------------------
// GAMEOVER
//--------------------------------------------------------------------------------------------------------------------------------------
void gameover()
{
  while(conta == 9 || conta2 == 9)
  {
    if(conta==9)                                         // Jugador 1 gano?
    {
      digitalWrite(OUT_MUSICA, LOW);
      String Wp= "Congratulation";
      String W= "Player1 Win";
      FillRect(0, 0, 240, 320, 0x0000b);
      LCD_Print(Wp, 0, 135, 2, 0xffff, 0x0000b);
      LCD_Print(W, 1, 170, 2, 0xffff, 0x0000b);
     
    }
    if(conta2==9)                                        // Jugador 2 gano?
    {
      digitalWrite(OUT_MUSICA, LOW);
      String Wp= "Congratulation";
      String W= "Player2 Win";
      //FillRect(0, 0, 240, 320, 0x9EDDb);
      FillRect(0, 0, 240, 320, 0x0000b);
      LCD_Print(Wp, 0, 135, 2, 0xffff, 0x0000b);
      LCD_Print(W, 1, 170, 2, 0xffff, 0x0000b);
     
    } 
  }  
}   

//--------------------------------------------------------------------------------------------------------------------------------------
//  convertir caracter ASCII a numero
//--------------------------------------------------------------------------------------------------------------------------------------
unsigned char Char_to_uChar(char letra){     
  unsigned char num;
  if(letra>=48 && letra <=57){               // para pasar de numero = "0" a numero = 0
    num = letra - 48;                        // num = 0
  }
  else if (letra >= 97 && letra <=102){      // convierte de "a" a 97
    num = letra -87;                         // a valor numerico
  }
  return num;
}

//--------------------------------------------------------------------------------------------------------------------------------------
// Función para dibujar una imagen a partir de un arreglo de colores 
//--------------------------------------------------------------------------------------------------------------------------------------

void LCD_SD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, char * direccion){    
  File myFile = SD.open(direccion);
  uint16_t n = 0;                                          // contador 
  uint16_t dimension = width*height*2;                     // tamaño del array
  unsigned char bitmap_SD[dimension] = {};                 // array vacio de dimension ancho por alto por 2
  if (myFile) {
  
    while (myFile.available()) {

      unsigned char numero = 0;                            // inicializo variable
      for(uint8_t m = 0; m < 2; m++){                      // cada elemento va de dos en dos
        char caracter = myFile.read();
        unsigned char digito = Char_to_uChar(caracter);    // lo escribo en forma unsigned char 
        if (m == 0){
          numero = digito*16;                              // variable lo multiplico por 16 porque son dos bytes, porque es el primer digito va posicion mas significativa
        }
        else if (m == 1){
          numero = numero + digito;                        // segundo digito
        }
      }
      bitmap_SD[n] = numero;                               // agrego al arreglo el valor de numero
      n ++;                                                // contando el tamaño arreglo
    }
    myFile.close();
  } else {
    Serial.println("error opening ");
  }
   LCD_Bitmap(x,y,width,height,bitmap_SD);
}



      
//***************************************************************************************************************************************
// Función para inicializar LCD: Funciones obtenidas de clase 
//***************************************************************************************************************************************
void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++){
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER) 
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x48 ^ 0xC0); // LCD_DATA(0x19);
  //LCD_DATA(0x40|0x80|0x20|0x08); // LCD_DATA(0x19);
  //****************************************
 LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
//  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on 
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);   
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);   
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);   
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);   
  LCD_CMD(0x2c); // Write_memory_start
}
//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c){  
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);   
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
    }
  digitalWrite(LCD_CS, HIGH);
} 
//***************************************************************************************************************************************
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i,j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8); 
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);  
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************

void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+w;
  y2 = y+h;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = w*h*2-1;
  unsigned int i, j;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);
      
      //LCD_DATA(bitmap[k]);    
      k = k - 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}

//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background) 
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;
  
  if(fontSize == 1){
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if(fontSize == 2){
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }
  
  char charInput ;
  int cLength = text.length();
  Serial.println(cLength,DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength+1];
  text.toCharArray(char_array, cLength+1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1){
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2){
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}
//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]){  
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+width;
  y2 = y+height;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      //LCD_DATA(bitmap[k]);    
      k = k + 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset){
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 

  unsigned int x2, y2;
  x2 =   x+width;
  y2=    y+height;
  SetWindows(x, y, x2-1, y2-1);
  int k = 0;
  int ancho = ((width*columns));
  if(flip){
  for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width -1 - offset)*2;
      k = k+width*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k - 2;
     } 
  }
  }else{
     for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width + 1 + offset)*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k + 2;
     } 
  }
    
    
    }
  digitalWrite(LCD_CS, HIGH);
}
