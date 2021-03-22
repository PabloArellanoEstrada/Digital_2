

const int buttonPin1 = PUSH1;      // pushbutton  
const int buttonPin2 = PUSH2;  

int buttonStateA = 0;              // variable for reading the pushbutton status
int buttonStateB = 0; 

int pressed_ok = 0;             //Jugador 1 variables tipo entero
int released_ok = 0;
int presionado = 0;
int i = 0;

int permitir = 0;
int ganadorA = 0;
int ganadorB = 0;

#define LEDR PF_1                  // semaforo
#define LEDG PF_3
#define LEDB PF_2

const int ledPinA0 =  PB_5;        // the number of the LED pin
const int ledPinA1 =  PB_0;        
const int ledPinA2 =  PB_1;        
const int ledPinA3 =  PE_4;        
const int ledPinA4 =  PE_5;      
const int ledPinA5 =  PB_4;        
const int ledPinA6 =  PA_5;       
const int ledPinA7 =  PA_6;  

const int ledPinB0 =  PB_2;        // the number of the LED pin
const int ledPinB1 =  PE_0;        
const int ledPinB2 =  PC_4;        
const int ledPinB3 =  PB_7;        
const int ledPinB4 =  PB_6;      
const int ledPinB5 =  PA_4;        
const int ledPinB6 =  PA_3;       
const int ledPinB7 =  PA_2;  

void semaforo (void);
void verde (void);
void jugador_1(void);
void jugador_1_escoger(void);
void funcion_ganadorA(void);



void setup() {                

  pinMode(LEDR, OUTPUT);         // initialize the digital pin as an output.
  pinMode(LEDG, OUTPUT); 
  pinMode(LEDB, OUTPUT);  

  pinMode(ledPinA0, OUTPUT); 
  pinMode(ledPinA1, OUTPUT); 
  pinMode(ledPinA2, OUTPUT); 
  pinMode(ledPinA3, OUTPUT); 
  pinMode(ledPinA4, OUTPUT); 
  pinMode(ledPinA5, OUTPUT); 
  pinMode(ledPinA6, OUTPUT); 
  pinMode(ledPinA7, OUTPUT); 

  pinMode(ledPinB0, OUTPUT); 
  pinMode(ledPinB1, OUTPUT); 
  pinMode(ledPinB2, OUTPUT); 
  pinMode(ledPinB3, OUTPUT); 
  pinMode(ledPinB4, OUTPUT); 
  pinMode(ledPinB5, OUTPUT); 
  pinMode(ledPinB6, OUTPUT); 
  pinMode(ledPinB7, OUTPUT);

  pinMode(buttonPin1, INPUT_PULLUP);  
  pinMode(buttonPin2, INPUT_PULLUP);  
}

void loop() {

   buttonStateA = digitalRead(buttonPin1);
   buttonStateB = digitalRead(buttonPin2);

   if (permitir == 0 && ganadorA == 0 && ganadorB == 0)
   {
     semaforo();
   }
   else if (permitir == 1 && ganadorA  == 0 && ganadorB == 0)
   {  
      verde ();
      jugador_1(); 
      jugador_1_escoger();
   }
   else {
      if (ganadorA == 1)
      {
        funcion_ganadorA();
      }
   }

}

void semaforo (void){

  if (buttonStateA == 0 || buttonStateB == 0) {
  digitalWrite(LEDR, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LEDG, LOW);    // turn the LED on (HIGH is the voltage level)
  digitalWrite(LEDB, LOW);    // turn the LED on (HIGH is the voltage level)
  delay(1000);    
  digitalWrite(LEDR, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LEDG, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LEDB, LOW);    // turn the LED on (HIGH is the voltage level)
  delay(1000);                // wait for a second
   permitir = 1;
  }
}

