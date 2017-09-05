#include "GMQTT.hpp"
#include "MQTTDevices/MQTTBinarySensor.hpp"

GMQTT::GMQTT()
{
}

void GMQTT::setup( String pSystemName, const char* pHost, int pPort, bool pAddStateSensor, bool pDoDiscovery )
{
	theSystemName = pSystemName;
	theClient.setClient( theWifi );
	theClient.setServer( pHost, pPort );
	theClient.setCallback( [ = ](char *pTopic, byte *pPayload, unsigned int pLength){
		this->callback( pTopic, pPayload, pLength );
	} );
	if( pAddStateSensor )
	{
		theStateSensor = new MQTTBinarySensor();
		theStateSensor->setup( this, theSystemName + "system", "system/" + pSystemName, "connectivity" );
	}
	theDoDiscovery = pDoDiscovery;
}

void GMQTT::callback(char* pTopic, byte* pPayload, unsigned int pLength)
{
	String lPayload = "";
	for (int i = 0; i < pLength; i++)
	{
		lPayload += (char)pPayload[i];
	}
	String lTopic = pTopic;

	for( int i = 0; i < theListeners.size(); i++ )
	{
		theListeners[i]->newMessage( lTopic, lPayload );
	}
}

void GMQTT::reconnect()
{
	if( !theClient.connected() )
	{
		Serial.print("GMQTT " );
		Serial.print( "( " );
		Serial.print( theClientID );
		Serial.print( " ) Connecting.... ");
		if( theClient.connect( theClientID.c_str(), theUserID.c_str(), thePassword.c_str(), theStateSensor->getStateTopic().c_str(), 2, true, "OFF" ) )
		{
			Serial.println(" Connected");
			theStateSensor->sendDiscovery();
			theStateSensor->setState( true );
		}
		else
		{
			Serial.print("failed, Error = " );
			Serial.print( theClient.state() );
		}
	}
}

void GMQTT::connect( String pClientID, String pUser, String pPassword )
{
	theClientID = pClientID;
	theUserID = pUser;
	thePassword = pPassword;

	reconnect();
}

void GMQTT::subscribe( String pTopic )
{
	theClient.subscribe( pTopic.c_str() );
}

void GMQTT::publish( String pTopic, String pMessage, bool pRetain )
{
	Serial.print( "Sending " );
	Serial.print( pMessage );
	Serial.print( " to " );
	Serial.println( pTopic );
	theClient.publish( pTopic.c_str(), pMessage.c_str(), pRetain );
}

void GMQTT::loop()
{
	reconnect();
	theClient.loop();
}

void GMQTT::addListener(MQTTListener *pListener)
{
	theListeners.push_back( pListener );
}
