//código simples, lê os valores do giroscópio e usa como parâmetro para enviar uma mensagem
//usa um giroscópio MPU-6050, um Arduino Uno e um módulo HC-05
//coded by D0kDoom
//My Github: https://github.com/D0kDoom

#include <SoftwareSerial.h>
#include <Wire.h>
#include <MPU6050_tockn.h>

const int blueRX = 2;
const int blueTX = 3;

const int tgtX = 45;
const int tgtY = 20;
const int tgtZ = 30;

bool acc = false;

String lat = "Latitude: 22 7 19 S";
String longi = "Longitude: 43 12 36 W";

SoftwareSerial SerialBT(blueRX, blueTX);

MPU6050 Gyro(Wire);

void setup() {
  
  Serial.begin(9600);
  SerialBT.begin(9600);
  Wire.begin();
  Gyro.begin();
  Gyro.calcGyroOffsets(true);

  Serial.println("Setup rodando, aguarde...");
 
  delay(1000);
 
  Serial.println("Setup finalizado, código irá rodar em 3 segundos");
  Serial.println("Coded by D0kDoom");
 
  delay(3000);
}

void loop() {

  Gyro.update();
  double angleX = Gyro.getAngleX();
  double angleY = Gyro.getAngleY();
  double angleZ = Gyro.getAngleZ();

  if (angleX >= tgtX||angleX <= -tgtX||angleY >= tgtY||angleY <= -tgtY||angleZ >= tgtZ||angleZ <= -tgtZ) {
    
    acc = true;
    
    SerialBT.println("Oh Noes, um acidente foi detectado");
    SerialBT.println("Enviando localização...");
    SerialBT.println(lat);
    SerialBT.println(longi);

  }

  while(acc = true) {
      if (SerialBT.available()) {
        char reset = SerialBT.read();
        if(reset = "0") {
          acc = false;
        }
      }
  }

}
