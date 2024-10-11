// C++ code
//
/*
MAX6675 |	ESP32
GND     |	GND
VCC     |	3.3V
SCK     |	GPIO 5
CS      |	GPIO 23
SO      |	GPIO 19
*/
#include <Servo.h>
////////////////////termopar//////////////////////////////////////
#include "max6675.h"
int thermoDO = 19;
int thermoCS = 23;
int thermoCLK = 5;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
///////////////////pinos///////////////////////////////////////
#define botton 12
#define led1 4
#define led2 5
#define led2 reler
bool estado=0,botaoEstadoAnterior=1;
unsigned long ultimoTempo;
int debounce=1000,leituraBotao;
////////////////////////servo motor/////////////////////////////////////////////
Servo meuServo; // Cria um objeto Servo para controlar o servo motor
int pos; // Declara uma variável para controlar a posição do servo motor 

void setup()
{
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(reler,OUTPUT);
  meuServo.attach(6); // Associa o servo motor ao pino digital 6 do Arduino
  meuServo.write(0);
  
  Serial.begin(9600);
  pinMode(botton, INPUT_PULLUP);
  Serial.println("Iniciando Systema");
  delay(500);
}

void loop()
{
  leituraBotao = digitalRead(botton);
  if(leituraBotao==LOW && botaoEstadoAnterior == HIGH &&(millis()-ultimoTempo)>debounce){
    estado=!estado;
    ultimoTempo=millis();
    
  }
   botaoEstadoAnterior = leituraBotao;  // Atualiza o estado anterior do botão
  switch(estado){
  case 1:
    start();
    break;
     case 0:
    desligar();
    break;
  }
}
void start(){
 digitalWrite(led1,HIGH);
 digitalWrite(led2,LOW); 
    //Serial.println("Ligado");
}
void desligar(){
  //Serial.println("Desligado");
 digitalWrite(reler,HIGH);
 digitalWrite(led2,HIGH);
 digitalWrite(led1,LOW);  
 meuServo.write(0);
}
