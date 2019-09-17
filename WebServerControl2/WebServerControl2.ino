#include <SimpleDHT.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>





    //---------Ethernet Shield Settings--

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x2D };
byte gateway[] = { 192, 168, 1, 254 }; //               <------- PUT YOUR ROUTERS IP Address to which your shield is connected Here
byte subnet[] = { 255, 255, 255, 0 }; //                <------- It will be as it is in most of the cases
IPAddress ip(192, 168, 1, 200);
EthernetServer server(80);

//---------Pins Variables--
#define RELAY_CH1  3
#define BED_TEMP_SENSOR 3

//sensors
int pinDHT11 = 2; //<--bedroomHumidityAndTempSensor to digitalPin 2 (right side)

//variables
int bedTemp;
int livTemp = 1;
int kitTemp = 1;
int bathTemp = 1;
int bedHum;
int livHum = 1;
int kitHum = 1;
int bathHum = 1;

bool furnitureLight = false;

String readString;

///////////////////////
String teststring = String(100);
String finalstring = String(100);
String flag = String(2);

SimpleDHT11 dht11(pinDHT11);

void setup(){
    // pinMode(BED_TEMP_SENSOR, INPUT);
     pinMode(RELAY_CH1, OUTPUT);
    // digitalWrite(RELAY_CH1, LOW);  // switch on LED1

    //----start the Ethernet connection and the server------

    Ethernet.begin(mac, ip);
    server.begin();
    Serial.print("server is at ");
    Serial.println(Ethernet.localIP());

    //enable serial data print 
    Serial.begin(9600);

}

void loop(){

  
  
////////Humidity And Temp Sensors//////

  if (pinDHT11) {

        byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, data)) != SimpleDHTErrSuccess) {
    bedTemp = 0;
    bedHum = 0;
    delay(1000);
    return;
  }

  bedTemp = (int)temperature;
  bedHum = (int)humidity;
  delay(2000);
  
    }
    
////////////////Server Setup //////

    EthernetClient client = server.available();
    if (client) {
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();

                //read char by char HTTP request
                if (readString.length() < 100) {

                    //store characters to string 
                    readString += c;
                }

                //if HTTP request has ended
                if (c == '\n') {

                    ///////////////
                    Serial.println(readString);
                    //readString looks like "GET /?-0p1555-1p500t1000 HTTP/1.1"

                    //////////////////////
                   //GET /?Slidervalue0=1800&Submit=Sub+0 HTTP/1.1
                   if (readString.indexOf("FURNITURELED_ON") > 0) {
                       furnitureLight = true;
                       digitalWrite(RELAY_CH1, HIGH);
          
                   }
                   else if (readString.indexOf("FURNITURELED_OFF") > 0) {
                       furnitureLight = false;
                       digitalWrite(RELAY_CH1, LOW);
                   }
                    ///////////////////

                    //now output HTML data header

                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type:  application/json");
                    client.println("Access-Control-Allow-Origin: *"); // allow cors
                    client.println("Connection: close");  // the connection will be closed after completion of the response
                    // client.println("Refresh: 5");  // refresh the page automatically every 5 sec
                    // ---json generator----
                    client.println();
const size_t capacity = 2*JSON_ARRAY_SIZE(0) + 3*JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(4) + 3*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 4*JSON_OBJECT_SIZE(5);
DynamicJsonDocument doc(capacity);

JsonArray sensors = doc.createNestedArray("sensors");

JsonObject sensors_0 = sensors.createNestedObject();
sensors_0["temp"] = bedTemp;
sensors_0["tempProcent"] = (bedTemp/60)*100;
sensors_0["humidity"] = 1;
sensors_0["icon"] = "fas fa-bed";
JsonArray sensors_0_relays = sensors_0.createNestedArray("relays");

JsonObject sensors_1 = sensors.createNestedObject();
sensors_1["temp"] = 1;
sensors_1["tempProcent"] = 1;
sensors_1["humidity"] = 1;
sensors_1["icon"] = "fas fa-couch";

JsonArray sensors_1_relays = sensors_1.createNestedArray("relays");

JsonObject sensors_1_relays_0 = sensors_1_relays.createNestedObject();
sensors_1_relays_0["status"] = false;
sensors_1_relays_0["name"] = "living lights";

JsonObject sensors_2 = sensors.createNestedObject();
sensors_2["temp"] = 1;
sensors_2["tempProcent"] = 1;
sensors_2["humidity"] = 1;
sensors_2["icon"] = "fas fa-utensils";

JsonArray sensors_2_relays = sensors_2.createNestedArray("relays");

JsonObject sensors_2_relays_0 = sensors_2_relays.createNestedObject();
sensors_2_relays_0["status"] = false;
sensors_2_relays_0["name"] = "kitchen lights";

JsonObject sensors_3 = sensors.createNestedObject();
sensors_3["temp"] = 1;
sensors_3["tempProcent"] = 1;
sensors_3["humidity"] = 1;
sensors_3["icon"] = "fas fa-bath";
JsonArray sensors_3_relays = sensors_3.createNestedArray("relays");

JsonArray relays = doc.createNestedArray("relays");

JsonObject relays_0 = relays.createNestedObject();
relays_0["name"] = "kitchen lights";
relays_0["icon"] = "fas fa-utensils";
relays_0["status"] = true;

serializeJson(doc, client);
                    Serial.println();
                    client.println();
                    delay(1);
                    //stopping client
                    client.stop();

                    /////////////////////
                    //clearing string for next read
                    readString = "";
                }
            }
        }
    }
}
