var wrapper = document.createElement("fieldset");
wrapper.className = "home-assistant";

var homeassistantLegend = document.createElement("legend");
homeassistantLegend.innerHTML = "Home Assistant";
wrapper.appendChild(homeassistantLegend);

var sensors = document.createElement("fieldset");
sensors.className = "sensors-assistant";
wrapper.appendChild(sensors);

var sensorsLegend = document.createElement("legend");
sensorsLegend.innerHTML = "Sensors";
sensors.appendChild(sensorsLegend);

var relays = document.createElement("fieldset");
relays.className = "relays-assistant";

var relaysLegend = document.createElement("legend");
relaysLegend.innerHTML = "Relays Status";
relays.appendChild(relaysLegend);
wrapper.appendChild(relays);
// arduino


var jsonData = {
    "tempSensors": [
        { "temp": 20, "badge": "Bedroom Temp :", "unit": "°C" },
        { "temp": 20, "badge": "Livin Temp :", "unit": "°C" },
        { "temp": 20, "badge": "Kitchen Temp :", "unit": "°C" },
        { "temp": 20, "badge": "Bathroom Temp :", "unit": "°C" }],
    "humiditySensors": [
        { "humidity": 20, "badge": "Bedroom humidity :", "unit": "%" },
        { "humidity": 20, "badge": "Living humidity :", "unit": "%" },
        { "humidity": 20, "badge": "Kitchen humidity :", "unit": "%" },
        { "humidity": 20, "badge": "Bathroom humidity :", "unit": "%" }],
    "relayStatus":
    {
        "bedroom":
            { "status": true, "trigger": "<a href='/?relay1on'>on<a>" }
    }
}

for (var i = 0; i < jsonData.tempSensors.length; i++) {
    console.log(jsonData.tempSensors[i].badge)


    sensors.insertAdjacentHTML('beforeend', `${jsonData.tempSensors[i].badge}<strong id="bedroomTempValue"><span class="badge bedroom">${jsonData.tempSensors[i].temp} ${jsonData.tempSensors[i].unit}</span></strong></br >`)
}

return wrapper;

