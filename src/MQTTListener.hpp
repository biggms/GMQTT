#ifndef MQTTListener_h
#define MQTTListener_h

class MQTTListener
{
	public:
		virtual void newMessage( String pTopic, String pMessage ) = 0;
		virtual void reconnected() = 0;
		virtual void chirp() = 0;
};

#endif
