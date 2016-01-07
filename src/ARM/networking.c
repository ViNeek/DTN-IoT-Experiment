#include <iot/dtn.h>
#include <iot/networking.h>

#include <stdio.h>

// Client broadcast receive callback
// Mainly used to learn the Data Mules address
static void _iot_client_broadcast_recv(struct broadcast_conn *c,
                                       const rimeaddr_t *from) {
  printf("broadcast message received from %d.%d: '%s'\n", from->u8[0],
         from->u8[1], (char *)packetbuf_dataptr());
}
static void _iot_client_unicast_recv(struct unicast_conn *c,
                                     const rimeaddr_t *from) {}

// Mule Unicast callback
// Mostly used to communicate with clients
static void _iot_mule_unicast_recv(struct unicast_conn *c,
                                   const rimeaddr_t *from) {
  printf("unicast message received from %d.%d\n", from->u8[0], from->u8[1]);
}

// Mule broadcast callback
// Not much use for now
static void _iot_mule_broadcast_recv(struct broadcast_conn *c,
                                     const rimeaddr_t *from) {}

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

iotInt32 iot_init_networking() { return 0; }

iotInt32 iot_uninit_networking() { return 0; }

// Init
// iotInt32 iot_client_create(struct iotClient *c) { return 0; }

// iotInt32 iot_broadcaster_create(struct iotBroadcaster *b) { return 0; }

// iotInt32 iot_listener_create(struct iotListener *b) { return 0; }

// Messaging
iotInt32 iot_client_sendto(struct iotClient *s, struct iotClient *r,
                           struct iotSendPacket *p) {
  return 0;
}

// Closing
iotInt32 iot_client_close(struct iotClient *c) { return 0; }
