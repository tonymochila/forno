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
//////////////////////////////////////////////////////////
#include "max6675.h"
int thermoDO = 19;
int thermoCS = 23;
int thermoCLK = 5;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
//////////////////////////////////////////////////////////
#define botton 12
bool estado=0,botaoEstadoAnterior=1;
unsigned long ultimoTempo;
int debounce=1000,leituraBotao;

Servo meuServo; // Cria um objeto Servo para controlar o servo motor
int pos; // Declara uma variável para controlar a posição do servo motor 

void setup()
{
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
    //Serial.println("Ligado");
}
void desligar(){
 
  //Serial.println("Desligado");

  meuServo.write(0);
}