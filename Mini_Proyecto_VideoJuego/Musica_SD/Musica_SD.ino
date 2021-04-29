
// ______________________________________________________________________________________________________________________________________

String fruit_name;                                         // Variables
String myFiles[15];
int cadena = 0;
int i3 = 0;

#include <SPI.h>                                           // Librerias
#include <SD.h>

File root;                                                 // Recorre lista de archivos en un directorio especificado en el Loop
File entry;                                                // Recorre lista de archivos en un directorio especificado dentro de funcion especifica
File dibujar;                                              // Para dibujar lo que esta en el archivo



// ________________________________________________________________________________________________________________________________________

void setup()
{
   Serial.begin(9600);                                     // 1. Empezar comunicacion serial y esperar a que el puerto se abra  
   SPI.setModule(0);                                       // 2. Empezar modulo SPI-0

   Serial.print("Initializing SD card...");                // 3. Inicializar SD
   pinMode(PA_3, OUTPUT);                                  //    Salida por default para que libreria SD funcione

   if (!SD.begin(PA_3))                                    //    No se ha iniciado la comunicacion al mandar el SS?
   {
      Serial.println("initialization failed!");            //    Fallo la inicializacion
      return;
   }
   Serial.println("initialization done.");                 //    Ya se inicializo

   root = SD.open("/");                                    // 4. Abrir la memoria SD en la ubicacion principal
   printDirectory(root, 0);                                // 5. Funcion para listar los archivos con directorio
   entry.close();                                          //    ** Para inicializar contadores

   
}

// ________________________________________________________________________________________________________________________________________

void loop()
{
 if (Serial.available())                                 // Hay caracter en el MISO?
 {
      graficar();
 }

   
}

// _____________________________________________________________________________________________________________________________________

void printDirectory(File dir, int numTabs) 
{
   dir.seek(0);                                           // Busca una nueva posicion dentro del archivo, en este caso la posicion inicial                                     
   cadena = 0;                                            // SD.h regresa el puntero a la posicion inicial dentro del archivo, pero no al inicio del SD 
   while(true) 
   {
      entry =  dir.openNextFile();                        // Indicar el proximo archivo dentro de la carpeta
     
      if (! entry)                                        // Hay mas archivos?
      {
          break;                                          // Termina al no haber mas archivos
      }
                
      if (entry.isDirectory())                            // Hay mas archivos en el directorio?
      {
          printDirectory(entry, numTabs+1);               // Se imprime serialmente cada archivo y directorio presente
      } 
      else 
      {                                   
          myFiles[cadena] = entry.name();                 // Se almacena en cadena tipo String los nombres de los archivos
          cadena = cadena + 1;                            // Contador para recorrer la cadena
      }
      
      entry.close();                                      // Se cierra el archivo
   }
}

// ______________________________________________________________________________________________________________________________________


void graficar(void)
{
    fruit_name = Serial.readStringUntil('\n');           // Lee valor hasta que haya una enter
    int i3 = fruit_name.toInt();                               // Pasar valor leido del Serial-Monitor a entero  
    String temp = myFiles[i3 + 1];                             // Guardar en String el nombre del archivo seleccionado 
    
    char __numeroFruta[sizeof(temp)];                          // Determinar tamaño de nombre de archivo 
    temp.toCharArray(__numeroFruta, sizeof(__numeroFruta));    // Guardar String en arreglo de Chars para que pueda ser variable leida
                                                  
    dibujar = SD.open(__numeroFruta);                             // Abrir el archivo a leer
    if (dibujar)                                                  // Existe algo en el archivo?
    {
       while (dibujar.available())                                // Leer el archivo hasta que haya terminado de leer
       {                     
           Serial.write(dibujar.read());
           
       }
       dibujar.close();                                           // Cierra el archivo
    } 
    else 
    {
       Serial.println("");
       Serial.println("Error al abrir. Selecciona otra opcion");  // Error al abrir si no se pudo abrir
    }
    dibujar.close();                                              // Cierra el archivo
}
