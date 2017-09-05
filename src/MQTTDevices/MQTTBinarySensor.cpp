#include "MQTTBinarySensor.hpp"

void MQTTBinarySensor::setup( GMQTT* pClient, String pName, String pStateTopic, String pDeviceClass )
{
	MQTTDevice::setup( pClient, pName, pStateTopic );
	theDeviceClass = pDeviceClass;
}

void MQTTBinarySensor::setState( bool pOn )
{
	theClient->publish( theStateTopic, pOn ? "ON" : "OFF", true );
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
	Serial.print( "Got a message2: " );
	Serial.print( pMessage );
	Serial.print( " from2: " );
	Serial.println( pTopic );
}
