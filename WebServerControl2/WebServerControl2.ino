

/*
  This a simple example of the aREST Library for Arduino (Uno/Mega/Due/Teensy)
  using the Ethernet library (for example to be used with the Ethernet shield).
  See the README file for more details.

  Written in 2014 by Marco Schwartz under a GPL license.
*/

// Libraries
#include <SPI.h>
#include <Ethernet.h>
#include <aREST.h>
#include <avr/wdt.h>
#include <dht.h>
#include <ArduinoJson.h>

// Enter a MAC address for your controller below.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x2D };
byte gateway[] = { 192, 168, 1, 254 }; //               <------- PUT YOUR ROUTERS IP Address to which your shield is connected Here
byte subnet[] = { 255, 255, 255, 0 }; //                <------- It will be as it is in most of the cases
// IP address in case DHCP fails
IPAddress ip(192,168,1,200);

// Ethernet server
EthernetServer server(80);

// Create aREST instance
aREST rest = aREST();

// Variables to be exposed to the API
#define RELAY_CH1 3 // releu bucatarie
#define DHT22_LIVING 2 //senzor temp/hum living
#define DHT22_KITCHEN 7 //senzor temp/hum kitchen
#define DHT22_BEDROOM 8 //senzor temp/hum bedroom

dht DHT;

// define variables
String sensorsJson;
String paramsJson;
int temperature;
int humidity;

const size_t capacity = JSON_ARRAY_SIZE(0) + 2*JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(3) + 2*JSON_OBJECT_SIZE(3) + 3*JSON_OBJECT_SIZE(4) + 220;
DynamicJsonDocument doc(capacity);

const size_t capacity2 = JSON_ARRAY_SIZE(10) + 10*JSON_OBJECT_SIZE(1) + 160;
DynamicJsonDocument docParams(capacity2);

// Declare functions to be exposed to the API
int sonoffStatus(String command);


void setup(void)
{
  // Start Serial
  Serial.begin(115200);
  
  // pin modes (output or input)
  pinMode(RELAY_CH1, OUTPUT);
  // Init variables and expose them to REST API
  
  //Define sensors Object
  JsonObject doc_0 = doc.createNestedObject();
  doc_0["temp"] = 1;
  doc_0["humidity"] = 1;
  doc_0["icon"] = "fas fa-bed";
  JsonArray doc_0_relays = doc_0.createNestedArray("relays");
  
  JsonObject doc_1 = doc.createNestedObject();
  doc_1["temp"] = 1;
  doc_1["humidity"] = 1;
  doc_1["icon"] = "fas fa-couch";
  JsonArray doc_1_relays = doc_1.createNestedArray("relays");
  
  JsonObject doc_1_relays_0 = doc_1_relays.createNestedObject();
  doc_1_relays_0["status"] = false;
  doc_1_relays_0["id"] = "100062aeb3";
  doc_1_relays_0["name"] = "Living lights";
  doc_1_relays_0["webhook"] = "living_lights_";
  
  JsonObject doc_2 = doc.createNestedObject();
  doc_2["temp"] = 1;
  doc_2["humidity"] = 1;
  doc_2["icon"] = "fas fa-utensils";
  
  JsonArray doc_2_relays = doc_2.createNestedArray("relays");
  
  JsonObject doc_2_relays_0 = doc_2_relays.createNestedObject();
  doc_2_relays_0["id"] = RELAY_CH1;
  doc_2_relays_0["status"] = false;
  doc_2_relays_0["name"] = "Kitchen lights";
    
  serializeJson(doc, sensorsJson);
  rest.variable("sensors",&sensorsJson);
  rest.variable("temperature",&temperature);
  rest.variable("humidity",&humidity);

  // Function to be exposed
  rest.function("sonoff",sonoffStatus);

  // Give name & ID to the device (ID should be 6 characters long)
  rest.set_id("001");
  rest.set_name("home_controller");

  // Start the Ethernet connection and the server
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);
  }
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  // Start watchdog
  wdt_enable(WDTO_4S);
}

void loop() {

  EthernetClient client = server.available();
  if (client) {

      deserializeJson(doc, sensorsJson);
      sensorsJson = "";
      // update json data when called
          
      int chk = DHT.read22(DHT22_BEDROOM);
      doc[0]["temp"] = (int)DHT.temperature;
      doc[0]["humidity"] = (int)DHT.humidity;

      chk = DHT.read22(DHT22_LIVING);
      doc[1]["temp"] = (int)DHT.temperature;
      doc[1]["humidity"] = (int)DHT.humidity;

      chk = DHT.read22(DHT22_KITCHEN);  
      doc[2]["temp"] = (int)DHT.temperature;
      doc[2]["humidity"] = (int)DHT.humidity;
            
      doc[2]["relays"][0]["status"] = digitalRead(RELAY_CH1);   
      serializeJson(doc, sensorsJson);
    }
  rest.handle(client);
  wdt_reset();
}

// Custom function accessible by the API
int sonoffStatus(String command) {

  // Get state from command
  int comm = command.toInt();

  deserializeJson(doc, sensorsJson);
    
  sensorsJson = "";
//    for (int k =0; k < doc.size(); k++) {
//      if(doc[k]["relays"][0]["id"] == "100062aeb3") {
//         Serial.println("result:");
//         Serial.println(k);
//       }
//    }
  // update json data when called  
  doc[1]["relays"][0]["status"] = comm;      
  serializeJson(doc, sensorsJson);
 
  return 1;
}
