/*
 *Práctica 3, se realizó con un registro de desplazamiento de 8 bits (CD4094)
 *Se tienen 6 secuencias, dependiendo el valor que indique el Potenciometro, son las siguientes:
 *                    LEDs -->     S7|S6|S5|S4|S3|S2|S1
 *               Qs CD4094 -->  Q8|Q7|Q6|Q5|Q4|Q3|Q2|Q1       Valor en decimales
 *0 -> Valor <= 1,000     ->    0 |0 |0 |0 |0 |0 |0 |0    =   0
 *1 -> Valor[1,000;1,500) ->    0 |1 |0 |1 |1 |1 |0 |1    =   93
 *2 -> Valor[1,500;2,000) ->    0 |0 |0 |1 |1 |1 |0 |0    =   28  ++
 *3 -> Valor[2,000;2,500) ->    0 |1 |1 |1 |1 |1 |1 |1    =   127
 *4 -> Valor[2,500;3,000) ->    0 |1 |1 |1 |1 |1 |1 |0    =   126
 *5 -> Valor >= 3,000     ->    0 |0 |0 |1 |1 |1 |0 |0    =   28  ++
 *                                ++ Valores Iguales
 */

const int pote = 2;         //Pin GPIO2 para el Potenciometro
const int pinData = 14;     //Pin GPIO14 para el Pin DATA del CD4094
const int pinClock = 15;    //Pin GPIO15 para el Pin CLOCK del CD4094
const int pinStrobe = 13;   //Pin GPIO13 para el Pin STROBE del CD4094
const int seg = 1000;       //Segundo para el retardo
                   //0 |1 |2 |3 | 4
int secuencias[5] = {0,93,28,127,126};  //Arreglo de las secuencias (Ver tabla de arriba)

void setup() {

  pinMode(pote,INPUT);        //Potenciometro como Entrada
  pinMode(pinData,OUTPUT);    //DATA como Salida
  pinMode(pinClock,OUTPUT);   //CLOCK como Salida
  pinMode(pinStrobe,OUTPUT);  //STROBE como Salida
  
  Serial.begin(115200);       //Velocidad de Baudios por default de la placa ESP32-CAM

}

void loop() {
  
  int s = 0;    //Variable Auxiliar para escoger entre las secuencias
  
  float valorPote = analogRead(pote);   //Valor que se recibe del Potenciometro

  float valor = ( valorPote / 1023 ) * 3300L;   //Valor del Potenciometro luego de ser sometido a esta operación
                                      //La 'L' es para forzar el número a un valor long
  Serial.println(valor);    //Mandar al Monitor Serial la variable 'valor'

  if(valor >= 1000 && valor < 1500){          //Primer Sentencia valor[1,000;1,500)
    s = secuencias[1];                        //<--
    digitalWrite(pinStrobe,LOW);              //Se apaga el STROBE
    shiftOut(pinData, pinClock, MSBFIRST, s); //shiftOut desplaza la salida de un byte un bit a la vez
    delay(seg);                               //Retardo de un segundo
    digitalWrite(pinStrobe,HIGH);             //Se enciende el STROBE y se manda este byte de información
  }
  else if(valor >= 1500 && valor < 2000){     //Segunda Sentencia valor[1,500;2,000)
    s = secuencias[2];                        //<--
    digitalWrite(pinStrobe,LOW);
    shiftOut(pinData, pinClock, MSBFIRST, s); //Explicación de pinData: Es el pin por donde sale cada bit (int)
    delay(seg);
    digitalWrite(pinStrobe,HIGH);
  }
  else if(valor >= 2000 && valor < 2500){     //Tercera Sentencia valor[2,000;2,500)
    s = secuencias[3];                        //<--
    digitalWrite(pinStrobe,LOW);
    shiftOut(pinData, pinClock, MSBFIRST, s); //Explicación de pinClock: Es el pin que cambia cada vez que pinData
    delay(seg);                               //ha sido establecido a su valor correcto (int)
    digitalWrite(pinStrobe,HIGH);
  }
  else if(valor >= 2500 && valor < 3000){     //Cuarta Sentencia valor[2,500;3,000)
    s = secuencias[4];                        //<--
    digitalWrite(pinStrobe,LOW);
    shiftOut(pinData, pinClock, MSBFIRST, s); //Explicación de MSBFIRST: Dirección de desplazamiento de los bits
    delay(seg);                               //En español significa Primero el Bit Más Significativo
    digitalWrite(pinStrobe,HIGH);
  }
  else if(valor >= 3000){                     //Quinta Sentencia valor >= 3,000
    s = secuencias[2];                        //<--
    digitalWrite(pinStrobe,LOW);
    shiftOut(pinData, pinClock, MSBFIRST, s); //También existe LSBFIRST, que significa: Primero el Bit Menos Significativo
    delay(seg);
    digitalWrite(pinStrobe,HIGH);
  }
  else {                                      //Sentencia 0 valor = 0
    s = secuencias[0];                        //<--
    digitalWrite(pinStrobe,LOW);
    shiftOut(pinData, pinClock, MSBFIRST, s); //La 's' representa el valor de los datos que se desplazan a la salida (byte)
    delay(seg);
    digitalWrite(pinStrobe,HIGH);
  }

}
/*
 * Oscar Isaac Tinajero Maldonado
 */
