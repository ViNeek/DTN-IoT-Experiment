#include <iot/common.h>

#include <iot/mjson.h>

#include <stdio.h>

static iotChar *g_StringPtrs[MAX_TYPE];
static iotChar g_StringBuffer[IOT_PACKET_SIZE];
static iotInt32 g_StringCount;

static const struct json_array_t g_AvailableTypes = {
    .element_type = t_string,
    .arr.strings.ptrs = g_StringPtrs,
    .arr.strings.store = g_StringBuffer,
    .arr.strings.storelen = sizeof(g_StringBuffer),
    .count = &g_StringCount,
    .maxlen = sizeof(g_StringPtrs) / sizeof(g_StringPtrs[0]),
};

// Client broadcast receive callback
// Mainly used to learn the Data Mules address

static void _iot_client_broadcast_recv(struct broadcast_conn *c,
                                       const rimeaddr_t *from) {
  IOT_LOG_INFO("Broadcast message received from %d.%d: '%s'\n", from->u8[0],
         from->u8[1], (char *)packetbuf_dataptr());

  struct iotClient* client = iot_client_ref();

  iot_set_mule_address(from);

  IOT_LOG_INFO("Mule address is %s", iot_mule_address());
}

static void _iot_client_unicast_recv(struct unicast_conn *c,
                                     const rimeaddr_t *from) {
  IOT_LOG_INFO("Unicast message received from %d.%d\n", from->u8[0], from->u8[1]);
}

// Mule Unicast callback
// Mostly used to communicate with clients
static void _iot_mule_unicast_recv(struct unicast_conn *c,
                                   const rimeaddr_t *from) {
  IOT_LOG_INFO("Unicast message received from %d.%d\n", from->u8[0], from->u8[1]);
}

// Mule broadcast callback
// Not much use for now
static void _iot_mule_broadcast_recv(struct broadcast_conn *c,
                                     const rimeaddr_t *from) {
    IOT_LOG_INFO("Broadcast message received from %d.%d\n", from->u8[0], from->u8[1]);

}

// Mule callbacks
static const struct broadcast_callbacks mule_bc_callback = {
    _iot_mule_broadcast_recv};
static const struct unicast_callbacks mule_uc_callback = {
    _iot_mule_unicast_recv};

// Client callbacks
static const struct broadcast_callbacks client_bc_callback = {
    _iot_client_broadcast_recv};
static const struct unicast_callbacks client_uc_callback = {
    _iot_client_unicast_recv};

// Return callback arrays
const struct unicast_callbacks *iot_mule_unicast_callbacks() {
  return &mule_uc_callback;
}

const struct broadcast_callbacks *iot_mule_broadcast_callbacks() {
  return &mule_bc_callback;
}

const struct unicast_callbacks *iot_client_unicast_callbacks() {
  return &client_uc_callback;
}

const struct broadcast_callbacks *iot_client_broadcast_callbacks() {
  return &client_bc_callback;
}

// Local Node RIME address
static iotChar g_NodeRimeAddr[IOT_RIME_ADDRESS_LENGTH];
iotInt32 iot_init_networking() { 
  g_NodeRimeAddr[0] = 0;
  sprintf(g_NodeRimeAddr, "%d.%d", rimeaddr_node_addr.u8[0],rimeaddr_node_addr.u8[1]);

  return 0;
}

iotInt32 iot_uninit_networking() { 
  return 0; 
}

// Init
// iotInt32 iot_client_create(struct iotClient *c) { return 0; }

// iotInt32 iot_broadcaster_create(struct iotBroadcaster *b) { return 0; }

// iotInt32 iot_listener_create(struct iotListener *b) { return 0; }

iotInt32 iot_broadcast(struct iotNetworkEntity* broadcaster, iotChar* data, iotInt32 len) {
  packetbuf_copyfrom(data, len);
  broadcast_send(&broadcaster->m_BC);

  IOT_LOG_INFO("Announcing %s of size %d", data, len);

  return 0;
}

iotInt32 iot_send(struct iotNetworkEntity* sender, struct iotNetworkEntity* receiver){ 
  return 0;
}

iotInt32 iot_recv(struct iotNetworkEntity* receiver, struct iotNetworkEntity* sender){ 
  return 0;
}


const iotChar* iot_node_address() {
  return g_NodeRimeAddr;
}


