#ifndef MQTTBinarySensor_h
#define MQTTBinarySensor_h
#include "MQTTDevice.hpp"
#include "../GMQTT.hpp"

class MQTTBinarySensor : public MQTTDevice
{
	public:
		void setup( GMQTT* pClient, String pName, String pStateTopic, String pDeviceClass );
		void setState( bool pOn );
		void newMessage( String pTopic, String pMessage );

	protected:
		String theDeviceClass;
		String getType();
		String getJSON();
};

#endif
