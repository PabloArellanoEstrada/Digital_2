/*
  ESP32 Web Server
  Ejemplo de creación de Web server 
  Basándose en los ejemplos de: 
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://electropeak.com/learn
*/

//------------------------------------------------------------------------------------------------
// Librerías
//------------------------------------------------------------------------------------------------
#include <WiFi.h>
#include <WebServer.h>

//------------------------------------------------------------------------------------------------
// Variables globales
//------------------------------------------------------------------------------------------------
const char* ssid = "TURBONETT_21E83C";  // Enter your SSID here
const char* password = "1D36E8B5D6";    //Enter your Password here

WebServer server(80);                   // Object of WebServer(HTTP port, 80 is defult)

bool Parqueo1 = LOW;                    // pagina web
bool Parqueo2 = LOW;
bool Parqueo3 = LOW;
bool Parqueo4 = LOW;

uint8_t conteo1 = 0;                    // suma de total 
uint8_t conteo2 = 0;
uint8_t conteo3 = 0;
uint8_t conteo4 = 0;
uint8_t total = 0;

String uart1 = "0";                            // uart
String uart2 = "1";
String uart3 = "0";
String uart4 = "1";

//------------------------------------------------------------------------------------------------
// Configuración
//------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("Try Connecting to ");
  Serial.println(ssid);

  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  server.on("/", handle_OnConnect); // Directamente desde e.g. 192.168.0.8

  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}
//------------------------------------------------------------------------------------------------
// loop principal
//------------------------------------------------------------------------------------------------
void loop() {
  server.handleClient();
  total = conteo1 + conteo2 + conteo3 + conteo4;

  if (Serial.available()) {                            // si hay caracter en el Rx

   uart1 = Serial.readStringUntil(',');                // lee valor leido hasta que haya una coma
   //Serial.read(); 
   uart2 = Serial.readStringUntil(',');                // lee valor leido hasta que haya una coma
   //Serial.read(); 
   uart3 = Serial.readStringUntil(',');                // lee valor leido hasta que haya una coma 
   //Serial.read();
   uart4 = Serial.readStringUntil(',');                // lee valor leido hasta que haya una coma 
   //Serial.read();
  }

  if (uart1.toInt() == 0){
    Parqueo1 = LOW;
    conteo1 = 1;}
  else{
    Parqueo1 = HIGH;
    conteo1 = 0;}

  if (uart2.toInt() == 0){
    Parqueo2 = LOW;
    conteo2 = 1;}
  else{
    Parqueo2 = HIGH;
    conteo2 = 0;}

  if (uart3.toInt() == 0){
    Parqueo3 = LOW;
    conteo3 = 1;}
  else{
    Parqueo3 = HIGH;
    conteo3 = 0;}

  if (uart4.toInt() == 0){
    Parqueo4 = LOW;
    conteo4 = 1;}
  else{
    Parqueo4 = HIGH;
    conteo4 = 0;}

  Serial.print(uart1);
  Serial.print(uart2);
  Serial.print(uart3);
  Serial.println(uart4);
}

//------------------------------------------------------------------------------------------------
// Handler de Inicio página
//------------------------------------------------------------------------------------------------
void handle_OnConnect() {
  Serial.println("Funcion exitosa");
  server.send(200, "text/html", SendHTML(Parqueo1, Parqueo2, Parqueo3, Parqueo4));
}

//------------------------------------------------------------------------------------------------
// Procesador de HTML
//------------------------------------------------------------------------------------------------
String SendHTML(uint8_t parqueo1, uint8_t parqueo2 ,uint8_t parqueo3 ,uint8_t parqueo4) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align:center;}\n";
  ptr += "table { width:50%;}}\n";
  ptr += "th, td { padding: 5px; text-align: center;}\n";
  ptr += "#t01 td:nth-child(odd) { background-color: #0E4BDA;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "  <h1>&#128640 ParqueoSpace &#128640</h1>\n";
  ptr += "  <table id=\"t01\" align=\"center\">\n";
  ptr += "\t<tr>\n";

  
  ptr += "\t  <td> <h2> Parqueo1 </h2></td>\n";
  if (parqueo1)
  {
    ptr += "\t  <td  style=\"background-color: #F71919;\"><h2> &#9940</h2></td>\t\n";
  }
  else
  {
    ptr += "\t  <td  style=\"background-color: #1BC50C;\"> <h2> &#9989</h2> </td>\t\n";
  }
  ptr += "\t</tr>\n";
  ptr += "\t<tr>\n";

  
  ptr += "\t  <td><h2> Parqueo2 </h2></td>\n";
  if (parqueo2)
  {
    ptr += "\t  <td  style=\"background-color: #F71919;\"><h2> &#9940</h2></td>\t\n";
  }
  else
  {
    ptr += "\t  <td  style=\"background-color: #1BC50C;\"> <h2> &#9989</h2> </td>\t\n";
  }
  ptr += "\t</tr>\n";
  ptr += "\t<tr>\n";


  ptr += "\t  <td><h2> Parqueo3 </h2></td>\n";
  if (parqueo3)
  {
    ptr += "\t  <td  style=\"background-color: #F71919;\"><h2> &#9940</h2></td>\t\n";
  }
  else
  {
    ptr += "\t  <td  style=\"background-color: #1BC50C;\"> <h2> &#9989</h2> </td>\t\n";
  }
  ptr += "\t</tr>\n";
  ptr += "\t<tr>\n";


  ptr += "\t  <td><h2> Parqueo4 </h2></td>\n";
  if (parqueo4)
  {
    ptr += "\t  <td  style=\"background-color: #F71919;\"><h2> &#9940</h2></td>\t\n";
  }
  else
  {
    ptr += "\t  <td  style=\"background-color: #1BC50C;\"> <h2> &#9989</h2> </td>\t\n";
  }
  ptr += "\t</tr>\n";
  ptr += "    <tr>\n";
  ptr += "\t  <td><h2> TOTAL </h2></td>\n";

  if (total == 0){
    ptr += "\t  <td  style=\"background-color: #FCFF00;\"><h2>0</h2></td>\t\n";
  }
  else if (total == 1){
    ptr += "\t  <td  style=\"background-color: #FCFF00;\"><h2>1</h2></td>\t\n";
  }
  else if (total == 2){
    ptr += "\t  <td  style=\"background-color: #FCFF00;\"><h2>2</h2></td>\t\n";
  }
  else if (total == 3){
    ptr += "\t  <td  style=\"background-color: #FCFF00;\"><h2>3</h2></td>\t\n";
  }
  else if (total == 4){
    ptr += "\t  <td  style=\"background-color: #FCFF00;\"><h2>4</h2></td>\t\n";
  }
  ptr += "\t</tr>\n";
  
  ptr += "<script>\n";  
  ptr += "<!--\n";
  ptr += "function timedRefresh(timeoutPeriod) {\n";
  ptr += "\tsetTimeout(\"location.reload(true);\",timeoutPeriod);\n";
  ptr += "}\n";
  ptr += "\n";
  ptr += "window.onload = timedRefresh(500);\n";
  ptr += "\n";
  ptr += "//   -->\n";
  ptr += "</script>";

  ptr += "</table>\n";
  ptr += "\t</body>\n";
  ptr += "</html>"; 
  return ptr;
  
}

//------------------------------------------------------------------------------------------------
// Handler de not found
//------------------------------------------------------------------------------------------------
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
