/* 
 * Project: FuelGauge_Example
 * Author: Cesar J. Silva
 * Date: August 8, 2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "Adafruit_LC709203F.h"


// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);
Adafruit_LC709203F lc;


// setup() runs once, when the device is first turned on
void setup() {
  Serial.begin(9600);
  waitFor(Serial.isConnected, 10000);
  delay(10);
  Serial.println("Adafruit LC709203F demo");

  // For the Feather ESP32-S2, we need to enable T2C power FIRST!
  //this section can be dleted for other boards
  // turn on the I2C power by setting pin to opposite of 'rest state'

  if (!lc.begin()) {
    Serial.println(F("Couldn't find Adafruit LC709203F?\nMake sure a battery is plugged in!"));
    while (1) delay(10);
  }
  Serial.println(F("Found LC709203F"));
  Serial.print("Version: 0x"); Serial.println(lc.getICversion(), HEX);

  lc.setThermistorB(3950);
  Serial.print("Thermistor B = "); Serial.println(lc.getThermistorB());

  lc.setPackSize(LC709203F_APA_500MAH);

  lc.setAlarmVoltage(3.8);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  Serial.print("Batt_Voltage:");
  Serial.print(lc.cellVoltage(), 3);
  Serial.print("\t");
  Serial.print("Batt_Percent:");
  Serial.print(lc.cellPercent(), 1);
  Serial.print("\t");
  Serial.print("Batt_Temp:");
  Serial.println(lc.getCellTemperature(), 1);

  delay(2000); // don't query too often!
}
