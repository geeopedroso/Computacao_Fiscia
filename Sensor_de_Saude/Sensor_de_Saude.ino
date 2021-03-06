#define USE_ARDUINO_INTERRUPTS true    

#include <PulseSensorPlayground.h> 
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


PulseSensorPlayground pulseSensor;
const int Pulse = 4;    //leitor BPM
const int Led13 = 13;   //led
const int Temp = 3;     //leitor de Temperatura
int Hold = 550;
int vetorTemp[10];
int posicao = 0;
int tamanho = 10;

void setup() {
  Serial.begin(9600);
  
  lcd.begin(16, 2);
  
  pulseSensor.analogInput(Pulse);   
  pulseSensor.blinkOnPulse(Led13);     
  pulseSensor.setThreshold(Hold); 
 
  pinMode(Led13,OUTPUT);

  if (pulseSensor.begin()) {
    lcd.print("Ativando Leitor de Batimentos Cardiacos!");
  }
}

String leStringSerial(){
  String conteudo = "";
  char caractere;
  
  while(Serial.available() > 0) {
    caractere = Serial.read();
    if (caractere != '\n'){
      conteudo.concat(caractere);
      lcd.clear();
    }
    delay(10);
  }
    
  Serial.print("Recebi: ");
  Serial.println(conteudo);

  lcd.print(conteudo);
  return conteudo;
}

void batimentosCardiacos(){
 int myBPM = pulseSensor.getBeatsPerMinute();  
                                              
  if (pulseSensor.sawStartOfBeat()) {            
   Serial.println("A HeartBeat Happened ! ");
   Serial.print("BPM: ");                       
   Serial.println(myBPM);                       

   lcd.print("Lendo Batimentos! "); 
   lcd.setCursor(0,1);
   lcd.print("BPM: ");                        
   lcd.print(myBPM); 
  }
  delay(20);                   
}

float Ler_Temperatura(){
  float temperatura = ( 5.0 * analogRead(Temp) * 100.0) / 1024.0;   
  Serial.println(temperatura);
  vetorTemp[posicao] = temperatura;
  posicao++;
}

float Calcula_Temperatura(){
  float soma = 0;
  for (int i=0;i<tamanho;i++){
    soma = soma + vetorTemp[i];
  }

  soma = soma/tamanho;

  return soma;
}

void Temperatura(){
  while(posicao<tamanho){
    Ler_Temperatura();
    delay(10000);
  }
  float temperatura = Calcula_Temperatura();
  //lcd.clear();
  //lcd.print("Temperatura: ");
  lcd.setCursor(0,1);
  lcd.print(temperatura);
}

void loop() {
  //batimentosCardiacos();
  
  Temperatura();
  
  //leStringSerial();
 
  //Serial.print("Sinal: ");
  //Serial.println(Signal); 

  //delay(2000);
}
