#pragma once
//TODO [DEBUG] test behavior/function
#include "config.h"
#include <DHT_U.h>
class ESPdht
{
public:
	ESPdht();
	~ESPdht();
	void init();
	void read();
	void update();
protected:
	int _temp, _humid;
	sensors_event_t _event;
private:
	uint32_t _delayMS;
	sensor_t _sensor;
	DHT_Unified _dht = DHT_Unified(DHT_PIN, DHT_TYPE);
};



