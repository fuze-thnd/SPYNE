#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include <Arduino_NetworkConfigurator.h>
#include "configuratorAgents/agents/BLEAgent.h"
#include "configuratorAgents/agents/SerialAgent.h"
void onToleranceDistanceChange();
void onCalibrateTriggerChange();

String postureState;
int chestDistance;
int headDistance;
int toleranceDistance;
bool calibrateTrigger;

KVStore kvStore;
BLEAgentClass BLEAgent;
SerialAgentClass SerialAgent;
WiFiConnectionHandler ArduinoIoTPreferredConnection; 
NetworkConfiguratorClass NetworkConfigurator(ArduinoIoTPreferredConnection);

void initProperties(){
  NetworkConfigurator.addAgent(BLEAgent);
  NetworkConfigurator.addAgent(SerialAgent);
  NetworkConfigurator.setStorage(kvStore);
  // For changing the default reset pin uncomment and set your preferred pin. Use DISABLE_PIN for disabling the reset procedure.
  //NetworkConfigurator.setReconfigurePin(your_pin);
  ArduinoCloud.setConfigurator(NetworkConfigurator);

  ArduinoCloud.addProperty(postureState, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(chestDistance, READ, 3 * SECONDS, NULL);
  ArduinoCloud.addProperty(headDistance, READ, 3 * SECONDS, NULL);
  ArduinoCloud.addProperty(toleranceDistance, READWRITE, ON_CHANGE, onToleranceDistanceChange);
  ArduinoCloud.addProperty(calibrateTrigger, READWRITE, ON_CHANGE, onCalibrateTriggerChange);

}
