
//   <fieldset class="home-assistant" style="width:400px;">
//   <legend>Home Assistant</legend>
//   <fieldset class="col-lg-2">
<legend>Sensors</legend>
Bedroom Temp: <strong id="bedroomTempValue"><span class="badge bedroom">76</span></strong></br >
    Livingroom Temp: <strong id="livingroomTempValue"><span class="badge livingroom">76</span></strong> </br >
        Kitchen Temp: <strong id="kitchenTempValue"><span class="badge kitchn">76</span></strong> </br >
            Bathroom Temp: <strong id="bathroomTempValue"><span class="badge bathroom">76</span></strong> </br > 
  </fieldset >
    <fieldset class="col-lg-2">
        <legend>Relays Status</legend>
        Light Status: <strong id="created"><span class="badge lightstatus">on</span></strong> </br>
Humidity Status: <strong id="created"><span class="badge himidity status">off</span></strong> </br > 
  </fieldset >
</fieldset >


var wrapper = document.createElement("div");

var homeassistant = document.createElement("fieldset");
homeassistant.className = "home-assistant";

var homeassistantLegend = document.createElement("legend");
homeassistantLegend.innerHTML = "Home Assistant";
homeassistant.appendChild(homeassistantLegend);

var sensors = document.createElement("fieldset");
sensors.className = "sensors-assistant";
homeassistant.appendChild(sensors);

var sensorsLegend = document.createElement("legend");
sensorsLegend.innerHTML = "Sensors";
sensors.appendChild(sensorsLegend);

var relays = document.createElement("fieldset");
relays.className = "relays-assistant";

var relaysLegend = document.createElement("legend");
relaysLegend.innerHTML = "Relays Status";
relays.appendChild(relaysLegend);
homeassistant.appendChild(relays);
wrapper.appendChild(homeassistant);

return wrapper;
