int salida = 4;             //Pin 4 -> Flash

void setup() {
  pinMode(salida,OUTPUT);   //Flash como salida
}//setup

void loop() {
  digitalWrite(salida, HIGH); //Encender
  delay(1000);                //1 Segundo de espera
  digitalWrite(salida, LOW);  //Apagar
  delay(1000);                //1 Segundo de espera
}//loop
