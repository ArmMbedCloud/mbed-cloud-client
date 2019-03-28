
#ifndef _MQTT_DATA_PROVIDER_H_
#define _MQTT_DATA_PROVIDER_H_

#include "NetworkInterface.h"
#include "MQTTThreadedClient.h"

class MQTTDataProvider{
 public:
     MQTTDataProvider( const char* aDeviceId, 
     	               map<string, M2MResource*>  aResources
                     ):
            deviceId(aDeviceId), 
            resources(aResources)
       {
       }

    ~MQTTDataProvider(){}

    void setupMQTTClient(NetworkInterface* network);
    void startSender();
    void readValues();
    void run(NetworkInterface* network);
    string getData(); //returns JSON as described here: https://confluence.arm.com/display/IoTBU/Message+Structure
    string getDataOld(int counter); //returns JSON in format used in 1st demo with plotting
    void publish_data(string key, string value);

    const char* deviceId;
    map<string, M2MResource*> resources;

    MQTT::MQTTThreadedClient* mqtt;

};
#endif
