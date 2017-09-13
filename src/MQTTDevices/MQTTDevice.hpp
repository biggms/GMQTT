#ifndef MQTTDevice_h
#define MQTTDevice_h
#include "../GMQTT.hpp"
#include "MQTTListener.hpp"

class MQTTDevice : public MQTTListener
{
	public:
		virtual void newMessage( String pTopic, String pMessage ) = 0;
		virtual void reconnected() = 0;

		virtual void setup( GMQTT* pClient, String pName, String pStateTopic );
		void sendDiscovery();

		const String getStateTopic() const;

	protected:
		GMQTT* theClient;
		String theName;
		String theStateTopic;

		virtual String getType() = 0;
		virtual String getJSON() = 0;

		String getConfigTopic();

	private:

		String getConfigMessage();
};

#endif
