#include "GMQTT.hpp"
#include "MQTTDevices/MQTTBinarySensor.hpp"

GMQTT::GMQTT()
{
}

void GMQTT::setup( String pSystemName, const char* pHost, int pPort, bool pAddStateSensor, bool pDoDiscovery )
{
	theSystemName = pSystemName;
	theClient.setClient( theWifi );
	theClient.setServer( pHost, pPort );
	theClient.setCallback( [ = ](char *pTopic, byte *pPayload, unsigned int pLength)
	{
		this->callback( pTopic, pPayload, pLength );
	} );
	if( pAddStateSensor )
	{
		theStateSensor = new MQTTBinarySensor();
		theStateSensor->setup( this, theSystemName + "system", "system/" + pSystemName, "connectivity" );
		theStayOnlineTopic = theStateSensor->getStateTopic() + "/stayonline";
	}
	theDoDiscovery = pDoDiscovery;
}

String GMQTT::getAvailabilityTopic()
{
	return "system/" + theSystemName;
}

void GMQTT::callback(char* pTopic, byte* pPayload, unsigned int pLength)
{
	String lPayload = "";
	for (int i = 0; i < pLength; i++)
	{
		lPayload += (char)pPayload[i];
	}
	String lTopic = pTopic;

	if( lTopic == theStayOnlineTopic && pLength == 1 )
	{
		theStayConnected = pPayload[0] == '1';
		if( theStayConnected )
		{
			Serial.println( "Told to stay online" );
		}
		else
		{
			Serial.println( "Told to go offline" );
		}
	}

	for( int i = 0; i < theListeners.size(); i++ )
	{
		theListeners[i]->newMessage( lTopic, lPayload );
	}
}

bool GMQTT::getStayConnected()
{
	return theStayConnected;
}

void GMQTT::reconnect( bool pInitial )
{
	if( !theClient.connected() )
	{
		Serial.print( String( millis() ) );
		Serial.print(": GMQTT " );
		Serial.print( "( " );
		Serial.print( theClientID );
		Serial.print( " ) Connecting .... ");
		if( theClient.connect( theClientID.c_str(), theUserID.c_str(), thePassword.c_str(), theStateSensor->getStateTopic().c_str(), 2, true, "OFF" ) )
		{
			Serial.println(" Connected");
			subscribe( theStayOnlineTopic );
			if( theDoDiscovery )
			{
				theStateSensor->sendDiscovery();
			}
			theStateSensor->setState( true, true );
			if( !pInitial )
			{
				Serial.print( String( millis() ) );
				Serial.println( ": Doing reconnect" );
				for( int i = 0; i < theListeners.size(); i++ )
				{
					theListeners[i]->reconnected();
					//theWifi.flush();
					theClient.loop();
				}
			}
		}
		else
		{
			Serial.print("failed, Error = " );
			Serial.print( theClient.state() );
		}
	}
}

void GMQTT::connect( String pClientID, String pUser, String pPassword )
{
	theClientID = pClientID;
	theUserID = pUser;
	thePassword = pPassword;

	reconnect( true );
}

void GMQTT::subscribe( String pTopic )
{
	theClient.subscribe( pTopic.c_str() );
}

void GMQTT::publish( String pTopic, String pMessage, bool pRetain )
{
	Serial.print( String( millis() ) );
	Serial.print( ": " );
	Serial.print( pMessage );
	Serial.print( " to: " );
	Serial.println( pTopic );
	theClient.publish( pTopic.c_str(), pMessage.c_str(), pRetain );
}

void GMQTT::loop()
{
	reconnect();
	if( theDoChirp && millis() - theLastTime > 60 * 1000 )
	{
		for( int i = 0; i < theListeners.size(); i++ )
		{
			theListeners[i]->chirp();
			theClient.loop();
		}
		theLastTime = millis();
	}
	theClient.loop();
}

void GMQTT::addListener(MQTTListener *pListener)
{
	theListeners.push_back( pListener );
}

void GMQTT::flush()
{
	theClient.loop();
}

void GMQTT::setDoChirp( bool pDoChirp )
{
	theDoChirp = pDoChirp;
}
