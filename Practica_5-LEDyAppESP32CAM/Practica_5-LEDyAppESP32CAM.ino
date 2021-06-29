//Librería para WiFi
#include <WiFi.h>

// Credenciales para la conexión a una Red
const char* ssid = "ARRIS-BF02";
const char* password = "F96BB15D4E937789";

// # de puerto (80)
WiFiServer server(80);

//Configuración de IP estática:
IPAddress local_IP(192, 168, 0, 20);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

// Variable para obtener el final del URL
String peticionHTML;

//LED para la práctica:
const int led = 2;

//Propiedades para PWM
int freq = 5000; //Frecuencia Hz
int ledChannel = 0; //Canal
int resolution = 8; //Resolucion 8bits

//Brillo del LED
int brillo;


// Tiempo a transcurrir
unsigned long currentTime = millis();
// Tiempo anterior
unsigned long previousTime = 0;
// Se define el tiempo de salida en milisegundos (ejemplo: 2000ms = 2s)
const long timeoutTime = 2000;

//Inicia el SETUP
void setup() {
  //Se inicia el Serial
  Serial.begin(115200);
  //Puerto a utilizar con ledcSetup & ledcAttachPin
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(led, ledChannel);

  // Establecimiento de la IP estática.
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Fallo en la configuración.");
  }

  // Conecta a la red wifi.
  Serial.println("*************");
  Serial.print("Conectando con ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conexion Aceptada.");

  // Inicio del Servidor web.
  server.begin();
  Serial.println("Servidor web iniciado.");

  // Esta es la IP
  Serial.print("Direccion IP --> ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}//Fin del SETUP

void loop() {
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

            // Apagado/Encendido de LEDs (puertos)
            if (peticionHTML.indexOf("ON25") >= 0) {
              Serial.println("Puerto GPIO 2 25%");
              brillo = pwmPorcentaje(25);
              ledcWrite(ledChannel, brillo);
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              client.println("<!DOCTYPE HTML>");
              client.println("<html>");
              client.println("Encendido GPIO 2 al 25% confirmado");
              client.println("</html>");
              client.stop(); //Cerramos la conexión
              delay(1); //Ponemos un retardo

            } else if (peticionHTML.indexOf("ON50") >= 0) {
              Serial.println("Puerto GPIO 2 50%");
              brillo = pwmPorcentaje(50);
              ledcWrite(ledChannel, brillo);
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              client.println("<!DOCTYPE HTML>");
              client.println("<html>");
              client.println("Encendido GPIO 2 al 50% confirmado");
              client.println("</html>");
              client.stop(); //Cerramos la conexión
              delay(1); //Ponemos un retardo

            } else if (peticionHTML.indexOf("ON75") >= 0) {
              Serial.println("Puerto GPIO 2 75%");
              brillo = pwmPorcentaje(75);
              ledcWrite(ledChannel, brillo);
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              client.println("<!DOCTYPE HTML>");
              client.println("<html>");
              client.println("Encendido GPIO 2 al 75% confirmado");
              client.println("</html>");
              client.stop(); //Cerramos la conexión
              delay(1); //Ponemos un retardo

            } else if (peticionHTML.indexOf("ON100") >= 0) {
              Serial.println("Puerto GPIO 2 100%");
              brillo = pwmPorcentaje(100);
              ledcWrite(ledChannel, brillo);
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              client.println("<!DOCTYPE HTML>");
              client.println("<html>");
              client.println("Encendido GPIO 2 al 100% confirmado");
              client.println("</html>");
              client.stop(); //Cerramos la conexión
              delay(1); //Ponemos un retardo

            } else if (peticionHTML.indexOf("OFF2") >= 0) {
              Serial.println("Puerto GPIO 2 off");
              brillo = pwmPorcentaje(0);
              ledcWrite(ledChannel, brillo);
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              client.println("<!DOCTYPE HTML>");
              client.println("<html>");
              client.println("Apagado GPIO 2 confirmado");
              client.println("</html>");
              client.stop(); //Cerramos la conexión
              delay(1); //Ponemos un retardo

            }

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
    
    Serial.println("Cliente desconectado.");
    Serial.println("");
  }
}//Fin del LOOP

int pwmPorcentaje(int valor) {
  valor = (valor * 255) / 100;
  return valor;
}
