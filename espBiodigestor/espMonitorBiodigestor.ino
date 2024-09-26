//código simples, recolhe dados de um higrômetro, um sensor de chuva e um DHT-11
//versão completa, funciona 100%, eu acho
//coded by D0kDoom

//inclusão de bibliotecas
#include <BluetoothSerial.h>
#include <Bonezegei_DHT11.h>

//pinos de sensores
const int rainSensor = 35;
const int humiSoloSensor = 34;

//pino do sensor DHT-11
Bonezegei_DHT11 tempSensor(32);

//inicialização das variáveis
double data[3][24][4] = {0};

int cycle = 0;
int rain = 0;
double humiSolo = 0;
double humiAir = 0;
double temp = 0;

int day = 1;
int hour = 1;

bool doc = false;

BluetoothSerial SerialBT;

void setup() {
    Serial.begin(115200);
    SerialBT.begin("Biodigestor Delta Flow");
    tempSensor.begin();
    Serial.println("Setup está rodando, aguarde...");
    delay(2000);
    Serial.println("Setup finalizado, o programa inicializará em 3 segundos");
    Serial.println("Coded by D0kDoom");
}

void loop() {
  if (SerialBT.avaliable() && doc == false) {
    SerialBT.println("Coded by D0kDoom");
    doc = true;
  }
  if (SerialBT.available()) {
        int request = SerialBT.read();
        if (request == 1) {
            String output = "";
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 24; j++) {
                    for (int k = 0; k < 4; k++) {
                        output += String(data[i][j][k]) + "\t";
                    }
                    output += "\n";
                }
            }
            SerialBT.print(output);
        }
    }

    unsigned long currentMillis = millis();
    static unsigned long previousMillis = 0;
    const long interval = 60000;

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        rain = digitalRead(rainSensor);
        temp += tempSensor.getTemperature();
        humiAir += tempSensor.getHumidity();
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
