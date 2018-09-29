#include "MQTTSwitch.hpp"

void MQTTSwitch::setup( GMQTT* pClient, String pName, String pStateTopic )
{
	MQTTDevice::setup( pClient, pName, pStateTopic );
	theClient->subscribe( theStateTopic + "/set" );
}

void MQTTSwitch::setState( bool pOn, bool pForce )
{
	if( theState != pOn || pForce )
	{
		theState = pOn;
		theClient->publish( theStateTopic, pOn ? "ON" : "OFF", true );
	}
}

bool MQTTSwitch::getState()
{
	return theState;
}

String MQTTSwitch::getType()
{
	return String( "switch" );
}

String MQTTSwitch::getJSON()
{
	String lJSON = "\"command_topic\": \"" + theStateTopic + "/set\", \"availability_topic\": \"" + theClient->getAvailabilityTopic() + "\", \"payload_on\": \"ON\", \"payload_off\": \"OFF\", \"state_on\": \"ON\", \"state_off\": \"OFF\", \"payload_available\": \"ON\", \"payload_not_available\": \"OFF\"";
	return lJSON;
}

void MQTTSwitch::newMessage( String pTopic, String pMessage )
{
	if( pTopic == theStateTopic + "/set" )
	{
		Serial.print( String( millis() ) );
		Serial.print( ": " );
		Serial.print( pMessage );
		Serial.print( " from: " );
		Serial.println( pTopic );
		setState( pMessage == "ON" ? true : false );
	}
}

void MQTTSwitch::reconnected()
{
	theClient->subscribe( theStateTopic + "/set" );
	setState( theState, true );
}

void MQTTSwitch::chirp()
{
	setState( theState, true );
}
