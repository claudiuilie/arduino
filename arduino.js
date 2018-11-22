
int bedTemp;
int livTemp;
int kitTemp;
int bathTemp;
int bedHum;
int livHum;
int kitHum;
int bathHum;

{
  "tempSensors": [
      { "temp": bedTemp, "badge": "Bedroom Temp :", "unit": "°C" },
      { "temp": livTemp, "badge": "Livin Temp :", "unit": "°C" },
      { "temp": kitTemp, "badge": "Kitchen Temp :", "unit": "°C" },
      { "temp": bathTemp, "badge": "Bathroom Temp :", "unit": "°C" }],
  "humiditySensors": [
      { "humidity": bedHum, "badge": "Bedroom humidity :", "unit": "%" },
      { "humidity": livHum, "badge": "Living humidity :", "unit": "%" },
      { "humidity": kitHum, "badge": "Kitchen humidity :", "unit": "%" },
      { "humidity": bathHum, "badge": "Bathroom humidity :", "unit": "%" }],
  "relayStatus":
  {
      "bedroom":
          { "status": true, "trigger": "<a href='/?relay1on'>on<a>" }
  }
}

