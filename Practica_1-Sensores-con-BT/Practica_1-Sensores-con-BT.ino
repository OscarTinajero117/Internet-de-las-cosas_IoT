#include<LiquidCrystal_I2C.h> //Librería para hacer uso del LCD con la Interface

LiquidCrystal_I2C lcd (0x27, 16, 2); //LCD

int BT = 0;                      //Variable para recibir los estados del Bluetooth
//***************SU***************//

//ECHO
int const echo = 2;
//TRIGGER
int const trigger = 3;
//DISTANCIA
long distSU;

//***************/SU***************//
//***************LED & LM35***************//

int const lm35 = A0;
float temp;                      //Variable para recibir la temperatura
int const led = 4;               //Puerto del LED
int estadoLed = 0;               //Estado inicial del LED

//***************/LED & LM35***************//

void setup() {
  //Inicializar el Serial a 9600 Baudios
  Serial.begin(9600);

  pinMode(echo,INPUT);            //Echo de entrada
  pinMode(trigger,OUTPUT);        //Disparo de Salida
  pinMode(led,OUTPUT);            //Led de salida
  
  //**************LCD**************//
  //Variables para el control del Display con I2C
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  //**************/LCD**************//
}//setup

void loop() {
  
  lcd.clear();                     //Se limpia el Display
  
  pulsoTrigger();                  //Se manda un pulso
  distSU = obtEcho();              //Variable que almacena la distancia

  temp = temperatura();            //Variable que almacena la temperatura
  
  //Distancia
  lcd.setCursor(0,0);              //Posicionar el cursor en 0,0 (el display va de 0,0 a 15,1)
  lcd.print("Distancia:");
  lcd.setCursor(10,0);
  lcd.print(distSU);
  lcd.print("cm");

  //Temperatura
  lcd.setCursor(0,1);
  lcd.print("Temp:");
  lcd.print(temp);
  lcd.print(" C");

  if(Serial.available()>0){
    BT = Serial.read();             //Si el BT recibe un dato, se almacena en esta variable
  }
  //ON-LED
  if(BT =='A'){                     //Si el estado del BT cambia a 'A' enciende el LED
    digitalWrite(led, HIGH);
  }
  //OFF-LED
  if(BT == 'B'){                    //Si el estado del BT cambia a 'B' apaga el LED
    digitalWrite(led,LOW);
  }

  bluetooth(distSU,temp);           //Funcion que manda los datos de los sensores al arduino
  
  delay(800);                       //Tiempo de Espera
}//loop

//*******************BT*********************//
//Funcion de escritura del BT, recibe las variables de los sensores y las manda a la App
void bluetooth(long ultra, float stLM35){
  Serial.print(stLM35);
  Serial.print(String("|"));        //Marca para hacer el Salto de línea
  Serial.print(ultra);
  Serial.print(String("|"));
}
//*******************/BT*********************//

//***************Funciones US***************//
//Función para obtener un pulso
void pulsoTrigger() {
  digitalWrite(trigger, LOW);       //Se apaga el Trigger por 2ms
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);      //Generación de pulso en el trigger por 10ms
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);       //Se apaga el Trigger
}//pulsoTrigger

//Función para captar un pulso a travez del echo
long obtEcho() {
  long duracion, distancia;         //Variables auxiliares
  duracion = pulseIn(echo, HIGH);   //La duración es igual al tiempo que tarda en llegar el pulso
  distancia = (duracion / 2) / 29;  //La distancia es igual a la mitad de la duracion entre 29

  return distancia;                 //Se retorna la distancia
}//obtEcho

//***************/Funciones US***************//
//***************Funcion LM35***************//
//Obtener la Temperatura
float temperatura(){
  int valor = analogRead(lm35);
  float grados = (float(valor) / 1023) * 500.0;
  return grados;
}

//***************/Funcion LM35***************//
