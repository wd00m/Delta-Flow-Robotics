//coded by D0kDoom
//Github:

//declaração de bibliotecas
#include <BluetoothSerial.h>
#include <Bonezegei_DHT11.h>

//início de variáveis
const int rainSensor = 35;
const int humiSoloSensor = 34;

Bonezegei_DHT11 dht(32);
double data[3][24][4] = {0};

int cycle = 0;
int rain = 0;
double humiSolo = 0;
double humiAir = 0;
double temp = 0;

int day = 0;
int hour = 0;
//fim de variáveis

//declaração do Bluetooth do ESP
BluetoothSerial SerialBT;

void setup() {
  //inicialização da comunicação serial
  SerialBT.begin("ESP32_Bluetooth");
  dht.begin();
}

void loop() {
  if (SerialBT.available() > 0) {
    String receivedData = SerialBT.readStringUntil('\n');
    if (receivedData.trim() == "1") {
      for (int i = 0; i < 3; i++) {
        String output = "";
        for (int j = 0; j < 24; j++) {
          for (int k = 0; k < 4; k++) {
            output += String(data[i][j][k]) + "\t";
          }
          output += "\n";
        }
        if (SerialBT.connected()) {
          SerialBT.println(output);
        }
      }
    }
  }

  unsigned long currentMillis = millis();
  static unsigned long previousMillis = 0;
  const long interval = 60000;

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    rain = digitalRead(rainSensor);
    temp += dht.getTemperature();
    humiAir += dht.getHumidity();
    humiSolo += analogRead(humiSoloSensor);

    cycle++;

    if (cycle >= 60) {
      double tempAverage = temp / cycle;
      double humiAirAverage = humiAir / cycle;
      double humiSoloAverage = humiSolo / cycle;

      data[day][hour][0] = tempAverage;
      data[day][hour][1] = humiSoloAverage;
      data[day][hour][2] = humiAirAverage;
      data[day][hour][3] = rain;

      cycle = 0;
      temp = 0;
      humiAir = 0;
      humiSolo = 0;
      hour++;

      if (hour >= 24) {
        day++;
        hour = 0;

        if (day >= 3) {
          day = 0;
          for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 24; j++) {
              for (int k = 0; k < 4; k++) {
                data[i][j][k] = 0;
              }
            }
          }
        }
      }
    }
  }
}