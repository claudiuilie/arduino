
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
      { "temp": bedTemp, "badge": "Bedroom "},
      { "temp": livTemp, "badge": "Living "},
      { "temp": kitTemp, "badge": "Kitchen "},
      { "temp": bathTemp, "badge": "Bathroom "}],
  "humiditySensors": [
      { "humidity": bedHum, "badge": "Bedroom "},
      { "humidity": livHum, "badge": "Living "},
      { "humidity": kitHum, "badge": "Kitchen "},
      { "humidity": bathHum, "badge": "Bathroom "}],
   "icons":[{
       "bed": "fa-bed",
       "tv": "fa-television",
       "cutlery": "fa-cutlery",
       "bath": "fa-bath",
       "humLow": "fa-thermometer-empty",
       "humMedium": "fa-thermometer-half",
       "humHigh": "fa-thermometer-full",
       "danger": "fa-exclamation-triangle",
       "humWarning":"fa-tint"
   }],   
   "unit":[{
       "temperature":"°" ,
       "humidity": "%"
   }],
  "relayStatus":
  {
      "bedroom":
          { "status": false}
  }
}

