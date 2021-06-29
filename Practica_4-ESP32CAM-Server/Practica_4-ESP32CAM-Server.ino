/*********
  Basado en el código de:
  Rui Santos
  Te recomiendo su página: http://randomnerdtutorials.com  
*********/

// Librería para usar el WiFi
#include <WiFi.h>

// Credenciales para la conexión a una Red
const char* ssid = "NombreDeRed";
const char* password = "ContraseñaDeRed";

// # de puerto (80)
WiFiServer server(80);

// Variable para obtener el final del URL
String peticionHTML; 

// Variables auxiliares para cambiar el estado de los puertos
String salidaVerde = "off"; 
String salidaBlanco = "off"; 

// Puertos Asignados para los LEDs
const int verde = 2;  
const int blanco = 14; 

// Tiempo a transcurrir
unsigned long currentTime = millis();
// Tiempo anterior
unsigned long previousTime = 0; 
// Se define el tiempo de salida en milisegundos (ejemplo: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Se inicializan las salidas
  pinMode(verde, OUTPUT);
  pinMode(blanco, OUTPUT);
  // Se apagan los puertos de salida
  digitalWrite(verde, LOW);
  digitalWrite(blanco, LOW);

  // Conexión a la Red WiFi
  Serial.print("Conectando a: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Se imprime la IP
  Serial.println("");
  Serial.println("Conexion Aceptada.");
  Serial.print("Direccion IP: -> ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listo para recibir clientes (usuarios)

  if (client) {                             // Si hay usuarios, comienza
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("Nuevo Cliente/Usuario.");          // Bandera - Hay un usuario
    String currentLine = "";                // Variable auxiliar para recibir cadenas del cliente
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // Ciclo para realizar tareas mientras el cliente este conectado
      currentTime = millis();
      if (client.available()) {             // Si se lee algo del Cliente/Usuario
        char c = client.read();             // Variable Auxiliar para leer los bytes
        Serial.write(c);                    // Se imprime el contenido de 'c' en el monitor serial
        peticionHTML += c;
        if (c == '\n') {                    // Si el byte es un salto de línea, entra en la siguiente sentencia
          
          if (currentLine.length() == 0) { // Si la longitud de la línea es igual a 0, entra en la siguiente sentencia
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Apagado/Encendido de LEDs (puertos)
            if (peticionHTML.indexOf("GET /verde/on") >= 0) {
              Serial.println("Puerto GPIO 2 on");
              salidaVerde = "on";
              digitalWrite(verde, HIGH);
            } else if (peticionHTML.indexOf("GET /verde/off") >= 0) {
              Serial.println("Puerto GPIO 2 off");
              salidaVerde = "off";
              digitalWrite(verde, LOW);
              
            } else if (peticionHTML.indexOf("GET /blanco/on") >= 0) {
              Serial.println("Puerto GPIO 14 on");
              salidaBlanco = "on";
              digitalWrite(blanco, HIGH);
            } else if (peticionHTML.indexOf("GET /blanco/off") >= 0) {
              Serial.println("Puerto GPIO 14 off");
              salidaBlanco = "off";
              digitalWrite(blanco, LOW);
            }
            
            // Inicio de la página web
            client.println("<!DOCTYPE html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // Estilos CSS
            client.println("<style>html { font-family: Arial; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Título de la página Web
            client.println("<body><h1>ESP32-CAM LEDs</h1>");
            
            // control de botones para la tarea de la práctica -> GPIO 2
            client.println("<p>LED Verde - Estado: " + salidaVerde + "</p>");
            // Si el LED verde esta apagado, saldrá ON      
            if (salidaVerde=="off") {
              client.println("<p><a href=\"/verde/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/verde/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // control de botones para la tarea de la práctica -> GPIO 2 
            client.println("<p>LED Blanco - Estado: " + salidaBlanco + "</p>");
                
            if (salidaBlanco=="off") {
              client.println("<p><a href=\"/blanco/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/blanco/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
            // Se manda un espacio para finalizar la página web
            client.println();
            //Se "salta" (rompe) el ciclo while
            break;
          } else { // Si se manda una nueva linea, le limpia la variable
            currentLine = "";
          }
        } else if (c != '\r') {  // Si se recibe algo diferente de un caracter, se retorna
          currentLine += c;      // y se añade
        }
      }
    }
    // Se limpia la peticion
    peticionHTML = "";
    // Se cierra la conexión
    client.stop();
    Serial.println("Cliente desconectado.");
    Serial.println("");
  }
}
