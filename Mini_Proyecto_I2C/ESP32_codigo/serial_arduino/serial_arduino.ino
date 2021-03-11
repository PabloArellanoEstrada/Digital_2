
// Variables ----------------------------------------------------------

int Ax = 0;
int Ay = 0;
int Az = 0;
String var1;
String var2;
String var3;

// Setup --------------------------------------------------------------

void setup() 
{
  Serial.begin(9600);                                 // Empezar comunicacion serial
}

// Loop  ---------------------------------------------------------------

void loop() {
   
  if (Serial.available()) {                           // si hay caracter en el Rx

   var1 = Serial.readStringUntil(',');                // lee valor leido hasta que haya una coma
   Serial.read(); 
   var2 = Serial.readStringUntil(',');                // lee valor leido hasta que haya una coma
   Serial.read(); 
   var3 = Serial.readStringUntil(',');                // lee valor leido hasta que haya una coma 
   Serial.read();
   
   Ax = var1.toInt()/405;                             // Conversion de datos
   Ay = var2.toInt()/405;
   Az = var3.toInt()/405;

   Serial.print("Ax = ");                             // Imprime valores en el monitor
   Serial.print(Ax);
   Serial.print("  Ay = ");
   Serial.print(Ay);
   Serial.print("  Az = ");
   Serial.println(Az);
    } 
}
