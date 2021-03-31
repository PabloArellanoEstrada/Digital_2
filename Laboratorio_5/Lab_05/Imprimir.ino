
void menu (void)
{
   Serial.println(" ");
   Serial.println(" ");
   Serial.println(" ");
   Serial.println(" -------------------------- MENU PRINCIPAL ----------------------------");
}



void lista (void)
{
   int limite = cadena - 2;
   for (int i2 = 0; i2 < limite;i2 ++ )
   {
      Serial.print(" ");
      Serial.print(i2);
      Serial.print(". ");
      Serial.println(myFiles[i2 + 2]);
   }
}

void graficar(void)
{
    int i3 = my_name.toInt();
                                        
    String temp = myFiles[i3 + 2];

    char __dataFileName[sizeof(temp)];
    temp.toCharArray(__dataFileName, sizeof(__dataFileName));
                                                  
    root = SD.open(__dataFileName);                    // re-open the file for reading:
    if (root) 
    {
       while (root.available())                    // read from the file until there's nothing else in it:
       {                     
           Serial.write(root.read());
       }
       root.close();                               // close the file:
    } 
    else 
    {
       Serial.println("error opening file");       // if the file didn't open, print an error:
    }
    root.close();                               // close the file:
}
