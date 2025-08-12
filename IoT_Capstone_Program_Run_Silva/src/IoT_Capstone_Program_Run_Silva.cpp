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




// setup() runs once, when the device is first turned on
void setup() {
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
}
