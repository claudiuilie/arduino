//--------------------------Libraryes----------------------------------------------------------------------------------------------------------------------

#include <SPI.h>
#include <Ethernet.h>


//--------------------------Ethernet Shield Settings-------------------------------------------------------------------------------------------------------

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x2D };
byte gateway[] = { 192, 168, 1, 254 }; //               <------- PUT YOUR ROUTERS IP Address to which your shield is connected Here
byte subnet[] = { 255, 255, 255, 0 }; //                <------- It will be as it is in most of the cases
IPAddress ip(192,168,1,200);
EthernetServer server(80);

//--------------------------Pins Variables-----------------------------------------------------------------------------------------------------------------
#define RELAY_CH1  2
#define RELAY_CH2  3
#define RELAY_CH3  4
#define RELAY_CH4  5
#define RELAY_CH5  6
#define RELAY_CH6  7
#define RELAY_CH7  8
#define RELAY_CH8  9


int temperature=32;
int RECV_PIN = 8; //IR receiver digital pin in

String readString;



//-------------------------Setup-----------------------------------------------------------------------------------------------------------------------------
void setup() {
          
  
        pinMode(RELAY_CH1, OUTPUT); 
 	digitalWrite(RELAY_CH1, LOW);  // switch on LED1
 
  
 //-------------------------------------------------------------------------------------------
 
 Serial.begin(9600); // Open serial communications and wait for port to open:

 //-------------------------------------------------------------------------------------------
 
 Ethernet.begin(mac, ip);// start the Ethernet connection and the server:
 server.begin();
 Serial.print("server is at ");
 Serial.println(Ethernet.localIP());
  
}


