/* 
 * Project: IoT_Capstone_Program_Run_Silva
 * Author: Cesar J. Silva
 * Date: August 12, 2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include <Adafruit_MQTT.h>
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h"
#include "Adafruit_MQTT/Adafruit_MQTT.h"
#include "credentials.h"
#include "Adafruit_LC709203F.h"

//MQTT Setup
TCPClient TheClient;
Adafruit_MQTT_SPARK mqtt(&TheClient, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish Batt_Voltage = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Battery Voltage");
Adafruit_MQTT_Publish Batt_Percent = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Battery Percentage");
Adafruit_MQTT_Publish Batt_Temp = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Battery Temperature");

Adafruit_MQTT_Subscribe CapHome = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Capstone_Dashboard");



// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(MANUAL);
Adafruit_LC709203F lc;

void MQTT_connect();
bool MQTT_ping();
int currentTime, lastSecond;
float subValue, pubValue;

// setup() runs once, when the device is first turned on
void setup() {
  Serial.begin(9600);
  waitFor(Serial.isConnected, 5000);

  //Connect to the Internet but not Particle Cloud
  WiFi.on();
  WiFi.connect();
  while(WiFi.connecting());
  Serial.printf(".");

  //Setup MQTT subscripntion
  mqtt.subscribe(&CapHome);
  
  if ((currentTime - lastSecond)>10000) {
    lastSecond = millis();
    Serial.println("Adafruit LC709203F demo");
  }

  if (!lc.begin()){
    Serial.println(F("Couldn't find Adafruit LC709203F?\nMake sure a battery is plugged in!"));
    delay(5000);
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

  MQTT_connect();
  MQTT_ping();

  Adafruit_MQTT_Subscribe *subscription;
  while((subscription = mqtt.readSubscription(1000))){
    if (subscription == &CapHome){
      subValue = atof((char *)CapHome.lastread);
    }
  }

  Serial.print("Batt_Voltage:");
  Serial.print(lc.cellVoltage(), 3);
  Serial.print("\t");
  Serial.print("Batt_Percent:");
  Serial.print(lc.cellPercent(), 1);
  Serial.print("\t");
  Serial.print("Batt_Temp:");
  Serial.println(lc.getCellTemperature(), 1);

  delay(2000);
}
void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }
  Serial.printf("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) !=0) {
    Serial.printf("Error Code %s\n", mqtt.connectErrorString(ret));
    Serial.printf("Retrying MQTT connection in 5 seconds... \n");
    mqtt.disconnect();
    delay(5000);
  }
  Serial.printf("MQTT Connected!\n");
}

bool MQTT_ping() {
  static unsigned int last;
  bool pingStatus;
  if ((millis() - last) > 120000) {
    Serial.printf("Pinging MQTT \n");
    pingStatus = mqtt.ping();
    if(!pingStatus) {
      Serial.printf("Disconnecting \n");
      mqtt.disconnect();
    }
    last = millis();
  }
  return pingStatus;
}