void verde (void){
   digitalWrite(LEDR, LOW);    // turn the LED off by making the voltage LOW
   digitalWrite(LEDG, HIGH);   // turn the LED on (HIGH is the voltage level)
   digitalWrite(LEDB, LOW);    // turn the LED on (HIGH is the voltage level)
}

void jugador_1(void) 
{
    
    //if (PORTAbits.RA2 == 1)                     // Verifica que el led ya este en verde
    {
        if (buttonStateA == 0)                 // Verifica que el boton este presionado 
        {
            pressed_ok = pressed_ok + 1;        // Se incrementa contador que verifica que el boton este presionado con rango de seguridad 
            released_ok = 0;                    // Variable de boton libre se reduce a cero porque boton se esta presionando
            if (pressed_ok > 500)               // Si el boton esta seguramente presionado
            {
                if (presionado == 0)            // Verifica que el boton esta en posicion presionado
                {    
                    i = i + 1;                  // Incrementa el entero i para calcular el siguiente valor
                    presionado = 1;             // Coloca el boton como ya presionado para no volver a repetir este ciclo
                    if (i == 8)                 // Si i no llega hasta 8...
                    {
                        //PORTAbits.RA0 = 1;      // El led del ganador no se enciende
                    }
                }
                pressed_ok = 0;                 // Se reduce contador de boton de seguridad presionado para siguiente ciclo
            }
        }
        else                                    // Si el boton no esta presionado
        {
            released_ok = released_ok + 1;      // Se incrementa contador que verifica que el boton este libre con rango de seguridad 
            pressed_ok = 0;                     // Contador de boton presionado se reduce a cero porque boton esta libre
            if (released_ok > 500)              // Verifica que el boton este libre ...
            {
                presionado = 0;                 // Coloca el boton como libre para siguiente ciclo
                released_ok = 0;                // Variable de boton libre se reduce a cero para siguiente ciclo
            }
        }
    }
}

void jugador_1_escoger(void){
   switch (i) {

   case 1:
   digitalWrite(ledPinA0, HIGH);    
   break;

   case 2:
   digitalWrite(ledPinA1, HIGH);   
   digitalWrite(ledPinA0, LOW);   
   break;

   case 3:
   digitalWrite(ledPinA2, HIGH);   
   digitalWrite(ledPinA1, LOW);   
   break;

   case 4:
   digitalWrite(ledPinA3, HIGH);   
   digitalWrite(ledPinA2, LOW);   
   break;

   case 5:
   digitalWrite(ledPinA4, HIGH);   
   digitalWrite(ledPinA3, LOW);   
   break;

   case 6:
   digitalWrite(ledPinA5, HIGH);   
   digitalWrite(ledPinA4, LOW);   
   break;

   case 7:
   digitalWrite(ledPinA6, HIGH);   
   digitalWrite(ledPinA5, LOW);   
   break;

   case 8:
   digitalWrite(ledPinA7, HIGH);   
   digitalWrite(ledPinA6, LOW);   
   ganadorA = 1;
   break;

   default:

   break;

 }  
}

void funcion_ganadorA(void){
   digitalWrite(ledPinA5, HIGH);   
   digitalWrite(ledPinA6, HIGH);    
   digitalWrite(ledPinA7, HIGH);
   digitalWrite(LEDR, HIGH);   // turn the LED on (HIGH is the voltage level)
   digitalWrite(LEDG, LOW);    // turn the LED on (HIGH is the voltage level)
   digitalWrite(LEDB, LOW);    // turn the LED on (HIGH is the voltage level)
   delay(1000);

   digitalWrite(ledPinA5, LOW);   
   digitalWrite(ledPinA6, LOW);    
   digitalWrite(ledPinA7, LOW);
   digitalWrite(LEDR, LOW);   // turn the LED on (HIGH is the voltage level)
   digitalWrite(LEDG, LOW);    // turn the LED on (HIGH is the voltage level)
   digitalWrite(LEDB, LOW);    // turn the LED on (HIGH is the voltage level)
   delay(1000);
 
}
