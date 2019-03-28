#include <cstdio>

#include "mbed.h"
#include "rtos.h"
#undef MBED_CONF_APP_ESP8266_DEBUG
#include "EthernetInterface.h"
#include "MQTTThreadedClient.h"
#include "mbedtls/platform.h"
#include "m2mresource.h"
#include <pal.h>
#include "MQTTDataProvider.h"

using namespace MQTT;

string MQTTDataProvider::getData(){

   //returns JSON as described here: https://confluence.arm.com/display/IoTBU/Message+Structure
   char str_time[64];

   uint64_t currTimeSeconds = pal_osGetTime();
   long long int currTimeMillis = currTimeSeconds * 1000;
   sprintf(str_time, "%lld", currTimeMillis);

   string json="{\"f\": \"1\",";                        
   json += "\"id\": \"";                                 
   json += deviceId;                                   
   json += "\",";
   json += "\"d\": [";                                
   
   size_t j=0; // resource counter - it is used to print or not to print the comma after }
   for( std::map<string,M2MResource*>::const_iterator it = resources.begin(); it != resources.end(); ++it )
   {

      // printf ("%d=> resource key=%s  value=%f \r\n",j,(it->first).c_str(), (it->second)->get_value_float());
         
      j++;
      json += "{";
      json += "\"";
      json += it->first;   //resource_path
      json += "\": [";
      json += "{";
      json += "\"t\": ";
      json +=str_time;
      json += ",";

      json += "\"v\": {";
             
      json += "\"";
      json += (it->second)->resource_type();   
      json += "\":";

      // TODO: figure the data type of the value, and use appropriate method; also need to construct json based on that.
      // TODO: handle values larger than 100?
      char val_string[100];
      sprintf(val_string, "%f", (it->second)->get_value_float());
      json += val_string;
      json += "}";
      json += "}";
      json += "]";

      json += "}";
      if (resources.size() > j) json += ",";
         json += "";
         
   }

   json += "]}";
     
   // printf ("Returning json: %s \r\n", json.c_str());
   return json;
}    

void MQTTDataProvider::readValues() {

    printf("IN READ VALUES\n");
    while(true)
    {
         Thread::wait(1 * 2 * 1000);
         PubMessage message;
         message.qos = QOS0;
         message.id = 123;
        
         // if (i > 3) continue;  // This is temporary statement to concentrate on TLS handshake issue
                  
         strcpy(&message.topic[0], topic_1);
         

         string json=getData();

         if  (json.length() >= MAX_MQTT_PAYLOAD_SIZE){
            printf("ERROR json lengh > %d  \r\n", MAX_MQTT_PAYLOAD_SIZE);
            break;
         }
 
         sprintf(&message.payload[0], "%s", json.c_str());
         printf("sending to topic=%s payload=%s\r\n", &message.topic[0],   &message.payload[0] );

         message.payloadlen = strlen((const char *) &message.payload[0]);
         // ADS DEBUG int ret = mqtt->publish(message);
         // if (ret) printf("ERROR mqtt.publish() ret=%d  ", ret);
         // if (ret) Thread::wait(6000);
     }
} 
