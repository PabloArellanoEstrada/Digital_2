

//*****************************************************************************************************
// Variables
//*****************************************************************************************************

const int buttonPin1 = PUSH1;      // Pushbuttons  
const int buttonPin2 = PUSH2;  

int buttonStateA = 0;              // Pushbutton valor de lectura
int buttonStateB = 0; 

int pressed_ok = 0;                // Jugador 1 debouncing
int released_ok = 0;
int presionado = 0;
int i = 0;

int pressed_ok2 = 0;               // Jugador 2 debouncing
int released_ok2 = 0;
int presionado2 = 0;
int i2 = 0;

int permitir = 0;                  // Variables de control
int ganadorA = 0;
int ganadorB = 0;

#define LEDR PF_1                  // Semaforo
#define LEDG PF_3
#define LEDB PF_2

const int ledPinA0 =  PB_5;        // Leds Jugador 1
const int ledPinA1 =  PB_0;        
const int ledPinA2 =  PB_1;        
const int ledPinA3 =  PE_4;        
const int ledPinA4 =  PE_5;      
const int ledPinA5 =  PB_4;        
const int ledPinA6 =  PA_5;       
const int ledPinA7 =  PA_6;  

const int ledPinB0 =  PB_2;        // Leds Jugador 2
const int ledPinB1 =  PE_0;        
const int ledPinB2 =  PC_4;        
const int ledPinB3 =  PB_7;        
const int ledPinB4 =  PB_6;      
const int ledPinB5 =  PA_4;        
const int ledPinB6 =  PA_3;       
const int ledPinB7 =  PA_2;  

//*****************************************************************************************************
// Funciones
//*****************************************************************************************************

void semaforo (void);              // Semaforo
void verde (void);

void jugador_1(void);              // Jugador 1
void jugador_1_escoger(void);
void funcion_ganadorA(void);

void jugador_2(void);              // Jugador 2
void jugador_2_escoger(void);
void funcion_ganadorB(void);

//*****************************************************************************************************
// Setup
//*****************************************************************************************************

void setup() {                

  pinMode(LEDR, OUTPUT);                // Leds RGB
  pinMode(LEDG, OUTPUT); 
  pinMode(LEDB, OUTPUT);  

  pinMode(ledPinA0, OUTPUT);            // Pines salida para Jugador 1
  pinMode(ledPinA1, OUTPUT); 
  pinMode(ledPinA2, OUTPUT); 
  pinMode(ledPinA3, OUTPUT); 
  pinMode(ledPinA4, OUTPUT); 
  pinMode(ledPinA5, OUTPUT); 
  pinMode(ledPinA6, OUTPUT); 
  pinMode(ledPinA7, OUTPUT); 

  pinMode(ledPinB0, OUTPUT);            // Pines salida para Jugador 2
  pinMode(ledPinB1, OUTPUT); 
  pinMode(ledPinB2, OUTPUT); 
  pinMode(ledPinB3, OUTPUT); 
  pinMode(ledPinB4, OUTPUT); 
  pinMode(ledPinB5, OUTPUT); 
  pinMode(ledPinB6, OUTPUT); 
  pinMode(ledPinB7, OUTPUT);

  pinMode(buttonPin1, INPUT_PULLUP);    // Pines entrada Botones
  pinMode(buttonPin2, INPUT_PULLUP);  
}

//*****************************************************************************************************
// Loop Principal
//*****************************************************************************************************

void loop() {

   buttonStateA = digitalRead(buttonPin1);                     // 0. Lectura de Botones
   buttonStateB = digitalRead(buttonPin2);

   if (permitir == 0 && ganadorA == 0 && ganadorB == 0)        // 1. Al inicio solo semaforo
   {
      semaforo();
   }
   else if (permitir == 1 && ganadorA  == 0 && ganadorB == 0)  // 2. Se permite jugar presionando botones con verde en semaforo
   {  
      verde ();                                                   
      jugador_1(); 
      jugador_1_escoger();
      jugador_2(); 
      jugador_2_escoger();
   }
   else {                                                      // 3. Luces de ganador y deshabilitar otro
      if (ganadorA == 1)
      {
         funcion_ganadorA();
      }
      if (ganadorB == 1)
      {
         funcion_ganadorB();
      }
   }

}

