
// ______________________________________________________________________________________________________________________________________

void menu (void)
{
   Serial.println(" ");
   Serial.println(" ");
   Serial.println(" ");
   Serial.println(" -------------------------- MENU PRINCIPAL ----------------------------");
   Serial.println(" ");
}

// ______________________________________________________________________________________________________________________________________

void lista (void)
{
   int limite = cadena - 2;                                    // Limite de cantidad de archivos                              
   for (int i2 = 0; i2 < limite;i2 ++ )                        // De cero a cantidad de archivos
   {
      Serial.print(" ");
      Serial.print(i2+1);                                      // Imprime 1, 2, 3, ...
      Serial.print(". ");
      Serial.println(myFiles[i2 + 2]);                         // Imprime nombre del archivo almacenado en cadena
   }
}

// ______________________________________________________________________________________________________________________________________

void graficar(void)
{
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
