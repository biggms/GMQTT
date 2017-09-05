#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "MQTTListener.hpp"
#include "Vector.hpp"
#include <Arduino.h>
#include <functional>

#ifndef GMQTT_h
#define GMQTT_h

class MQTTBinarySensor;

class GMQTT
{
	public:
		GMQTT();

		void setup( String pSystemName, const char* pHost, int pPort, bool pAddStateSensor, bool pDoDiscovery );
		void connect( String pClientID, String pUser, String pPassword );
		void subscribe( String pTopic );
		void publish( String pTopic, String pMessage, bool pRetain );
		void loop();
		void addListener( MQTTListener* pListener );
		void callback( char* pTopic, byte* pPayload, unsigned int pLength );

	private:
		WiFiClient theWifi;
		PubSubClient theClient;
		Vector<MQTTListener*> theListeners;

		String theSystemName;
		String theClientID;
		String theUserID;
		String thePassword;

		bool theDoDiscovery;

		MQTTBinarySensor* theStateSensor;

		void reconnect();
};

#endif
