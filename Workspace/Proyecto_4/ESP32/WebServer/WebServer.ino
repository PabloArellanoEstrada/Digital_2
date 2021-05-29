/*
  ESP32 Web Server
  Ejemplo de creación de Web server 
  Basándose en los ejemplos de: 
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://electropeak.com/learn
  Proyecto4 de Digital 
  Pablo Arellano, 151379
*/

//------------------------------------------------------------------------------------------------
// Librerías
//------------------------------------------------------------------------------------------------

#include <WiFi.h>                      // Wi-Fi
#include <WebServer.h>                 // WebServer

//------------------------------------------------------------------------------------------------
// Variables globales
//------------------------------------------------------------------------------------------------

const char* ssid = "TURBONETT_21E83C";  // SSID
const char* password = "1D36E8B5D6";    // Password

WebServer server(80);                   // Object of WebServer(puerto HTTP POR DEFAULT)

bool Parqueo1 = LOW;                    // pagina web
bool Parqueo2 = LOW;
bool Parqueo3 = LOW;
bool Parqueo4 = LOW;

uint8_t conteo1 = 0;                    // suma de total 
uint8_t conteo2 = 0;
uint8_t conteo3 = 0;
uint8_t conteo4 = 0;
uint8_t total   = 0;

String uart1 = "0";                     // UART
String uart2 = "0";
String uart3 = "0";
String uart4 = "0";

//------------------------------------------------------------------------------------------------
// Configuración
//------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);                     // BaudRate comunicacion serial               
  Serial.println("Try Connecting to ");
  Serial.println(ssid);

 
  WiFi.begin(ssid, password);               // Conectarse al WIFI
  while (WiFi.status() != WL_CONNECTED) {   // Se chequea si Wi-Fi esta conectado a la red
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());            // Mostrar la direccion IP del ESP32

  server.on("/", handle_OnConnect);          // Conectarse deirectamente a una funcion desde la IP

  server.onNotFound(handle_NotFound);        // Si no encuentra la IP se manda a la funcion error

  server.begin();                            // Inicia el servidor HTTP
  Serial.println("HTTP server started");     
  delay(100);
}

//------------------------------------------------------------------------------------------------
// loop principal
//------------------------------------------------------------------------------------------------

void loop() {
  server.handleClient();                           // Servidor conecta a los clientes
  total = conteo1 + conteo2 + conteo3 + conteo4;

  if (Serial.available()) {                        // si hay caracter en el Rx

   uart1 = Serial.readStringUntil(',');            // lee valor leido hasta que haya una coma
   uart2 = Serial.readStringUntil(',');            // lee valor leido hasta que haya una coma 
   uart3 = Serial.readStringUntil(',');            // lee valor leido hasta que haya una coma 
   uart4 = Serial.readStringUntil(',');            // lee valor leido hasta que haya una coma 
  }

  if (uart1.toInt() == 0){                         // Si es cero, es verde y el conteo 1                
    Parqueo1 = LOW;                                // Si es uno,  es rojo y el conteo 0
    conteo1 = 1;}
  else{
    Parqueo1 = HIGH;
    conteo1 = 0;}

  if (uart2.toInt() == 0){                         // Si es cero, es verde y el conteo 1 
    Parqueo2 = LOW;                                // Si es uno,  es rojo y el conteo 0
    conteo2 = 1;}
  else{
    Parqueo2 = HIGH;
    conteo2 = 0;}

  if (uart3.toInt() == 0){                         // Si es cero, es verde y el conteo 1 
    Parqueo3 = LOW;                                // Si es uno,  es rojo y el conteo 0
    conteo3 = 1;}
  else{
    Parqueo3 = HIGH;
    conteo3 = 0;}

  if (uart4.toInt() == 0){                         // Si es cero, es verde y el conteo 1 
    Parqueo4 = LOW;                                // Si es uno,  es rojo y el conteo 0
    conteo4 = 1;}
  else{
    Parqueo4 = HIGH;
    conteo4 = 0;}

  Serial.print(uart1);                             // Se imprimen en la consola 
  Serial.print(uart2);                             // los valores de los pines
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
  
  String ptr = "<!DOCTYPE html> <html>\n";                              // Pagina Principal, Titulo, Tabla y Total
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

  //--------------------------------------------------------------------------------------------------------
  
  ptr += "\t  <td> <h2> Parqueo1 </h2></td>\n";                          // Parqueo 1                         
  if (parqueo1)                                                          // Si es uno, se coloca rojo
  {                                                                      // Si es cero, se coloca verde
    ptr += "\t  <td  style=\"background-color: #F71919;\"><h2> &#9940</h2></td>\t\n";
  }
  else
  {
    ptr += "\t  <td  style=\"background-color: #1BC50C;\"> <h2> &#9989</h2> </td>\t\n";
  }
  ptr += "\t</tr>\n";
  ptr += "\t<tr>\n";

  //--------------------------------------------------------------------------------------------------------
  
  ptr += "\t  <td><h2> Parqueo2 </h2></td>\n";                           // Parqueo 2
  if (parqueo2)                                                          // Si es uno, se coloca rojo
  {                                                                      // Si es cero, se coloca verde
    ptr += "\t  <td  style=\"background-color: #F71919;\"><h2> &#9940</h2></td>\t\n";
  }
  else
  {
    ptr += "\t  <td  style=\"background-color: #1BC50C;\"> <h2> &#9989</h2> </td>\t\n";
  }
  ptr += "\t</tr>\n";
  ptr += "\t<tr>\n";

  //--------------------------------------------------------------------------------------------------------
  
  ptr += "\t  <td><h2> Parqueo3 </h2></td>\n";                           // Parqueo 3
  if (parqueo3)                                                          // Si es uno, se coloca rojo
  {                                                                      // Si es cero, se coloca verde
    ptr += "\t  <td  style=\"background-color: #F71919;\"><h2> &#9940</h2></td>\t\n";
  }
  else
  {
    ptr += "\t  <td  style=\"background-color: #1BC50C;\"> <h2> &#9989</h2> </td>\t\n";
  }
  ptr += "\t</tr>\n";
  ptr += "\t<tr>\n";

  //--------------------------------------------------------------------------------------------------------
  
  ptr += "\t  <td><h2> Parqueo4 </h2></td>\n";                           // Parqueo 4
  if (parqueo4)                                                          // Si es uno, se coloca rojo
  {                                                                      // Si es cero, se coloca verde
    ptr += "\t  <td  style=\"background-color: #F71919;\"><h2> &#9940</h2></td>\t\n";
  }
  else
  {
    ptr += "\t  <td  style=\"background-color: #1BC50C;\"> <h2> &#9989</h2> </td>\t\n";
  }
  ptr += "\t</tr>\n";
  ptr += "    <tr>\n";
  ptr += "\t  <td><h2> TOTAL </h2></td>\n";

  //--------------------------------------------------------------------------------------------------------

  if (total == 0){                                                       // Valor del total
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

  //--------------------------------------------------------------------------------------------------------
  
  ptr += "<script>\n";                                                    // Actualizacion cada 0.5 segundos
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
// Handler no encontrado
//------------------------------------------------------------------------------------------------

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
