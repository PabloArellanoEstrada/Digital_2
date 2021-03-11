// Adafruit IO Publish Example

// Variables ----------------------------------------------------------

AdafruitIO_Feed *Az_feed = io.feed("Az");         // Medidor de salida
int Ax = 0;
int Ay = 0;
int Az = 0;
String var1;
String var2;
String var3;

// Setup --------------------------------------------------------------

void setup() {

  Serial.begin(9600);                             // 1. Empezar comunicacion serial

  while(! Serial);                                // 2. Esperar a que el monitor se abra

  io.connect();                                   // 3. Se conecta a io.adafruit.com

  while(io.status() < AIO_CONNECTED) {            // 4. Se espera conexion y se imprimen puntos de prueba
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println(io.statusText());                // 5. Conexion exitosa y se envian datos de Feeds
}

// Loop  --------------------------------------------------------------

void loop() 
{
  if (Serial.available()) {                        // si hay caracter?
    Az = Serial.parseInt();                        // Se lee en entero del Rx
  }
  Az = Az / 405;                                   // Conversion a sistema metrico
  
  io.run();                                        // conexion con Adaruit.com

  Serial.print("  Az = ");                         // Se imprime valor leido de UART en monitor
  Serial.println(Az);

  Az_feed->save(Az);                               // Se envia valor a Adafruit.com

  delay(3000);                                     // Delay para evitar mas de 30 eventos por minuto
} 
