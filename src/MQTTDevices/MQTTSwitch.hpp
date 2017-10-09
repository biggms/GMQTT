#ifndef MQTTSwitch_h
#define MQTTSwitch_h
#include "MQTTDevice.hpp"
#include "../GMQTT.hpp"

class MQTTSwitch : public MQTTDevice
{
	public:
		void newMessage( String pTopic, String pMessage );
		void reconnected();
		void chirp();

		void setup( GMQTT* pClient, String pName, String pStateTopic );
		void setState( bool pOn, bool pForce = false );
		bool getState();

	protected:
		String theDeviceClass;
		String getType();
		String getJSON();

	private:
		bool theState = false;
};

#endif
