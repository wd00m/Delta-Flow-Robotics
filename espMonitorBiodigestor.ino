// coded by D0kDoom
// Github:

// declaração de bibliotecas
#include <BluetoothSerial.h>
#include <Bonezegei_DHT11.h>

// início de variáveis
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
// fim de variáveis

BluetoothSerial SerialBT(Serial, true); // Passando Serial e o booleano

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Sistema de Monitoramento do Solo"); // Nome do módulo Bluetooth
}

void loop() {
  // verifica se há um pedido por dados, e se houver, envia os dados
  if (SerialBT.available()) {
    String receivedData = SerialBT.readStringUntil('\n', 100); // Timeout de 100ms
    if (receivedData.trim() == "1") {
      for (int i = 0; i < 3; i++) {
        String output = "";
        for (int j = 0; j < 24; j++) {
          for (int k = 0; k < 4; k++) {
            output += String(data[i][j][k]) + "\t";
          }
          output += "\n";
        }
        SerialBT.print(output); // Use print ao invés de println
      }
    }
  }

  // contagem de tempo em millis para ciclos
  unsigned long currentMillis = millis();
  static unsigned long previousMillis = 0;
  const long interval = 60000;

  // conta os ciclos
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    rain = digitalRead(rainSensor);
    temp += dht.getTemperature();
    humiAir += dht.getHumidity();
    humiSolo += analogRead(humiSoloSensor);

    cycle++;

    // quando a variável cycles atinge 60, recomeça a contagem na próxima hora
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

      // quando se completam 24 horas, recomeça o ciclo no dia seguinte
      if (hour >= 24) {
        day++;
        hour = 0;

        // quando se completa 3 dias, reinicia-se o sistema
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
