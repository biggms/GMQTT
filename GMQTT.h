#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#ifndef GMQTT_h
#define GMQTT_h

class GMQTT
{
  public:
    GMQTT();

    void setup( const char* pSystemName, const char* pHost, int pPort, MQTT_CALLBACK_SIGNATURE );
    void connect( const char* pClientID, const char* pUser, const char* pPassword );
    void subscribe( const char* pTopic );
    void publish( const char* pTopic, const char* pMessage, bool pRetain );
    void loop();

  private:
    WiFiClient theWifi; 
    PubSubClient theClient;

    const char* theSystemName;

    const char* theClientID;
    const char* theUserID;
    const char* thePassword;

    const char* theDiscoveryTopic;
    const char* theDiscoveryMessage;

    void reconnect();
};

#endif
