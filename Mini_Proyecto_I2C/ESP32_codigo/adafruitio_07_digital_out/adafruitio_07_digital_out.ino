// Adafruit IO Digital Output Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-digital-output

// Variables ----------------------------------------------------------

AdafruitIO_Feed *Led_1Feed = io.feed("Led_1");          // Boton 1
AdafruitIO_Feed *Led_2Feed = io.feed("Led_2");          // Boton 2
byte opcion;
byte opcion2;
int led2 = 2;
int led15 = 15;

// Setup --------------------------------------------------------------

void setup() {
  
  pinMode(LED_PIN, OUTPUT);                             // 1. Puertos
  pinMode(led2, OUTPUT);
  pinMode(led15, OUTPUT);
  
  Serial.begin(9600);                                   // 2. Empezar comunicacion serial

  while(! Serial);                                      // 3. Esperar a que el monitor se abra

  Serial.print("Connecting to Adafruit IO");            // 4. Se conecta a io.adafruit.com
  io.connect();

  Led_1Feed->onMessage(handleMessage);                  // 5. Funciones que reciben feeds y se llama cada vez que hay un cambio 
  Led_2Feed->onMessage(handleMessage2);

  while(io.status() < AIO_CONNECTED)                    // 6. Se espera conexion y se imprimen puntos de prueba
    {                  
    Serial.print(".");
    delay(500);
    }

  Serial.println();                                     // 7. Conexion exitosa y se obtienen datos de Feeds
  Serial.println(io.statusText());
  Led_1Feed->get();
  Led_2Feed->get();
}

// Loop  ----------------------------------------------------------------

void loop() {
  io.run();                                             // conexion con Adaruit.com
}

// Funciones 1  ----------------------------------------------------------

void handleMessage(AdafruitIO_Data *data) {             // Funcion cada vez que hay un cambio en los botones

  Serial.print("received <- ");                         // Imprime valores de Adafruit.com
  Serial.println(data->value());

  if(data->toString() == "ON")                          // Si boton uno esta en alto
  {
    digitalWrite(LED_PIN, HIGH);                        // Se imprime High
    digitalWrite(led2, HIGH);                           // Se pone el Pin2 en High (Tx no posible)
    opcion = B00000001;                                 // Se imprime uno
  }
  else
  {
    digitalWrite(LED_PIN, LOW);                         // Se imprime Low
    digitalWrite(led2, LOW);                            // Se pone el Pin2 en Low (Tx no posible)
    opcion = B00000000;                                 // Se imprime cero
  }
  Serial.print ("Led_1 = ");                            // Se imprime en monitor valor de led
  Serial.println (opcion);
}

// Funciones 2  ----------------------------------------------------------

void handleMessage2(AdafruitIO_Data *data) {            // Funcion cada vez que hay un cambio en los botones

  Serial.print("received <- ");                         // Imprime valores de Adafruit.com
  Serial.println(data->value());

  if(data->toString() == "ON")                          // Si boton uno esta en alto
  {
    digitalWrite(LED_PIN, HIGH);                        // Se imprime High
    digitalWrite(led15, HIGH);                          // Se pone el Pin15 en High (Rx no posible)
    opcion2 = B00000001;                                // Se imprime uno
  }
  else
  {
    digitalWrite(LED_PIN, LOW);                         // Se imprime Low
    digitalWrite(led15, LOW);                           // Se pone el Pin15 en Low (Rx no posible)
    opcion2 = B00000000;                                // Se imprime cero
  }
  Serial.print ("Led_2 = ");                            // Se imprime en monitor valor de led
  Serial.println (opcion2);
}
