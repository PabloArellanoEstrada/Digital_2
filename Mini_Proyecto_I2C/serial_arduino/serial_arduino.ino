

void setup() {
  Serial.begin(9600);
}

void loop() {
  
 
  if (Serial.available()) {  // si hay caracter
    int inByte = Serial.read();  // obtener caracter
    //Serial.write(inByte);
    Serial.print(inByte);
    Serial.println(inByte);
  }

 
}