//*****************************************************************************************************
// Funciones
//*****************************************************************************************************

void semaforo (void){                                             
  if (buttonStateA == 0 || buttonStateB == 0) {     // Si el boton es presionado 
  digitalWrite(LEDR, HIGH);   
  digitalWrite(LEDG, LOW);                          // Rojo
  digitalWrite(LEDB, LOW);    
  delay(1500);    
  digitalWrite(LEDR, HIGH);   
  digitalWrite(LEDG, HIGH);                         // Amarillo
  digitalWrite(LEDB, LOW);   
  delay(1500);                
   permitir = 1;                                    // Permite jugar ya en verde
  }
}

//-----------------------------------------------------------------------------------------------------

void verde (void){
  digitalWrite(LEDR, LOW);     
  digitalWrite(LEDG, HIGH);                         // Verde
  digitalWrite(LEDB, LOW);    
}

//-----------------------------------------------------------------------------------------------------

void jugador_1(void) 
{
    if (buttonStateA == 0)                 // Boton presionado?
    {
       pressed_ok = pressed_ok + 1;        // Contador presionado con rango de seguridad
       released_ok = 0;                    // Contador suelto en cero
       if (pressed_ok > 500)               // Desborde contador?
       {
           if (presionado == 0)            // Estuvo suelto antes?
           {    
               i = i + 1;                  // Incrementa contador
               presionado = 1;             // Se presiono
          
           }
           pressed_ok = 0;                 // Reinicia contador
        }
    }
    else                                   // No presinado?
    {
       released_ok = released_ok + 1;      // Contador suelto con rango de seguridad
       pressed_ok = 0;                     // Contador presioando en cero
       if (released_ok > 500)              // Desborde contador?
       {
          presionado = 0;                  // Se solto
          released_ok = 0;                 // Reinicia contador
       }
    }
}

//-----------------------------------------------------------------------------------------------------

void jugador_2(void) 
{
    if (buttonStateB == 0)                 // Boton presionado?
    {
       pressed_ok2 = pressed_ok2 + 1;      // Contador presionado con rango de seguridad
       released_ok2 = 0;                   // Contador suelto en cero
       if (pressed_ok2 > 500)              // Desborde contador?
       {
           if (presionado2 == 0)           // Estuvo suelto antes?
           {    
               i2 = i2 + 1;                // Incrementa contador
               presionado2 = 1;            // Se presiono
          
           }
           pressed_ok2 = 0;                // Reinicia contador
        }
    }
    else                                   // No presinado?
    {
       released_ok2 = released_ok2 + 1;    // Contador suelto con rango de seguridad 
       pressed_ok2 = 0;                    // Contador presioando en cero
       if (released_ok2 > 500)             // Desborde contador?
       {
          presionado2 = 0;                 // Se solto
          released_ok2 = 0;                // Reinicia contador
       }
    }
}

//-----------------------------------------------------------------------------------------------------

void jugador_1_escoger(void){              // Para escoger que Led se enciende
   switch (i) {

   case 1:                                 // A0
   digitalWrite(ledPinA0, HIGH);    
   break;

   case 2:                                 // A1
   digitalWrite(ledPinA1, HIGH);   
   digitalWrite(ledPinA0, LOW);   
   break;

   case 3:                                 // A2                                
   digitalWrite(ledPinA2, HIGH);   
   digitalWrite(ledPinA1, LOW);   
   break;

   case 4:                                 // A3
   digitalWrite(ledPinA3, HIGH);   
   digitalWrite(ledPinA2, LOW);   
   break;

   case 5:                                 // A4
   digitalWrite(ledPinA4, HIGH);   
   digitalWrite(ledPinA3, LOW);   
   break;

   case 6:                                 // A5
   digitalWrite(ledPinA5, HIGH);   
   digitalWrite(ledPinA4, LOW);   
   break;

   case 7:                                 // A6
   digitalWrite(ledPinA6, HIGH);   
   digitalWrite(ledPinA5, LOW);   
   break;

   case 8:                                 // A7
   digitalWrite(ledPinA7, HIGH);   
   digitalWrite(ledPinA6, LOW);   
   ganadorA = 1;                           // Establece a Jugador 1 como ganador
   break;
   }  
}

