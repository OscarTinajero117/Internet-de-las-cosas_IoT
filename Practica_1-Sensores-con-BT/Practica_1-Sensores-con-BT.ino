#include<LiquidCrystal_I2C.h> //Librería para hacer uso del LCD con la Interface

LiquidCrystal_I2C lcd (0x27, 16, 2); //LCD

int BT = 0;
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
float temp;
int const led = 4;
int estadoLed = 0;

//***************/LED & LM35***************//

void setup() {
  Serial.begin(9600);

  pinMode(echo,INPUT);
  pinMode(trigger,OUTPUT);
  pinMode(led,OUTPUT);
  
  //**************LCD**************//

  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  //**************/LCD**************//
}//setup

void loop() {
  lcd.clear();
  
  pulsoTrigger();
  distSU = obtEcho();

  temp = temperatura();
  
  //Distancia
  lcd.setCursor(0,0);
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
    BT = Serial.read();
  }
  //ON
  if(BT =='A'){
    digitalWrite(led, HIGH);
  }
  if(BT == 'B'){
    digitalWrite(led,LOW);
  }

  bluetooth(distSU,temp);
  
  delay(800);
}//loop

//*******************BT*********************//
void bluetooth(long ultra, float stLM35){
  Serial.print(stLM35);
  Serial.print(String("|"));
  Serial.print(ultra);
  Serial.print(String("|"));
}
//*******************/BT*********************//

//***************Funciones US***************//

void pulsoTrigger() {
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH); //Generación de pulso en el trigger por 10ms
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
}//pulsoTrigger

long obtEcho() {
  long duracion, distancia;
  duracion = pulseIn(echo, HIGH);
  distancia = (duracion / 2) / 29;

  return distancia;
}//obtEcho

//***************/Funciones US***************//
//***************Funcion LM35***************//

float temperatura(){
  int valor = analogRead(lm35);
  float grados = (float(valor) / 1023) * 500.0;
  return grados;
}

//***************/Funcion LM35***************//
