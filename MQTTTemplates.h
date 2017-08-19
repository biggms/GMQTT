#ifndef MQTTTemplates_h
#define MQTTTemplates_h

class MQTTDevice
{
  public:
    MQTTDevice()
    {
    }

    virtual void setup( GMQTT* pClient, const char* pName, const char* pStateTopic )
    {
      theClient = pClient;
      theName = pName;
      theStateTopic = pStateTopic;
    }

    void sendDiscovery()
    {
      Serial.println( getConfigTopic() );
      Serial.println( getConfigMessage() );
      theClient->publish( getConfigTopic(), getConfigMessage(), true );
    }

  protected:
    GMQTT* theClient;
    const char* theName;
    const char* theStateTopic;
    virtual const char* getType() = 0;
    virtual const char* getJSON() = 0;

  private:
    const char* getConfigTopic()
    {
      char lTopic[ 512 ];
      sprintf( lTopic, "homeassistant/%s/%s/config", getType(), theName );
      return lTopic;
    }

    const char* getConfigMessage()
    {
      char lJSON[ 1024 ];
      sprintf( lJSON, "{ \"name\": \"%s\", \"state_topic\": \"%s\", %s }", theName, theStateTopic, getJSON() );
      return lJSON;
    }
};

class MQTTBinarySensor : public MQTTDevice
{
  public:
    void setup( GMQTT* pClient, const char* pName, const char* pStateTopic, const char* pDeviceClass )
    {
      MQTTDevice::setup( pClient, pName, pStateTopic );
      theDeviceClass = pDeviceClass;
    }

  protected:
    const char* theDeviceClass;
    const char* getType()
    {
      return "binary_sensor";
    }
    const char* getJSON()
    {
      char lJSON[ 1024 ];
      sprintf( lJSON, "\"device_class\": \"%s\"", theDeviceClass );
      return lJSON;
    }

  private:

};

//send discovery message
//register for state changes
//detect state change and allow veto
//allow setter for state

#endif