#ifndef MQTTBinarySensor_h
#define MQTTBinarySensor_h
#include "MQTTDevice.hpp"
#include "../GMQTT.hpp"

class MQTTSensor : public MQTTDevice
{
	public:
		void newMessage( String pTopic, String pMessage );
		void reconnected();

		void setup( GMQTT* pClient, String pName, String pStateTopic, String pUnits );
		void setValue( String pStatus, bool pForce = false );

	protected:
		String theValue;
		String theUnits;
		String getType();
		String getJSON();
};

#endif
