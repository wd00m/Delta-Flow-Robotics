//código simples, lê os valores do giroscópio e usa como parâmetro para enviar uma mensagem
//usa um giroscópio MPU-6050, um Arduino Uno e um módulo HC-05
//compilou de primeira, nunca fui tão feliz <3
//coded by D0kDoom
//my Github: https://github.com/D0kDoom

//declaração de bibliotecas
#include <SoftwareSerial.h>
#include <Wire.h>
#include <MPU6050_tockn.h>

//constantes para os pinos RX TX
const int blueRX = 2;
const int blueTX = 3;

//constantes que definem a condição de ativação
const int tgtX = 45;
const int tgtY = 20;
const int tgtZ = 30;

//define se um acidente aconteceu ou não
bool acc = false;

//latitude e longitude da sala de robótica no SESI, mudar de acordo com localização
String lat = "Latitude: 22 7 19 S";
String longi = "Longitude: 43 12 36 W";

SoftwareSerial SerialBT(blueRX, blueTX);

MPU6050 Gyro(Wire);

void setup() {

  //inicialização das seriais e wire
  Serial.begin(9600);
  SerialBT.begin(9600);
  Wire.begin();
  Gyro.begin();
  Gyro.calcGyroOffsets(true);
  Serial.println(" ");
  Serial.println("Setup rodando, aguarde...");
 
  delay(1000);
 
  Serial.println("Setup finalizado, código irá rodar em 3 segundos");
  Serial.println("Coded by D0kDoom");
 
  delay(3000);
}

void loop() {

  //recebe os valores de ângulo do giroscópio, envia os valores para as variáveis locais
  Gyro.update();
  double angleX = Gyro.getAngleX();
  double angleY = Gyro.getAngleY()+90;
  double angleZ = Gyro.getAngleZ();

  //faz a comparação dos ângulos e define se um acidente ocorreu
  if (angleX >= tgtX||angleX <= -tgtX||angleY >= tgtY||angleY <= -tgtY||angleZ >= tgtZ||angleZ <= -tgtZ) {
    
    acc = true;

    //envia o aviso junto com a geolocalização para o bluetooth
    Serial.println("Oh Noes, um acidente foi detectado");
    Serial.println("Enviando localização...");
    Serial.println(lat);
    Serial.println(longi);

  }

  //função que permite o reset manual do sistema sem a necessidade de reiniciar o Arduino fisicamente
  while(acc == true) {
    
        int reset = Serial.read();
        
        if(reset == 0) {
         
          acc = false;
          Serial.println(" ");
          Serial.println("Iniciando o processo de reinicialização, aguarde 10 segundos...");

          int i = 0;
          for(i == 0; i > 30; i++) {
            
            Gyro.update();
            angleX = Gyro.getAngleX();
            angleY = Gyro.getAngleY()+90;
            angleZ = Gyro.getAngleZ();
            delay(300);
            
          }
          Serial.println("Reinicialização concluída, recomeçando protocolo");
    
        }
      
  }

}