void loop() {

//--------------------------------------------------------------------------Remote Control Code--------------------------------------------------------------------------------------//  
//--------------------------Relay 1-------------------//
            
//if((results.value == 16724175) && (onOffCounter1 == 0 )){
//      digitalWrite(RELAY_CH1, HIGH);
//      onOffCounter1 = 1;
//      results.value = 0;
//      Serial.println(onOffCounter1);
//}
//if ((results.value == 16724175) && (onOffCounter1 == 1 )){
//      digitalWrite(RELAY_CH1, LOW);
//      onOffCounter1 = 0;
//      results.value = 0;
//      Serial.println(onOffCounter1);
//}

//-------------------------Turn Off All Relays-------------//




//-------------------------------------Ethernet server setup-----------------------------------
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
        
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Access-Control-Allow-Origin: *");
          client.println("Connection: close");  // the connection will be closed after completion of the response
	        client.println("Refresh: 3");  // refresh the page automatically every 5 sec
          client.println();
//          client.println("<!DOCTYPE HTML>");
          client.println("{\"tempIn :");
          client.println(temperature);
          client.println("tempOut\":16.8, \"unit\":\"Celcius\" }");
          // client.println("<html>");
          // client.println("<HEAD>");
          // client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
          // client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
          // //client.println("<link rel=\"stylesheet\" type=\"text/css\" href=\"http://www.digital2u.net/autohome.css\" />");
          // //client.println("<meta http-equiv=\"refresh\" content=\"5;url=http://192.168.0.199/\"/>" );
          // client.println("</HEAD>");
          // client.println("<body bgcolor=\"#ccffdd\">");
          // client.print("<center> <p><h1>Arduino Home</h1></p><center>");
          //     client.println("<table border=\"5\">");
          //         client.println("<tr>"); 
          //             if (digitalRead(RELAY_CH1))
          //             { 
          //                  client.print("<td> <p style=\"font-family:arial;color:black;font-size:26px;\">Device 1.</p><p style=\"font-family:arial;color:green;font-size:35px;\">ON</p> </td>");
          //             }
          //             else
          //             {
          //                  client.print("<td> <p style=\"font-family:arial;color:black;font-size:26px;\">Device 1.</p><p style=\"font-family:arial;color:red;font-size:35px;\">OFF</p></td>");
          //             }
                      
                                  
          //             if (digitalRead(RELAY_CH2))
          //             {   
          //                  client.print("<td> <p style=\"font-family:arial;color:black;font-size:26px;\">Device 2.</p><p style=\"font-family:arial;color:green;font-size:35px;\">ON</p></td>");
          //             }
          //             else
          //             {  
          //                 client.print("<td> <p style=\"font-family:arial;color:black;font-size:26px;\">Device 2.</p><p style=\"font-family:arial;color:red;font-size:35px;\">OFF</p></td>");
          //             }
                     
                      
          //             if (digitalRead(RELAY_CH3))
          //             { 
          //                  client.print("<td><p style=\"font-family:arial;color:black;font-size:26px;\">Device 3.</p><p style=\"font-family:arial;color:green;font-size:35px;\">ON</p></td>");
          //             }
          //             else
          //             {
          //                 client.print("<td><p style=\"font-family:arial;color:black;font-size:26px;\">Device 3.</p><p style=\"font-family:arial;color:red;font-size:35px;\">OFF</p></td>");
          //             }
                     
                     
          //             if (digitalRead(RELAY_CH4))
          //             { 
          //                client.print("<td><p style=\"font-family:arial;color:black;font-size:26px;\">Device 4.</p><p style=\"font-family:arial;color:green;font-size:35px;\">ON</p></td>");
          //             }
          //             else
          //             {
          //                 client.print("<td><p style=\"font-family:arial;color:black;font-size:26px;\">Device 4.</p><p style=\"font-family:arial;color:red;font-size:35px;\">OFF</p></td>");
          //             }
                     
                     
          //             if (digitalRead(RELAY_CH5))
          //             { 
          //                  client.print("<td><p style=\"font-family:arial;color:black;font-size:26px;\">Device 5.</p><p style=\"font-family:arial;color:green;font-size:35px;\">ON</p></td>");
          //             }
          //             else
          //             {
          //                 client.print("<td><p style=\"font-family:arial;color:black;font-size:26px;\">Device 5.</p><p style=\"font-family:arial;color:red;font-size:35px;\">OFF</p></td>");
          //             }
                     
                             
          //             if (digitalRead(RELAY_CH6))
          //             {   
          //                  client.print("<td><p style=\"font-family:arial;color:black;font-size:26px;\">Device 6.</p><p style=\"font-family:arial;color:green;font-size:35px;\">ON</p></td>");
          //             }
          //             else
          //             {  
          //                 client.print("<td><p style=\"font-family:arial;color:black;font-size:26px;\">Device 6.</p><p style=\"font-family:arial;color:red;font-size:35px;\">OFF</p></td>");
          //             }
                      
                          
          //             if (digitalRead(RELAY_CH7))
          //             { 
          //                  client.print("<td><p style=\"font-family:arial;color:black;font-size:26px;\">Device 7.</p><p style=\"font-family:arial;color:green;font-size:35px;\">ON</p></td>");
          //             }
          //             else
          //             {
          //                 client.print("<td><p style=\"font-family:arial;color:black;font-size:26px;\">Device 7.</p><p style=\"font-family:arial;color:red;font-size:35px;\">OFF</p></td>");
          //             }
                     
                     
          //             if (digitalRead(RELAY_CH8))
          //             { 
          //                client.print("<td><p style=\"font-family:arial;color:black;font-size:26px;\">Device 8.</p><p style=\"font-family:arial;color:green;font-size:35px;\">ON</p></td>");
          //             }
          //             else
          //             {
          //                 client.print("<td><p style=\"font-family:arial;color:black;font-size:26px;\">Device 8.</p><p style=\"font-family:arial;color:red;font-size:35px;\">OFF</p></td>");
          //             }
                                         
          //        client.println("</tr>");                   
          //     client.println("</table>"); 
          // client.println("</center>");
          // client.println("<br />");

         /* Relay Control Code  */                
        
          // client.println("<a href=\"/?relay1on\"\"> <button style=\"width:360px;height:120px\"> <font size=\"7\"; color:red ;>Device 1 ON </font> </button> </a> ");
          // client.println("<a href=\"/?relay1off\"\"> <button style=\"width:360px;height:120px\"> <font size=\"7\">Device 1 OFF </font> </button> </a> <br />"); 
          // client.println("<br />");
          
          // client.println("<a href=\"/?relay2on\"\"> <button style=\"width:360px;height:120px\"> <font size=\"7\">Device 2 ON </font> </button> </a> ");
          // client.println("<a href=\"/?relay2off\"\"> <button style=\"width:360px;height:120px\"> <font size=\"7\">Device 2 OFF </font> </button> </a> <br />"); 
          // client.println("<br />");
                  
          // client.println("<a href=\"/?relay3on\"\"> <button style=\"width:360px;height:120px\"> <font size=\"7\">Device 3 ON </font> </button> </a> ");
          // client.println("<a href=\"/?relay3off\"\"> <button style=\"width:360px;height:120px\"> <font size=\"7\">Device 3 OFF </font> </button> </a> <br />"); 
          // client.println("<br />");
          
          // client.println("<a href=\"/?relay4on\"\"> <button style=\"width:360px;height:120px\"> <font size=\"7\">Device 4 ON </font> </button> </a> ");
          // client.println("<a href=\"/?relay4off\"\"> <button style=\"width:360px;height:120px\"> <font size=\"7\">Device 4 OFF </font> </button> </a> <br />"); 
          // client.println("<br />");
                 
          // client.println("<a href=\"/?relay5on\"\"> <button style=\"width:360px;height:120px\"> <font size=\"7\">Device 5 ON </font> </button> </a> ");
          // client.println("<a href=\"/?relay5off\"\"><button style=\"width:360px;height:120px\"> <font size=\"7\">Device 5 OFF </font> </button> </a> <br />"); 
          // client.println("<br />");        

          // client.println("<a href=\"/?relay6on\"\"><button style=\"width:360px;height:120px\"> <font size=\"7\">Device 6 ON </font> </button> </a> ");
          // client.println("<a href=\"/?relay6off\"\"><button style=\"width:360px;height:120px\"> <font size=\"7\">Device 6 OFF </font> </button> </a> <br />"); 
          // client.println("<br />");  
          
          // client.println("<a href=\"/?relay7on\"\"><button style=\"width:360px;height:120px\"> <font size=\"7\">Device 7 ON </font> </button> </a> ");
          // client.println("<a href=\"/?relay7off\"\"><button style=\"width:360px;height:120px\"> <font size=\"7\">Device 7 OFF </font> </button> </a> <br />");  
          // client.println("<br />");
          
          // client.println("<a href=\"/?relay8on\"\"><button style=\"width:360px;height:120px\"> <font size=\"7\">Device 8 ON </font> </button> </a> ");
          // client.println("<a href=\"/?relay8off\"\"><button style=\"width:360px;height:120px\"> <font size=\"7\">Device 8 OFF </font> </button> </a> <br />"); 
          // client.println("<br />");
         
          
          // control arduino pin via ethernet Start //

        if(readString.indexOf("?relay1on") >0 )//checks for on
        {
            digitalWrite(RELAY_CH1, HIGH);    // set pin 4 high
            //Serial.println("Led On");
            
            client.println("works");
            //client.println("Light 1 Is On");
            client.println("not worked");
	}
        else{
          if(readString.indexOf("?relay1off") >0)//checks for off
          {
            digitalWrite(RELAY_CH1, LOW);    // set pin 4 low
            //Serial.println("Led Off");
            client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/off.png' />");
	    //client.println("Light 1 Is Off");
            client.println("<br />");
          }
        }
          
          
  //        if(readString.indexOf("?relay2on") >0)//checks for on
  //       {
  //           digitalWrite(RELAY_CH2, HIGH);    // set pin 4 high
  //           //Serial.println("Led On");
			
  //           client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/on.png' />");
  //           //client.println("Light 1 Is On");
  //           client.println("<br />");
	// }
  //       else{
  //         if(readString.indexOf("?relay2off") >0)//checks for off
  //         {
  //           digitalWrite(RELAY_CH2, LOW);    // set pin 4 low
  //           //Serial.println("Led Off");
  //           client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/off.png' />");

	//     //client.println("Light 1 Is Off");
  //           client.println("<br />");
  //         }
  //       }
          
          
  //         if(readString.indexOf("?relay3on") >0)//checks for on
  //       {
  //           digitalWrite(RELAY_CH3, HIGH);    // set pin 4 high
  //           //Serial.println("Led On");
			
  //           client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/on.png' />");
  //           //client.println("Light 1 Is On");
  //           client.println("<br />");
	// }
  //       else{
  //         if(readString.indexOf("?relay3off") >0)//checks for off
  //         {
  //           digitalWrite(RELAY_CH3, LOW);    // set pin 4 low
  //           //Serial.println("Led Off");
  //           client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/off.png' />");

	//     //client.println("Light 1 Is Off");
  //           client.println("<br />");
  //         }
  //       }
        
  //       if(readString.indexOf("?relay4on") >0)//checks for on
  //       {
  //           digitalWrite(RELAY_CH4, HIGH);    // set pin 4 high
  //           //Serial.println("Led On");
			
  //           client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/on.png' />");
  //           //client.println("Light 1 Is On");
  //           client.println("<br />");
	// }
  //       else{
  //         if(readString.indexOf("?relay4off") >0)//checks for off
  //         {
  //           digitalWrite(RELAY_CH4, LOW);    // set pin 4 low
  //           //Serial.println("Led Off");
  //           client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/off.png' />");

	//     //client.println("Light 1 Is Off");
  //           client.println("<br />");
  //         }
  //       }
        
  //        if(readString.indexOf("?relay5on") >0)//checks for on
  //       {
  //           digitalWrite(RELAY_CH5, HIGH);    // set pin 4 high
  //           //Serial.println("Led On");
			
  //           client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/on.png' />");
  //           //client.println("Light 1 Is On");
  //           client.println("<br />");
	// }
  //       else{
  //         if(readString.indexOf("?relay5off") >0)//checks for off
  //         {
  //           digitalWrite(RELAY_CH5, LOW);    // set pin 4 low
  //           //Serial.println("Led Off");
  //           client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/off.png' />");

	//     //client.println("Light 1 Is Off");
  //           client.println("<br />");
  //         }
  //       }
          
          
  //        if(readString.indexOf("?relay6on") >0)//checks for on
  //       {
  //           digitalWrite(RELAY_CH6, HIGH);    // set pin 4 high
  //           //Serial.println("Led On");
			
  //           client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/on.png' />");
  //           //client.println("Light 1 Is On");
  //           client.println("<br />");
	// }
  //       else{
  //         if(readString.indexOf("?relay6off") >0)//checks for off
  //         {
  //           digitalWrite(RELAY_CH6, LOW);    // set pin 4 low
  //           //Serial.println("Led Off");
  //           client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/off.png' />");

	//     //client.println("Light 1 Is Off");
  //           client.println("<br />");
  //         }
  //       }
          
          
  //         if(readString.indexOf("?relay7on") >0)//checks for on
  //       {
  //           digitalWrite(RELAY_CH7, HIGH);    // set pin 4 high
  //           //Serial.println("Led On");
			
  //           client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/on.png' />");
  //           //client.println("Light 1 Is On");
  //           client.println("<br />");
	// }
  //       else{
  //         if(readString.indexOf("?relay7off") >0)//checks for off
  //         {
  //           digitalWrite(RELAY_CH7, LOW);    // set pin 4 low
  //           //Serial.println("Led Off");
  //           client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/off.png' />");

	//     //client.println("Light 1 Is Off");
  //           client.println("<br />");
  //         }
  //       }
        
  //       if(readString.indexOf("?relay8on") >0)//checks for on
  //       {
  //           digitalWrite(RELAY_CH8, HIGH);    // set pin 4 high
  //           //Serial.println("Led On");
			
  //           client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/on.png' />");
  //           //client.println("Light 1 Is On");
  //           client.println("<br />");
	// }
  //       else{
  //         if(readString.indexOf("?relay8off") >0)//checks for off
  //         {
  //           digitalWrite(RELAY_CH8, LOW);    // set pin 4 low
  //           //Serial.println("Led Off");
  //           client.println("<link rel='apple-touch-icon' href='http://chriscosma.co.cc/off.png' />");

	//     //client.println("Light 1 Is Off");
  //           client.println("<br />");
  //         }
  //       }
                    
        // INPUT
          
          
          // output the value of each analog input pin
          //for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
          //  int sensorReading = analogRead(analogChannel);
          //  client.print("analog input ");
          //  client.print(analogChannel);
          //  client.print(" is ");
          //  client.print(sensorReading);
          //  client.println("<br />");       
          //}
                   
          // client.println("<hr> <p> By <a href=\"http://androidcontrol.blogspot.com\"></p><p style=\"font-family:arial;color:blue;font-size:20px;\">Android Control Blogspot</p></a>");
                  
          //  readString="";
          //  client.println("</body>");         
//           client.println("</html>");
         
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
    //client.println("<meta http-equiv=\"refresh\" content=\"10;url=http://192.168.0.200/\"/>" );
    
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
    
  }//if(Client)
  
}//Loop
