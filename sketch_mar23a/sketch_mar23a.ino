#include <Wire.h>
#include <MS5611.h>

#include <SPI.h>
#include <SD.h>

const int PIN_CHIP_SELECT = 4;
 
MS5611 ms5611;
double referencePressure;
 
void setup()
{
  Serial.begin(9600);

  Serial.println("MS5611 Sensor");
  
  pinMode(10, OUTPUT);

  if (!SD.begin(PIN_CHIP_SELECT)) {
    Serial.println("Что-то пошло не так с SD картой");
    return;
  }

  Serial.println("Карта работает.");
  
  while(!ms5611.begin())
  {
    Serial.println("Ошибка!");
    delay(500);
  }

  referencePressure = ms5611.readPressure();

  checkSettings();
}
 
void checkSettings()
{
  Serial.print("Передискретизация: ");
  Serial.println(ms5611.getOversampling());
}
 
void loop()
{
  uint32_t rawTemp = ms5611.readRawTemperature();
  uint32_t rawPressure = ms5611.readRawPressure();

  double realTemperature = ms5611.readTemperature();
  long realPressure = ms5611.readPressure();

  float absoluteAltitude = ms5611.getAltitude(realPressure);
  float relativeAltitude = ms5611.getAltitude(realPressure, referencePressure);
 
  Serial.println("--");
 
  Serial.print("Сырые данные о температуре = ");
  Serial.print(rawTemp);
  Serial.print(", Обработанные данные о температуре = ");
  Serial.print(realTemperature);
  Serial.println(" *C");
 
  Serial.print(" Сырые данные о давлении = ");
  Serial.print(rawPressure);
  Serial.print(", Обработанные данные о давлении = ");
  Serial.print(realPressure);
  Serial.println(" Pa");
 
  Serial.print(" Абсолютная высота = ");
  Serial.print(absoluteAltitude);
  Serial.print(" m, Относительная высота = ");
  Serial.print(relativeAltitude);
  Serial.println(" m");

  File data = SD.open("data.csv", FILE_WRITE);

  if (data) {
    data.println(rawTemp);
    data.println(realTemperature);
    data.println(rawPressure);
    data.println(realPressure);
    data.println(absoluteAltitude);
    data.println(relativeAltitude);
    data.close();
  } else {
    Serial.println("Не удалось открыть файл.");
  }
   
  delay(1000);
}
