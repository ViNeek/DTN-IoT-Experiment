#ifndef __IOT_NETWORK__H
#define __IOT_NETWORK__H

#include <iot/types.h>
#include <iot/dtn.h>

#define IOT_RCV_BUFFER_MAX 512
#define IOT_SND_BUFFER_MAX 512

#define IOT_RIME_BROADCAST_CHANNEL 129
#define IOT_RIME_UNICAST_CHANNEL 146
#define IOT_RIME_ADDRESS_LENGTH 16

iotInt32 iot_init_networking();
iotInt32 iot_uninit_networking();

// Access callback arrays
const struct unicast_callbacks* iot_mule_unicast_callbacks();
const struct broadcast_callbacks* iot_mule_broadcast_callbacks();
const struct unicast_callbacks* iot_client_unicast_callbacks();
const struct broadcast_callbacks* iot_client_broadcast_callbacks();

iotInt32 iot_broadcast(struct iotNetworkEntity* broadcaster, iotChar* data, iotInt32 len);
iotInt32 iot_send(struct iotNetworkEntity* sender, struct iotNetworkEntity* receiver);
iotInt32 iot_recv(struct iotNetworkEntity* receiver, struct iotNetworkEntity* sender);

const iotChar* iot_node_address();

#endif // __IOT_NETWORK__H
