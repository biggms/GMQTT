#include "GMQTT.h"

GMQTT::GMQTT()
{
}


void GMQTT::setup( const char* pSystemName, const char* pHost, int pPort, MQTT_CALLBACK_SIGNATURE )
{
  theSystemName = pSystemName;
  theClient.setClient( theWifi );
  theClient.setServer( pHost, pPort );
  theClient.setCallback( callback );
}

void GMQTT::reconnect()
{
  while( !theClient.connected() )
  {
    Serial.print("GMQTT " );
    Serial.print( "( " );
    Serial.print( theClientID );
    Serial.print( " ) Connecting.... ");
    char lTopic[ 512 ];
    sprintf( lTopic, "system/%s", theSystemName );
    if( theClient.connect( theClientID, theUserID, thePassword, lTopic, 2, true, "OFF" ) )
    {
      Serial.println(" Connected");
      publish( lTopic, "ON", true );

      char lJSON[ 1024 ];
      sprintf( lTopic, "homeassistant/binary_sensor/%ssystem/config", theSystemName );
      sprintf( lJSON, "{ \"name\": \"%ssystem\", \"state_topic\": \"system/%s\", \"device_class\": \"connectivity\" }", theSystemName, theSystemName );
      publish( lTopic, lJSON, true );
    }
    else
    {
      Serial.print("failed, Error = " );
      Serial.print( theClient.state() );
      Serial.println(" .. trying again in 5 seconds" );
      delay( 5000 );
    }
  }
}

void GMQTT::connect( const char* pClientID, const char* pUser, const char* pPassword )
{
  theClientID = pClientID;
  theUserID = pUser;
  thePassword = pPassword;

  reconnect();
}

void GMQTT::subscribe( const char* pTopic )
{
  theClient.subscribe( pTopic );
}

void GMQTT::publish( const char* pTopic, const char* pMessage, bool pRetain )
{
  theClient.publish( pTopic, pMessage, pRetain );
}

void GMQTT::loop()
{
  reconnect();
  theClient.loop();
}
