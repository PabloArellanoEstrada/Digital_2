

void setup() {
  Serial.begin(9600);
}

void loop() {
  
 
  if (Serial.available()) {  // si hay caracter
    char inByte = Serial.read();  // obtener caracter
    //Serial.write(inByte);
    //Serial.print(inByte);
    Serial.print(inByte);
    
  }

 
}
