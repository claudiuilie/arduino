#include < Dhcp.h >
#include < Dns.h >
#include < Ethernet.h >
#include < EthernetClient.h >
#include < EthernetServer.h >
#include < EthernetUdp.h >
#include < SPI.h >
#include < ArduinoJson.h >


    //---------Ethernet Shield Settings--

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x2D };
byte gateway[] = { 192, 168, 1, 254 }; //               <------- PUT YOUR ROUTERS IP Address to which your shield is connected Here
byte subnet[] = { 255, 255, 255, 0 }; //                <------- It will be as it is in most of the cases
IPAddress ip(192, 168, 1, 200);
EthernetServer server(80);

//---------Pins Variables--
#define RELAY_CH1  2
#define BED_TEMP_SENSOR 3

int bedTemp;
int livTemp = 32;
int kitTemp = 34;
int bathTemp = 31;
int bedHum = 10;
int livHum = 20;
int kitHum = 30;
int bathHum = 40;

bool bedLight = false;

String readString;

///////////////////////
String teststring = String(100);
String finalstring = String(100);
String flag = String(2);


void setup(){
    // pinMode(BED_TEMP_SENSOR, INPUT);
    // pinMode(RELAY_CH1, OUTPUT);
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
    // Create a client connection
    Client client = server.available();
    if (client) {
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();

                //read char by char HTTP request
                if (readString.length() < 100) {

                    //store characters to string 
                    readString.append(c);
                }

                //if HTTP request has ended
                if (c == '\n') {

                    ///////////////
                    Serial.println(readString);
                    //readString looks like "GET /?-0p1555-1p500t1000 HTTP/1.1"

                    ////////////////////////
                    //GET /?Slidervalue0=1800&Submit=Sub+0 HTTP/1.1
                    if (readString.contains("turnOnLight")) {
                        bedLight = true;
                    }
                    else if (readString.contains("turnOffLight")) {
                        bedLight = false;
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
                    StaticJsonBuffer < 1200 > jsonBuffer;
                    JsonObject & root = jsonBuffer.createObject();
                    JsonArray & tempSensors = root.createNestedArray("tempSensors");

                    JsonObject & tempSensors_0 = tempSensors.createNestedObject();
                    tempSensors_0["temp"] = bedTemp;
                    tempSensors_0["badge"] = "Bedroom Temp :";
                    tempSensors_0["unit"] = "°C";

                    JsonObject & tempSensors_1 = tempSensors.createNestedObject();
                    tempSensors_1["temp"] = livTemp;
                    tempSensors_1["badge"] = "Livin Temp :";
                    tempSensors_1["unit"] = "°C";

                    JsonObject & tempSensors_2 = tempSensors.createNestedObject();
                    tempSensors_2["temp"] = kitTemp;
                    tempSensors_2["badge"] = "Kitchen Temp :";
                    tempSensors_2["unit"] = "°C";

                    JsonObject & tempSensors_3 = tempSensors.createNestedObject();
                    tempSensors_3["temp"] = bathTemp;
                    tempSensors_3["badge"] = "Bathroom Temp :";
                    tempSensors_3["unit"] = "°C";

                    JsonArray & humiditySensors = root.createNestedArray("humiditySensors");

                    JsonObject & humiditySensors_0 = humiditySensors.createNestedObject();
                    humiditySensors_0["humidity"] = bedHum;
                    humiditySensors_0["badge"] = "Bedroom humidity :";
                    humiditySensors_0["unit"] = "%";

                    JsonObject & humiditySensors_1 = humiditySensors.createNestedObject();
                    humiditySensors_1["humidity"] = livHum;
                    humiditySensors_1["badge"] = "Living humidity :";
                    humiditySensors_1["unit"] = "%";

                    JsonObject & humiditySensors_2 = humiditySensors.createNestedObject();
                    humiditySensors_2["humidity"] = kitHum;
                    humiditySensors_2["badge"] = "Kitchen humidity :";
                    humiditySensors_2["unit"] = "%";

                    JsonObject & humiditySensors_3 = humiditySensors.createNestedObject();
                    humiditySensors_3["humidity"] = bathHum;
                    humiditySensors_3["badge"] = "Bathroom humidity :";
                    humiditySensors_3["unit"] = "%";

                    JsonObject & relayStatus = root.createNestedObject("relayStatus");

                    JsonObject & relayStatus_bedroom = relayStatus.createNestedObject("bedroom");
                    relayStatus_bedroom["status"] = bedLight;

                    root.printTo(Serial);
                    Serial.println();
                    root.prettyPrintTo(client);

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



