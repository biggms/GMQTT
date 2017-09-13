#include "MQTTBinarySensor.hpp"

void MQTTBinarySensor::setup( GMQTT* pClient, String pName, String pStateTopic, String pDeviceClass )
{
	MQTTDevice::setup( pClient, pName, pStateTopic );
	theDeviceClass = pDeviceClass;
}

void MQTTBinarySensor::setState( bool pOn, bool pForce )
{
	if( theState != pOn || pForce )
	{
		theState = pOn;
		theClient->publish( theStateTopic, theState ? "ON" : "OFF", true );
	}
}

bool MQTTBinarySensor::getState()
{
	return theState;
}

String MQTTBinarySensor::getType()
{
	return String( "binary_sensor" );
}

String MQTTBinarySensor::getJSON()
{
	String lJSON = "\"device_class\": \"" + theDeviceClass + "\"";
	return lJSON;
}

void MQTTBinarySensor::newMessage( String pTopic, String pMessage )
{
	if( pTopic == theStateTopic )
	{
		Serial.print( String( millis() ) );
		Serial.print( ": " );
		Serial.print( pMessage );
		Serial.print( " from: " );
		Serial.println( pTopic );
		theState = pMessage == "ON" ? true : false;
		setState(theState);
	}
}

void MQTTBinarySensor::reconnected()
{
	setState( theState, true );
}
