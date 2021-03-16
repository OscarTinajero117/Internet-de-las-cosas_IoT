#include<LiquidCrystal_I2C.h> //Librería para hacer uso del LCD con la Interface

LiquidCrystal_I2C lcd (0x27, 16, 2); //LCD


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
  
  Serial.println(temp);
  delay(700);
}//loop

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
