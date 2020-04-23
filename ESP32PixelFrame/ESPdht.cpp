#include "ESPdht.h"

ESPdht::ESPdht()
{
	init();
}

ESPdht::~ESPdht()
{
}

void ESPdht::init()
{
	_dht.begin();
	delay(100);
	_dht.temperature().getSensor(&_sensor);
#ifdef DEBUG
	Serial.println(F("------------------------------------"));
	Serial.println(F("Temperature Sensor"));
	Serial.print(F("Sensor Type: ")); Serial.println(_sensor.name);
	Serial.print(F("Driver Ver:  ")); Serial.println(_sensor.version);
	Serial.print(F("Unique ID:   ")); Serial.println(_sensor.sensor_id);
	Serial.print(F("Max Value:   ")); Serial.print(_sensor.max_value); Serial.println(F("°C"));
	Serial.print(F("Min Value:   ")); Serial.print(_sensor.min_value); Serial.println(F("°C"));
	Serial.print(F("Resolution:  ")); Serial.print(_sensor.resolution); Serial.println(F("°C"));
	Serial.println(F("------------------------------------"));
#endif // DEBUG
	// Print humidity sensor details.
	_dht.humidity().getSensor(&_sensor);
#ifdef DEBUG
	Serial.println(F("Humidity Sensor"));
	Serial.print(F("Sensor Type: ")); Serial.println(_sensor.name);
	Serial.print(F("Driver Ver:  ")); Serial.println(_sensor.version);
	Serial.print(F("Unique ID:   ")); Serial.println(_sensor.sensor_id);
	Serial.print(F("Max Value:   ")); Serial.print(_sensor.max_value); Serial.println(F("%"));
	Serial.print(F("Min Value:   ")); Serial.print(_sensor.min_value); Serial.println(F("%"));
	Serial.print(F("Resolution:  ")); Serial.print(_sensor.resolution); Serial.println(F("%"));
	Serial.println(F("------------------------------------"));
#endif // DEBUG
	// Set delay between sensor readings based on sensor details.
	_delayMS = _sensor.min_delay / 1000;
}

void ESPdht::read()
{
	_dht.temperature().getEvent(&_event);
	if (isnan(_event.temperature)) {
		Serial.println(F("Error reading temperature!"));
	}
	else {
		_temp = _event.temperature;
#ifdef DEBUG
		Serial.print(F("Temperature: "));
		Serial.print(_event.temperature);
		Serial.println(F("°C"));
#endif // DEBUG


	}
	// Get humidity event and print its value.
	_dht.humidity().getEvent(&_event);
	if (isnan(_event.relative_humidity)) {
		Serial.println(F("Error reading humidity!"));
	}
	else {
		_humid = _event.relative_humidity;
#ifdef DEBUG
		Serial.print(F("Humidity: "));
		Serial.print(_event.relative_humidity);
		Serial.println(F("%"));
#endif // DEBUG
	}
}

void ESPdht::update()
{
	//TODO [CODE] display Temperature and Humidity
}
