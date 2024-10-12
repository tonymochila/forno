// C++ code
//
/*
MAX6675 |  ESP32
GND     | GND
VCC     | 3.3V
SCK     | GPIO 5
CS      | GPIO 23
SO      | GPIO 19
*/
/////////////////////////////bluetooh////////////////////////////////////
#include "BluetoothSerial.h"

String device_name = "Forno Semi-automatico";

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;
////////////////////////////////////////////////////////////////////
#include <ESP32Servo.h>

static const int servoPin = 32;

Servo servo1;
////////////////////termopar//////////////////////////////////////
#include <max6675.h>
int thermoDO = 19;
int thermoCS = 23;
int thermoCLK = 5;
bool partida= true; // start incial centelha
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
///////////////////pinos///////////////////////////////////////
#define botton 12 // botão de ligar e desligar
#define led1 36 // led sinalizador ligado
#define led2 39 // led sinalizador desligador
#define reler 34 // pino de ligar os reler
bool estado=0,botaoEstadoAnterior=1,piscar=0; //vaiaveis para controle de tempo e  estado
unsigned long ultimoTempo,ultimoTempo01,ultimoTempo02;
int debounce=1000,leituraBotao;
////////////////////////servo motor/////////////////////////////////////////////
int n=0; //pode apgar
int pos=0; //posição do servo motor
#define Gas 35  // pino de leitura 
int tempoPartida=2000; //define o tempo que os reler ficara acionado
unsigned long tempoAnterior01;
int timerNews=1000;
//int tempoServo=1000;
int i=0;
void setup()
{
  Serial.begin(115200);
  SerialBT.begin(device_name);  //Bluetooth device name
  Serial.printf("ligando systema");
  
  
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(reler,OUTPUT);
  servo1.attach(servoPin); // Associa o servo motor ao pino digital 6 do Arduino
  servo1.write(0);
  
  pinMode(botton, INPUT_PULLUP);
  pinMode(Gas, INPUT_PULLUP);
  Serial.println("Iniciando Systema");
  delay(500);
}

void loop()
{
  leituraBotao = digitalRead(botton);
  if(leituraBotao==LOW && botaoEstadoAnterior == HIGH &&(millis()-ultimoTempo)>debounce){
    estado=!estado;
    ultimoTempo=millis();
    if(estado==0){
      i=2;
    }
    else{
      i=1;
    }
  }
   botaoEstadoAnterior = leituraBotao;  // Atualiza o estado anterior do botão
  switch(estado){
  case 1:
    start();
    break;
     case 0:
     if(i!=3){
      
     }
    desligar();
    break;
  }
   servo1.write(pos);
}
void start(){
  digitalWrite(led2,LOW); 
 if(i==1){
  SerialBT.println("maquina iniciada manualmente");
   i=0;
 }
 
 if((millis()-ultimoTempo01)>timerNews){
  piscar=!piscar;
    digitalWrite(led1,piscar);
    ultimoTempo01=millis();
     SerialBT.print(thermocouple.readCelsius());
     SerialBT.println(" C°");
  }


 if(digitalRead(Gas)==LOW){
  i=3;
  estado=0;
 }
 else {
  if(partida){
    SerialBT.println("Acionando chamas");
    digitalWrite(reler,LOW);
    partida=false;
  }
  else{
    if((millis()-ultimoTempo02)>2000){
    ultimoTempo02=millis();
      digitalWrite(reler,HIGH);
   }
  }
  pos=map(thermocouple.readCelsius(),30,200,180,0);
 
   }
 }

void desligar(){
 digitalWrite(reler,HIGH);
 digitalWrite(led2,HIGH);
 digitalWrite(led1,LOW);  
 if(i==3){
  SerialBT.println("Gás detectado desligamento automatico acionado");
  i=0;
 }
  if(i==2){
  SerialBT.println("Maquina deligada manualmente");
  i=0;
 }
 pos=0;
}
