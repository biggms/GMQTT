#include "MQTTSensor.hpp"

void MQTTSensor::setup( GMQTT* pClient, String pName, String pStateTopic, String pUnits )
{
	MQTTDevice::setup( pClient, pName, pStateTopic );
	theUnits = pUnits;
}

void MQTTSensor::setValue( String pValue, bool pForce )
{
	if( theValue != pValue || pForce )
	{
		theValue = pValue;
		theClient->publish( theStateTopic, theValue, true );
	}
}

String MQTTSensor::getType()
{
	return String( "sensor" );
}

String MQTTSensor::getJSON()
{
	String lJSON = "\"unit_of_measurement\": \"" + theUnits + "\"";
	return lJSON;
}

void MQTTSensor::newMessage( String pTopic, String pMessage )
{
}

void MQTTSensor::reconnected()
{
	setValue( theValue, true );
}
