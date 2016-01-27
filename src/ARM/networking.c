#include <iot/common.h>

#include <iot/mjson.h>

#include <string.h>
#include <stdio.h>

// Client broadcast receive callback
// Mainly used to learn the Data Mules address

static void _iot_client_broadcast_recv(struct broadcast_conn *c,
                                       const rimeaddr_t *from) {
  iotInt32 len;
  iotChar buff[IOT_PACKET_SIZE];
  iotChar* buffPtr;
  struct iotClient* client = iot_client_ref();

  //IOT_LOG_INFO("Unexpected broadcast message received from %d.%d %s\n", from->u8[0], from->u8[1], packetbuf_dataptr());
  IOT_LOG_INFO("ALIVE %d\n", client->m_InRange);

#if TARGET!=IOT_PLATFORM_SKY
  if ( !client->m_InRange ) {
    return;
  }
#endif

  // Update mule structures
  iot_set_prev_mule_address(iot_mule_address());
  iot_set_mule_address(from);

  // Skip the mule if it' s the same
  if ( iot_mule_discovered() ) {
    if ( !iot_mule_changed() ) {
      return;
    }
  }

  buffPtr = packetbuf_dataptr();
  len = strlen((char*) packetbuf_dataptr());
  //iot_recv(IOT_NETWORK_ENTITY(client), &buffPtr, &len, from);
  //IOT_LOG_INFO("Broadcast message received from %d.%d: '%s'\n", from->u8[0], from->u8[1], buffPtr);

  // Maybe the mule has nothing
  if ( iot_packet_generate_request(client, buffPtr, buff, &len) ) {
    packetbuf_copyfrom(buff, len+1);
    unicast_send(&client->m_UC, from);
    //iot_send(IOT_NETWORK_ENTITY(client), buff, len, from);
  }

  // Send any existing forward request
  if ( client->m_PendingAnnounce ) {
    //iot_client_announce(client);
  }

  //IOT_LOG_INFO("Mule address is %s", iot_mule_address_desc());
  //IOT_LOG_INFO("Interested in %s", buff);
}

static void _iot_client_unicast_recv(struct unicast_conn *c,
                                     const rimeaddr_t *from) {
  iotInt32 len;
  iotChar* buffPtr;

  struct iotClient* client = iot_client_ref();

  buffPtr = packetbuf_dataptr();
  len = strlen((char*) packetbuf_dataptr());
  //iot_recv(IOT_NETWORK_ENTITY(dm), &buffPtr, &len, from);
  //IOT_LOG_INFO("Unicast message received from %d.%d : %s", from->u8[0], from->u8[1], buffPtr);
  
  //iot_recv(IOT_NETWORK_ENTITY(client), &buffPtr, &len, from);
  IOT_LOG_INFO("Available information %s", buffPtr);  
}

// Mule Unicast callback
// Mostly used to communicate with clients
static void _iot_mule_unicast_recv(struct unicast_conn *c,
                                   const rimeaddr_t *from) {
  iotInt32 len;
  iotChar buff[256];
  iotChar* buffPtr;
  
  struct iotDataMule* dm = iot_mule_ref();

  buffPtr = packetbuf_dataptr();
  len = strlen((char*) packetbuf_dataptr());
  //iot_recv(IOT_NETWORK_ENTITY(dm), &buffPtr, &len, from);
  IOT_LOG_INFO("Unicast message received from %d.%d : %s", from->u8[0], from->u8[1], buffPtr);
  
  if ( strstr(buffPtr, "\"F\"") ) {
    // Store packet in cache
    iot_cache_add(&dm->m_PacketCache, buffPtr, len);
  } else {
    // Reply with available info
    iot_packet_generate_response(dm, buffPtr, buff, &len);
    packetbuf_copyfrom(buff, len+1);
    unicast_send(&dm->m_UC, from);
    //iot_send(IOT_NETWORK_ENTITY(dm), buff, len, from);
  }

}

// Mule broadcast callback
// Not much use for now
static void _iot_mule_broadcast_recv(struct broadcast_conn *c,
                                     const rimeaddr_t *from) {
  // Shoud never get here if there is NO other mule
  IOT_LOG_INFO("Unexpected broadcast message received from %d.%d %s\n", from->u8[0], from->u8[1], packetbuf_dataptr());
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

const struct broadcast_callbacks *  iot_mule_broadcast_callbacks() {
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
  //packetbuf_clear ();
  iotInt32 status;
  struct iotDataMule* mule= iot_mule_ref();
  packetbuf_copyfrom(data, len+1);
  status = broadcast_send(&mule->m_BC);

  IOT_LOG_INFO("List of available information Types %s : %d", data, status);

  return 0;
}

iotInt32 iot_send(struct iotNetworkEntity* sender, iotChar* data, iotInt32 len, const rimeaddr_t *to){ 
  //packetbuf_clear ();
  packetbuf_copyfrom(data, len+1);
  //packetbuf_copyfrom(data, len);
  unicast_send(&sender->m_UC, to);

  //IOT_LOG_INFO("Unicasting %s of size %d", data, len);

  return 0;
}

iotInt32 iot_recv(struct iotNetworkEntity* receiver, iotChar** dataPtr, iotInt32* len, const rimeaddr_t *from){ 
  //packetbuf_clear();
  *dataPtr = packetbuf_dataptr();
  *len = strlen((char*) packetbuf_dataptr());

  //IOT_LOG_INFO("Receiving %s of size %d", *dataPtr, *len);

  return 0;
}


const iotChar* iot_node_address() {
  return g_NodeRimeAddr;
}


