#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <ArduinoJson.h>


    //---------Ethernet Shield Settings--

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x2D };
byte gateway[] = { 192, 168, 1, 254 }; //               <------- PUT YOUR ROUTERS IP Address to which your shield is connected Here
byte subnet[] = { 255, 255, 255, 0 }; //                <------- It will be as it is in most of the cases
IPAddress ip(192, 168, 1, 200);
EthernetServer server(80);

//---------Pins Variables--
#define RELAY_CH1  2 
#define BED_TEMP_SENSOR 3

int bedTemp ;
int livTemp = 32;
int kitTemp = 34;
int bathTemp = 31;
int bedHum = 10;
int livHum = 20;
int kitHum = 30;
int bathHum = 40;

bool bedLight = false;

String readString;

//---------Setup-------------
void setup() {

    pinMode(BED_TEMP_SENSOR, INPUT);
  
    pinMode(RELAY_CH1, OUTPUT);
    digitalWrite(RELAY_CH1, LOW);  // switch on LED1

    //------Open serial communications and wait for port to open:-----------

    Serial.begin(9600); 

    //----start the Ethernet connection and the server------

    Ethernet.begin(mac, ip);
    server.begin();
    Serial.print("server is at ");
    Serial.println(Ethernet.localIP());
}

//---------Loop-------------

void loop() {

    //------Ethernet server setup---
  
    EthernetClient client = server.available();

    if (client) {
        Serial.println("new client");
        // an http request ends with a blank line
        boolean currentLineIsBlank = true;
        while (client.connected()) {

            if (client.available()) {

                char c = client.read();

                if (readString.length() < 100) {
                    //store characters to string
                    readString += c;			//Serial.print(c);
                }

                Serial.write(c);
                // if you've gotten to the end of the line (received a newline
                // character) and the line is blank, the http request has ended,
                // so you can send a reply
                // https://arduinojson.org/v5/assistant/  --> json assistant
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
                if (bedLight == false){
                    relayStatus_bedroom["trigger"] = "<a href='/?turnONLight'>on</a>";
                }
                else if (bedLight == true) {
                    relayStatus_bedroom["trigger"] = "<a href='/?turnOFFLight'>on</a>";
                    // de adaugat parametru sa porneasca releul
                }

                root.printTo(Serial);
                Serial.println();

                if (c == '\n' && currentLineIsBlank) {

                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: application/json");
                    client.println("Access-Control-Allow-Origin: *"); // allow cors
                    client.println("Connection: close");  // the connection will be closed after completion of the response
                    client.println("Refresh: 3");  // refresh the page automatically every 5 sec
                    client.println();
                    root.prettyPrintTo(client);
                    //             if (digitalRead(RELAY_CH8))
                    //             { 
                    //                client.print("<td><p style=\"font-family:arial;color:black;font-size:26px;\">Device 8.</p><p style=\"font-family:arial;color:green;font-size:35px;\">ON</p></td>");
                    //             }
                    //             else
                    //             {
                    //                 client.print("<td><p style=\"font-family:arial;color:black;font-size:26px;\">Device 8.</p><p style=\"font-family:arial;color:red;font-size:35px;\">OFF</p></td>");
                    //             }
                    // client.println("<a href=\"/?relay8on\"\"><button style=\"width:360px;height:120px\"> <font size=\"7\">Device 8 ON </font> </button> </a> ");
                    // client.println("<a href=\"/?relay8off\"\"><button style=\"width:360px;height:120px\"> <font size=\"7\">Device 8 OFF </font> </button> </a> <br />"); 
                    // client.println("<br />");
                    // control arduino pin via ethernet Start //

                    if (readString.indexOf("?turnONLight") > 0)//checks for on
                    {
                       bedLight = true;
                    }
                    else {
                        if (readString.indexOf("?turnOFFLight") > 0)//checks for off
                        {
                            bedLight = false;
                        }
                    }
                    break;
                }// if ( c == \n )

                if (c == '\n') {
                    // you're starting a new line
                    currentLineIsBlank = true;
                }
                else if (c != '\r') {
                    // you've gotten a character on the current line
                    currentLineIsBlank = false;
                }
            }
        }
        // give the web browser time to receive the data   
        delay(1);
        // close the connection:
        client.stop();
        Serial.println("client disonnected");
    }//if(Client)
}//Loop
