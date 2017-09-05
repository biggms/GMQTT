#include "MQTTDevice.hpp"

void MQTTDevice::setup( GMQTT* pClient, String pName, String pStateTopic )
{
	theClient = pClient;
	theName = pName;
	theStateTopic = pStateTopic;
	pClient->addListener(this);
}

void MQTTDevice::sendDiscovery()
{
	theClient->publish( getConfigTopic().c_str(), getConfigMessage().c_str(), false );
}

const String MQTTDevice::getStateTopic() const {
	return theStateTopic;
}


String MQTTDevice::getConfigTopic()
{
	String lTopic = "homeassistant/" + getType() + "/" + theName + "/config";
	return lTopic;
}

String MQTTDevice::getConfigMessage()
{
	String lJSON = "{ \"name\": \"" + theName + "\", \"state_topic\": \"" + theStateTopic + "\", " + getJSON() + " }";
	return lJSON;
}