//-----------------------------------------------------------------------------------------------------

void jugador_2_escoger(void){              // Para escoger que Led se enciende
   switch (i2) {

   case 1:                                 // B0
   digitalWrite(ledPinB0, HIGH);    
   break;

   case 2:                                 // B1
   digitalWrite(ledPinB1, HIGH);   
   digitalWrite(ledPinB0, LOW);   
   break;

   case 3:                                 // B2
   digitalWrite(ledPinB2, HIGH);   
   digitalWrite(ledPinB1, LOW);   
   break;

   case 4:                                 // B3
   digitalWrite(ledPinB3, HIGH);   
   digitalWrite(ledPinB2, LOW);   
   break;

   case 5:                                 // B4
   digitalWrite(ledPinB4, HIGH);   
   digitalWrite(ledPinB3, LOW);   
   break;

   case 6:                                 // B5
   digitalWrite(ledPinB5, HIGH);   
   digitalWrite(ledPinB4, LOW);   
   break;

   case 7:                                 // B6
   digitalWrite(ledPinB6, HIGH);   
   digitalWrite(ledPinB5, LOW);   
   break;

   case 8:                                 // B7
   digitalWrite(ledPinB7, HIGH);   
   digitalWrite(ledPinB6, LOW);   
   ganadorB = 1;                           // Establece a Jugador 2 como ganador
   break;
   }  
}

//-----------------------------------------------------------------------------------------------------

void funcion_ganadorA(void){
   digitalWrite(ledPinB0, LOW);             // Color Azul apagado
   digitalWrite(ledPinB1, LOW);    
   digitalWrite(ledPinB2, LOW);
   digitalWrite(ledPinB3, LOW);   
   digitalWrite(ledPinB4, LOW);    
   digitalWrite(ledPinB5, LOW);
   digitalWrite(ledPinB6, LOW);   
   digitalWrite(ledPinB7, LOW);
       
   digitalWrite(ledPinA5, HIGH);            // Leds Rojo encendido 
   digitalWrite(ledPinA6, HIGH);    
   digitalWrite(ledPinA7, HIGH);
   
   digitalWrite(LEDR, HIGH);                // RGB Rojo encendido
   digitalWrite(LEDG, LOW);    
   digitalWrite(LEDB, LOW);    
   delay(1000);

   digitalWrite(ledPinA5, LOW);             // Leds Rojo apagado 
   digitalWrite(ledPinA6, LOW);    
   digitalWrite(ledPinA7, LOW);
   
   digitalWrite(LEDR, LOW);                 // RGB Rojo apagado
   digitalWrite(LEDG, LOW);    
   digitalWrite(LEDB, LOW);    
   delay(1000);
 
}

//-----------------------------------------------------------------------------------------------------

void funcion_ganadorB(void){
   
   digitalWrite(ledPinA0, LOW);             // Color Rojo apagado  
   digitalWrite(ledPinA1, LOW);    
   digitalWrite(ledPinA2, LOW);
   digitalWrite(ledPinA3, LOW);   
   digitalWrite(ledPinA4, LOW);    
   digitalWrite(ledPinA5, LOW);
   digitalWrite(ledPinA6, LOW);   
   digitalWrite(ledPinA7, LOW);   
   
   digitalWrite(ledPinB5, HIGH);            // Leds Azul encendido  
   digitalWrite(ledPinB6, HIGH);    
   digitalWrite(ledPinB7, HIGH);
 
   digitalWrite(LEDR, LOW);                 // RGB Azul encendido   
   digitalWrite(LEDG, LOW);    
   digitalWrite(LEDB, HIGH);    
   delay(1000);

   digitalWrite(ledPinB5, LOW);             // Leds Azul apagado 
   digitalWrite(ledPinB6, LOW);    
   digitalWrite(ledPinB7, LOW);
   
   digitalWrite(LEDR, LOW);                 // RGB Azul apagado   
   digitalWrite(LEDG, LOW);    
   digitalWrite(LEDB, LOW);    
   delay(1000);
}
