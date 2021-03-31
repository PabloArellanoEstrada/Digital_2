
// ______________________________________________________________________________________________________________________________________

String my_name;
uint8_t i=0;
uint8_t a=0; 
int cadena = 0;
char rx_byte = 0;
String myFiles[10];


#include <SPI.h>
#include <SD.h>

File root;
File entry;

// ________________________________________________________________________________________________________________________________________

void setup()
{
  Serial.begin(9600);                        // Open serial communications and wait for port to open
  SPI.setModule(0);

  Serial.print("Initializing SD card...");
  pinMode(PA_3, OUTPUT);                     // It's set as an output by default, must be left as an output, or the SD library functions will not work.

  if (!SD.begin(PA_3)) 
  {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

}

// ________________________________________________________________________________________________________________________________________

void loop()
{
     
   if (Serial.available())                         // is a character available?
   {                       
      my_name = Serial.readStringUntil('\n');
      //Serial.println(my_name);        
      menu();                                   
      root = SD.open("/");
      printDirectory(root, 0);
      entry.close();
      graficar();
   }  
}

// _____________________________________________________________________________________________________________________________________

void printDirectory(File dir, int numTabs) 
{
   dir.seek(0); 
   a = 1;
   cadena = 0;
   while(true) 
   {
      entry =  dir.openNextFile();
     
      if (! entry) 
      {
          break;         // no more files
      }
      
      for (uint8_t i=0; i<numTabs; i++) 
      {
         Serial.print('\t');
      }
            
      if (entry.isDirectory()) 
      {
          printDirectory(entry, numTabs+1);
      } 
      else 
      {
          a = a + 1;
          myFiles[cadena] = entry.name();
          cadena = cadena + 1;
      }
      entry.close();
   }
   lista();
}
