

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
#define DHT11_PIN 2 //senzor dth11
dht DHT;

// define variables
String sensorsJson;
int temperature;
int humidity;

const size_t capacity = 2*JSON_ARRAY_SIZE(0) + JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(3) + JSON_OBJECT_SIZE(3) + 3*JSON_OBJECT_SIZE(4) +170;
DynamicJsonDocument doc(capacity);


// Declare functions to be exposed to the API
int ledControl(String command);


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
  doc_1["temp"] = 10;
  doc_1["humidity"] = 1;
  doc_1["icon"] = "fas fa-couch";
  JsonArray doc_1_relays = doc_1.createNestedArray("relays");
  
  JsonObject doc_2 = doc.createNestedObject();
  doc_2["temp"] = 1;
  doc_2["humidity"] = 1;
  doc_2["icon"] = "fas fa-utensils";
  
  JsonArray doc_2_relays = doc_2.createNestedArray("relays");
  
  JsonObject doc_2_relays_0 = doc_2_relays.createNestedObject();
  doc_2_relays_0["id"] = RELAY_CH1;
  doc_2_relays_0["status"] = false;
  doc_2_relays_0["name"] = "kitchen lights";
    
  serializeJson(doc, sensorsJson);
  rest.variable("sensors",&sensorsJson);
  rest.variable("temperature",&temperature);
  rest.variable("humidity",&humidity);

  // Function to be exposed
  rest.function("led",ledControl);

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
      int chk = DHT.read11(DHT11_PIN);
      deserializeJson(doc, sensorsJson);
      sensorsJson = "";
      // update json data when called
      doc[1]["temp"] = DHT.temperature;
      doc[1]["humidity"] = DHT.humidity;
      doc[2]["relays"][0]["status"] = digitalRead(RELAY_CH1);      
      serializeJson(doc, sensorsJson);
    }
  rest.handle(client);
  wdt_reset();
}

// Custom function accessible by the API
int ledControl(String command) {

  // Get state from command
  int state = command.toInt();
  
  digitalWrite(6,state);
 
  return 1;
}
